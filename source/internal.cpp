// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010)
// Affiliation: Polish Academy of Sciences
//
// Copyright (C) 2010 The vecds authors
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------

#include "../include/vecds/main_viewer.h"
#include "../include/vecds/main_window.h"
#include "../include/vecds/internal.h"
#include "../include/vecds/additional.h"
#include "../include/vecds/constant.h"
#include "../include/vecds/function.h"
#include <QTranslator>



int Love_function(const gsl_vector *x, void *par, gsl_vector *result_funct) {
  double rad_fact = 1.;
  double nu = 0.35;
  double be = ((struct params *) par)->be;
  double bz = ((struct params *) par)->bz;
  double u0x = ((struct params *) par)->u0x;
  double u0y = ((struct params *) par)->u0y;
  double u0z = ((struct params *) par)->u0z;

  const double distx = gsl_vector_get(x, 0);
  const double disty = gsl_vector_get(x, 1);
  // const double distz = gsl_vector_get(x, 2);

  double r2 = distx*distx+disty*disty;
  double r = sqrt(r2);
  double xx = distx/r;
  double yy = disty/r;
  double r02 = rad_fact*be*be; // radius of inmobile ring relative to which the atoms in the core move up
  const double ux =  u0x - be /(2.*constant::pi)*(atan2(yy,xx)+xx*yy/(2.*(1.-nu)));// - 0.5*be;
  const double uy = u0y + be /(8.*constant::pi*(1.-nu)) * ((1.-nu-nu)*log(r2/r02) + (xx+yy)*(xx-yy));
  const double uz = u0z - bz/(2.*constant::pi)*atan2(yy, xx);
  gsl_vector_set(result_funct, 0, ux);
  gsl_vector_set(result_funct, 1, uy);
  gsl_vector_set(result_funct, 2, uz);
  return GSL_SUCCESS;
}

int Beta_function(const gsl_vector *x, void *par, gsl_matrix *jac) {
  double nu = 0.35;
  double be = ((struct params *) par)->be;
  double bz = ((struct params *) par)->bz;

  const double xx = gsl_vector_get(x, 0);
  const double yy = gsl_vector_get(x, 1);
  // const double zz = gsl_vector_get(x, 2);

  const double y2 = yy*yy;
  const double x2 = xx*xx;
  const double r2 = x2+y2;

  if ( r2<1.e-15 ) {
    cout << " Atom  in the center of dislocation core" << endl;
    gsl_matrix_set(jac, 0, 0, 1.);
    gsl_matrix_set(jac, 0, 1, 0.);
    gsl_matrix_set(jac, 0, 2, 0.);
    gsl_matrix_set(jac, 1, 0, 0.);
    gsl_matrix_set(jac, 1, 1, 1.);
    gsl_matrix_set(jac, 1, 2, 0.);
    gsl_matrix_set(jac, 2, 0, 0.);
    gsl_matrix_set(jac, 2, 1, 0.);
    gsl_matrix_set(jac, 2, 2, 1.);
  } else {
    const double a = be/(4. * constant::pi * (1.-nu) * r2*r2);                  // a = bx/(4. * pi * (1.-n) * r2*r2)
    const double bxx = 1. + a * yy * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2); // u(4) = -a * y * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xx 
    const double byx = a * xx * ((1.-2.*nu)*x2 + (3.-2.*nu)*y2);      // u(5) = -a * x * ((1.-2.*n)*x*x + (3.-2.*n)*y*y) !yx
    const double bzx = bz/(2.*constant::pi) * yy/r2;                            // u(6) = -bz/(2.*pi) * y/r2                       !zx
    const double bxy = -a * xx * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2);     // u(7) =  a * x * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xy
    const double byy = 1. - a * yy * ((1.+2.*nu)*x2 - (1.-2.*nu)*y2); // u(8) =  a * y * ((1.+2.*n)*x*x - (1.-2.*n)*y*y) !yy
    const double bzy = bz/(2.*constant::pi) * xx/r2;                            // u(9) = -bz/(2.*pi) * x/r2                       !zy
    gsl_matrix_set(jac, 0, 0, bxx);
    gsl_matrix_set(jac, 0, 1, bxy);
    gsl_matrix_set(jac, 0, 2, 0.);
    gsl_matrix_set(jac, 1, 0, byx);
    gsl_matrix_set(jac, 1, 1, byy);
    gsl_matrix_set(jac, 1, 2, 0.);
    gsl_matrix_set(jac, 2, 0, bzx);
    gsl_matrix_set(jac, 2, 1, bzy);
    gsl_matrix_set(jac, 2, 2, 1.); }
  return GSL_SUCCESS;
}

int Love_fdf(const gsl_vector *x, void *par, gsl_vector *result_funct, gsl_matrix *jac) {
  Love_function(x, par, result_funct);
  Beta_function(x, par, jac);
  return GSL_SUCCESS;
}





Internal::Internal ()
{
  // Create a bunch of internals:
  this->ap       = new AtomsProperties();
  this->actcrstr = new CrystalStructure ();
  this->crstr    = new CrystalStructure[10];
  this->set0     = new Settings(); 
  this->atoms    = new Atoms();
  this->actdisl  = new Dislocations();
  this->disl     = new Dislocations[100];
  
  // Then do something here...
  this->sliderMove   = false;
  this->img_loaded   = "none";
  this->atoms_loaded = "none";

  this->current_dir = QDir::currentPath();
  this->current_dir.replace(QString("/bin"), QString(""));

  init_atoms();
  init_structures();
  read_settings();

  for (int i=0; i<8; i++) 
    this->visible[i] = set0->vis[i];
  this->mfactor = set0->mfact;

  // Finally zero out a bunch of things:
  this->rot_tensor = this->rot_inv = glm::dmat3(1., 0., 0.,
                                                0., 1., 0.,
                                                0., 0., 1.);
  
  this->Mode       = 0;
  this->choice     = false;
  this->ndisl      = 0;
}

Internal::~Internal ()
{}

void Internal::init_atoms ()
{
  int i = 0;

  QString cd0 = this->current_dir;

  QFile file (cd0.append("/data/internal/data/atoms.babel"));
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
      qWarning("atoms.babel not found");
      return;
    }
  
  QTextStream in(&file);
  //    AtomsProperties ap;
    while (!in.atEnd()) {
      QString line = in.readLine();
      QStringList fields = line.split(' ', QString::SkipEmptyParts); 
      //         int ile = fields.size();
      fields.removeFirst();
      this->ap->namea[i] = fields.takeFirst();
      //         atoms_list << namea;
      this->ap->a_rad1[i] = fields.takeFirst().toFloat();
      this->ap->a_rad2[i] = fields.takeFirst().toFloat();
      //         qWarning("i=%d ile=%d nr=%d", i, ile, nr);
      //         qWarning("i=%d nr=%d r1=%f r2=%f", i, nr, a_rad1[i], a_rad2[i]);
      this->ap->atom_red[i] = fields.takeFirst().toFloat();
      this->ap->atom_green[i] = fields.takeFirst().toFloat();
      this->ap->atom_blue[i] = fields.takeFirst().toFloat();
      //         qWarning("nr=%d, rgb=%f,%f,%f", i, atom_red[i], atom_green[i], atom_blue[i]);
      ++i;
    }
    qWarning("+++++++++++++++  ATOMS INITIALIZED  +++++++++++++++++++");
    //     if  ( i!=117 ) qWarning("aaaa");
}

void Internal::init_structures()
{
  QString line;
  QStringList fields;
  int nf;
  QString cd0=this->current_dir;
  QFile file(cd0.append("/data/internal/data/structures.definitions"));

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
     qWarning("Error - no structures.definitions!");
     return;
  }

  QTextStream in(&file);
  line = in.readLine();
  fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts); 
//  fields.removeFirst(); //
  this->numbcrstr = fields.takeFirst().toInt();
  int nl = 1;
  int index = 0;
  double c2o0, c2o1, c2o2, c2o4, c2o5, c2o7, c2o8;
  while (!in.atEnd()) {
        qWarning("+-------line nr %d", nl);
     line = in.readLine(); ++nl;
     fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
     nf = fields.size();
     if ( nf!=7 ) qWarning("Error - Atom_data 1 line=%d", nl);
     actcrstr->struct_name = fields.takeFirst();
     actcrstr->a = fields.takeFirst().toDouble();
     actcrstr->b = fields.takeFirst().toDouble();
     actcrstr->c = fields.takeFirst().toDouble();
     actcrstr->alpha = fields.takeFirst().toDouble();
     actcrstr->beta  = fields.takeFirst().toDouble();
     actcrstr->gamma = fields.takeFirst().toDouble();
//    qWarning("a=%g, b=%g, c=%g"
     line = in.readLine(); ++nl;
     fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
     nf = fields.size();
     if ( nf!=2 ) qWarning("Error - Atom_data 2 line=%d", nl);
     actcrstr->nchem = fields.takeFirst().toInt();
     actcrstr->ncores = fields.takeFirst().toInt();
     if ( actcrstr->nchem<=0 || actcrstr->ncores<=0 ) {
        qWarning("nchem ncores");
     }
     for (int i=0; i<actcrstr->nchem; i++ ) {
       line = in.readLine(); ++nl;
       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
       nf = fields.size();
       if ( nf!=4 ) qWarning("Error - Atom_data 3 line=%d", nl);
       actcrstr->cr_kind[i] = which_atom(fields.takeFirst());
//     qWarning("***  ***   i=%d, kind=%d", i, actcrstr->cr_kind[i]);
       actcrstr->cryst[i].setX(read_fraction(fields.takeFirst()));
       actcrstr->cryst[i].setY(read_fraction(fields.takeFirst()));
       actcrstr->cryst[i].setZ(read_fraction(fields.takeFirst()));
//     qWarning("%d - (%g %g %g)", actcrstr->cr_kind[i],
//      actcrstr->cryst[i].x,
//      actcrstr->cryst[i].y,
//      actcrstr->cryst[i].z);
    }
    
    for (int i=0; i<actcrstr->ncores; i++ ) {
       line = in.readLine(); ++nl;
       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
       nf = fields.size();
       if ( nf!=4 ) qWarning("Error - Atom_data 4 line=%d", nl);
       actcrstr->co_name[i] = fields.takeFirst();
       actcrstr->core[i].setX(read_fraction(fields.takeFirst()));
       actcrstr->core[i].setY(read_fraction(fields.takeFirst()));
       actcrstr->core[i].setZ(read_fraction(fields.takeFirst()));
    }
    
    c2o0 = actcrstr->a; //0
//    c2o3 = 0.;   //3
//    c2o6 = 0.;   //6
    double cosa6 = cos(constant::deg2rad*actcrstr->gamma);
    double sina6 = sin(constant::deg2rad*actcrstr->gamma);
    if ( actcrstr->gamma==90. ) {
       cosa6 = 0.;
       sina6 = 1.;
    }
    c2o1 = actcrstr->b * cosa6; //1
    c2o4 = actcrstr->b * sina6;//4
    c2o7 = 0.;//7
    double cosa5 = cos(constant::deg2rad*actcrstr->beta);
    double cosa4 = cos(constant::deg2rad*actcrstr->alpha);
    if ( actcrstr->alpha==90. ) cosa4 = 0.;
    if ( actcrstr->beta==90. ) cosa5 = 0.;
    c2o2 = actcrstr->c * cosa5;//2
    c2o5 = actcrstr->c * (cosa4-cosa5*cosa6)/sina6;//5
    c2o8 = actcrstr->c * 
       sqrt(1.+2.*cosa4*cosa5*cosa6-cosa4*cosa4-cosa5*cosa5-cosa6*cosa6)/sina6;//8
       
//    actcrstr->o2c = invert(actcrstr->c2o);

    actcrstr->C2O = glm::dmat3(c2o0, 0, 0., 
                               c2o1, c2o4, c2o7,
                               c2o2, c2o5, c2o8);
    actcrstr->O2C = glm::inverse(actcrstr->C2O);
    actcrstr->c2o = Mat9d(actcrstr->C2O);
    actcrstr->o2c = Mat9d(actcrstr->O2C);

/*
  qWarning(":::::::::::::::::: matrix c2o:");
  qWarning("%g %g %g", 
    actcrstr->c2o[0], actcrstr->c2o[1], actcrstr->c2o[2]);
  qWarning("%g %g %g", 
    actcrstr->c2o[3], actcrstr->c2o[4], actcrstr->c2o[5]);
  qWarning("%g %g %g", 
    actcrstr->c2o[6], actcrstr->c2o[7], actcrstr->c2o[8]);
  qWarning("::::::::::::::::::  matrix o2c:");
  qWarning("%g %g %g", 
    actcrstr->o2c[0], actcrstr->o2c[1], actcrstr->o2c[2]);
  qWarning("%g %g %g", 
    actcrstr->o2c[3], actcrstr->o2c[4], actcrstr->o2c[5]);
  qWarning("%g %g %g", 
    actcrstr->o2c[6], actcrstr->o2c[7], actcrstr->o2c[8]);
*/
    crstr[index++] = *actcrstr;
//    ++index;
/*-------------
  qWarning(":::::::::::::::::: matrix test:");
  Mat9d test = matmult(actcrstr->c2o, actcrstr->o2c);
  qWarning("%g %g %g", 
    test[0], test[1], test[2]);
  qWarning("%g %g %g", 
    test[3], test[4], test[5]);
  qWarning("%g %g %g", 
    test[6], test[7], test[8]);
*/
  }
  if ( index!=numbcrstr ) qWarning("Error - Atom_data - number of lines");
  actcrstr = &crstr[0];
//  at_dat_num = 0;
 qWarning("============== init structures O.K.");
}

void Internal::read_settings()
{
  QString cd0=this->current_dir;
  QFile file(cd0.append("/data/internal/data/settings.set0"));

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
     qWarning("===== ERROR    file 'settings.set0' not found");
     return;
  }

  QTextStream in(&file);
  int count = 0;
  int r, g, b;
//  Settings set0;
  while (!in.atEnd()) {
     QString line = in.readLine();
     if ( line.startsWith("#") ) continue;
     QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
     int nf = fields.size();
//         fields.removeFirst();
     if ( count==0 ) {
         this->set0->fov = fields.takeFirst().toFloat();
         goto endloop;
     }
     if ( count==1 ) {
         this->set0->mfact = fields.takeFirst().toDouble();
         goto endloop;
     }
     if ( count==2 ) {
         if ( nf!=8 ) qWarning("Settings - line 3 - error!");
         for (int i=0; i<8; i++) this->set0->vis[i] = (fields.takeFirst().toInt()==0)? false : true;
         goto endloop;
     }
//         atoms_list << namea;
     r = fields.takeFirst().toInt();
     g = fields.takeFirst().toInt();
     b = fields.takeFirst().toInt();
     this->set0->colour_spectrum[count-3] = Int3(r, g, b);
 endloop:
     ++count;
  }
 qWarning("Settings :   count=%d", count);

}


void Internal::read_alc_xyz(QString aname)
{
//  qWarning(" read_alc_xyz");
    this->a_min_ = QVector3D(1.e15, 1.e15, 1.e15);
    this->a_max_ = QVector3D(-1.e15, -1.e15, -1.e15);
    QFile file(aname);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       qWarning("===== ERROR  file %s not found", aname.toAscii().data());
       return;
    }
//    if ( VS->atoms_loaded!=aname && VS->atoms_loaded!="none" ) {
    if ( this->atoms->n_atoms>0 ) {
       delete [] this->atoms->type;
       delete [] this->atoms->coord;
//   qWarning("old atoms deleted -- coords");
    }
    if ( this->atoms->n_bonds>0 ) {
       delete [] this->atoms->bonds;
//  qWarning("old atoms deleted  -- bonds");
    }

    QTextStream in(&file);
    QString line = in.readLine();
    QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    this->atoms->n_atoms = fields.takeFirst().toInt();
    if ( this->atoms->n_atoms>0 ) {
       this->atoms->type = new int[this->atoms->n_atoms];
       this->atoms->coord = new QVector3D[this->atoms->n_atoms];
       this->atoms->u = new QVector3D[this->atoms->n_atoms];
       this->atoms->coord1 = new glm::dvec3[this->atoms->n_atoms];
       this->atoms->du = new glm::dvec3[this->atoms->n_atoms];
    }
//    atom_show = new bool[n_atoms];
//    additional = new int[n_atoms];
//    at_bool = new bool[n_atoms];
//    u = new QVector3D[n_atoms];
    bool alc_ = aname.contains(".alc");
    if ( alc_ ) {
       fields.removeFirst();
       this->atoms->n_bonds = fields.takeFirst().toInt();
       if ( this->atoms->n_bonds>0 ) {
//    qWarning("Actual->n_bonds = %d", Actual->n_bonds);
          this->atoms->bonds = new Int2[this->atoms->n_bonds];
//          bonds2 = new int[n_bonds];
       }
    } else {  // !alc_
       this->atoms->n_bonds = 0;
       line = in.readLine();
    }
  qWarning("READ_ALC     ---   n_atoms = %d", this->atoms->n_atoms);
    for (unsigned int i=0; i<this->atoms->n_atoms; i++) {
       line = in.readLine();
       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
//       int nf = fields.size(); 
       if ( alc_ ) fields.removeFirst();
       QString nam_a = fields.takeFirst();
       int ak = which_atom(nam_a);
       if ( ak==0 ) qWarning("unknown atom in line nr. %d in %s",
                                         i+1, aname.toAscii().data());
       this->atoms->type[i] = ak;
       this->atoms->coord[i].setX(fields.takeFirst().toDouble());// *1.e10;
       this->atoms->coord[i].setY(fields.takeFirst().toDouble());// *1.e10;
       this->atoms->coord[i].setZ(fields.takeFirst().toDouble());// *1.e10;
    }
    minmax3(this->atoms->coord, this->atoms->n_atoms, this->a_min_, this->a_max_);
    this->atoms_loaded = aname;
    if ( !alc_ ) return;
    for (unsigned int i=0; i<this->atoms->n_bonds; i++) {
       line = in.readLine();
       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
       fields.removeFirst();
       this->atoms->bonds[i].i1 = fields.takeFirst().toInt();
       this->atoms->bonds[i].i2 = fields.takeFirst().toInt();
//       qWarning("bonds: %d %d", Actual->bonds1[i], Actual->bonds2[i]);
    }
}



int Internal::which_atom(QString nam_a)
{
  for (int i=1; i<125; i++)
     if ( nam_a.startsWith(this->ap->namea[i], Qt::CaseInsensitive) ) return i;
  return 0;
}

double Internal::read_fraction(QString line)
{
  int i_fr;
  if ( (i_fr=line.indexOf("/", 0))<0 ) return line.toDouble();
  else  return double(line.left(i_fr).toInt())/double(line.mid(i_fr+1).toInt());
}

void Internal::minmax3(QVector3D *vec, int numb, QVector3D &vmin, QVector3D &vmax)
{
  double mxx = -1.e15;  double mxy = -1.e15;  double mxz= -1.e15;
  double mnx = 1.e15;   double mny = 1.e15;   double mnz= 1.e15;
  for (int i=0; i<numb; i++) {
     QVector3D temp = vec[i];
     if ( temp.x()>mxx ) mxx = temp.x();
     if ( temp.y()>mxy ) mxy = temp.y();
     if ( temp.z()>mxz ) mxz = temp.z();
     if ( temp.x()<mnx ) mnx = temp.x();
     if ( temp.y()<mny ) mny = temp.y();
     if ( temp.z()<mnz ) mnz = temp.z();
  }
  vmax = QVector3D(mxx, mxy, mxz);
  vmin = QVector3D(mnx, mny, mnz);
}

void Internal::minmax1(double *vec, int numb, double &vmin, double &vmax)
{
  double mxx = -1.e15;
  double mnx = 1.e15;
  for (int i=0; i<numb; i++) {
     double temp = vec[i];
     if ( temp>mxx ) mxx = temp;
     if ( temp<mnx ) mnx = temp;
  }
  vmax = mxx;
  vmin = mnx;
}

void Internal::do_axis_rotation(Mat9d r_tens)
{
  this->axeX = matvecmult(r_tens, this->axeX);
  this->axeY = matvecmult(r_tens, this->axeY);
  this->axeZ = matvecmult(r_tens, this->axeZ);
}

void Internal::saveAtoms(QString sname)
{
  QString line;
//  QString cd0=this->current_dir;
//  QFile fil(cd0.append("/data/atoms/").append(sname));
  QFile fil(sname);

// qWarning("file: %s, numChoosedAtoms=%d", sname.toAscii().data(), Actual->num_choosedAtoms);  
  if (!fil.open(QIODevice::WriteOnly)) { 
     qWarning("Cannot open for writing: %s", qPrintable(fil.errorString()));
     return; 
  }
  QTextStream out(&fil);
  out << line.sprintf("%d\n ---\n", atoms->n_atoms);
  for (unsigned int i=0; i<atoms->n_atoms; i++)
     out << line.sprintf("%4s %12.7f %12.7f %12.7f\n",
                   ap->namea[atoms->type[i]].toAscii().data(),
                   atoms->coord[i].x(), atoms->coord[i].y(), atoms->coord[i].z());
}


void Internal::SL_singleDisl(QVector3D rr)
{
//  double a = actcrstr->a;
//  double c = actcrstr->c;
 
  rr += this->cent_;
  actdisl->rrr = rr;// + this->cent_;
  actdisl->burgstr = this->act_disl;
  actdisl->p1 = QVector3D(rr.x(), rr.y(), this->a_min_.z());
  actdisl->p2 = QVector3D(rr.x(), rr.y(), this->a_max_.z());
 qWarning("SL_singleDisl - p1, p2 - (%g %g %g) (%g %g %g)",
    actdisl->p1.x(), actdisl->p1.y(), actdisl->p1.z(), actdisl->p2.x(), actdisl->p2.y(), actdisl->p2.z());

  glm::dvec3 burg_vect = rot_tensor * actcrstr->C2O * glm::dvec3(fraction*indMiller[0], fraction*indMiller[1], fraction*indMiller[2]);

  actdisl->burgers_vector = to_QV(burg_vect);
  actdisl->rotation_tensor = this->rot_tensor;
//  calc_disloc(7, ndisl);
  calc_disl0();

  disl[ndisl++] = *actdisl;
//  actdisl->rrr.z = Actual->cent_.z;
 qWarning("SINGLE DISL %d,  rrr= (%g %g %g)", ndisl-1, 
                 actdisl->rrr.x(), actdisl->rrr.y(), actdisl->rrr.z());
  return; 
}

/*
int Internal::Love_function(const gsl_vector *x, void *par, gsl_vector *result_funct) {
  double rad_fact = 1.;
  double nu = 0.35;
  double be = ((struct params *) par)->be;
  double bz = ((struct params *) par)->bz;
  double u0x = ((struct params *) par)->u0x;
  double u0y = ((struct params *) par)->u0y;
  double u0z = ((struct params *) par)->u0z;
  const double distx = gsl_vector_get(x, 0);
  const double disty = gsl_vector_get(x, 1);
  const double distz = gsl_vector_get(x, 2);
  double r2 = distx*distx+disty*disty;
  double r = sqrt(r2);
  double xx = distx/r;
  double yy = disty/r;
  double r02 = rad_fact*be*be; // radius of inmobile ring relative to which the atoms in the core move up
  const double ux =  u0x - be /(2.*constant::pi)*(atan2(yy,xx)+xx*yy/(2.*(1.-nu)));// - 0.5*be;
  const double uy = u0y + be /(8.*constant::pi*(1.-nu)) * ((1.-nu-nu)*log(r2/r02) + (xx+yy)*(xx-yy));
  const double uz = u0z - bz/(2.*constant::pi)*atan2(yy, xx);
  gsl_vector_set(result_funct, 0, ux);
  gsl_vector_set(result_funct, 1, uy);
  gsl_vector_set(result_funct, 2, uz);
  return GSL_SUCCESS;
}

int Internal::Beta_function(const gsl_vector *x, void *par, gsl_matrix *jac) {
  double nu = 0.35;
  double be = ((struct params *) par)->be;
  double bz = ((struct params *) par)->bz;
  const double xx = gsl_vector_get(x, 0);
  const double yy = gsl_vector_get(x, 1);
  const double zz = gsl_vector_get(x, 2);
  const double y2 = yy*yy;
  const double x2 = xx*xx;
  const double r2 = x2+y2;
  if ( r2<1.e-15 ) {
    cout << " Atom  in the center of dislocation core" << endl;
    gsl_matrix_set(jac, 0, 0, 1.);
    gsl_matrix_set(jac, 0, 1, 0.);
    gsl_matrix_set(jac, 0, 2, 0.);
    gsl_matrix_set(jac, 1, 0, 0.);
    gsl_matrix_set(jac, 1, 1, 1.);
    gsl_matrix_set(jac, 1, 2, 0.);
    gsl_matrix_set(jac, 2, 0, 0.);
    gsl_matrix_set(jac, 2, 1, 0.);
    gsl_matrix_set(jac, 2, 2, 1.);
  } else {
    const double a = be/(4. * constant::pi * (1.-nu) * r2*r2);                  // a = bx/(4. * pi * (1.-n) * r2*r2)
    const double bxx = 1. + a * yy * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2); // u(4) = -a * y * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xx 
    const double byx = a * xx * ((1.-2.*nu)*x2 + (3.-2.*nu)*y2);      // u(5) = -a * x * ((1.-2.*n)*x*x + (3.-2.*n)*y*y) !yx
    const double bzx = bz/(2.*constant::pi) * yy/r2;                            // u(6) = -bz/(2.*pi) * y/r2                       !zx
    const double bxy = -a * xx * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2);     // u(7) =  a * x * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xy
    const double byy = 1. - a * yy * ((1.+2.*nu)*x2 - (1.-2.*nu)*y2); // u(8) =  a * y * ((1.+2.*n)*x*x - (1.-2.*n)*y*y) !yy
    const double bzy = bz/(2.*constant::pi) * xx/r2;                            // u(9) = -bz/(2.*pi) * x/r2                       !zy
    gsl_matrix_set(jac, 0, 0, bxx);
    gsl_matrix_set(jac, 0, 1, bxy);
    gsl_matrix_set(jac, 0, 2, 0.);
    gsl_matrix_set(jac, 1, 0, byx);
    gsl_matrix_set(jac, 1, 1, byy);
    gsl_matrix_set(jac, 1, 2, 0.);
    gsl_matrix_set(jac, 2, 0, bzx);
    gsl_matrix_set(jac, 2, 1, bzy);
    gsl_matrix_set(jac, 2, 2, 1.); }
  return GSL_SUCCESS;
}

int Internal::Love_fdf(const gsl_vector *x, void *par, gsl_vector *result_funct, gsl_matrix *jac) {
  Love_function(x, par, result_funct);
  Beta_function(x, par, jac);
  return GSL_SUCCESS;
}
*/



void Internal::newdisl(unsigned int n_a, bool sw_iter)
{
qWarning("newdisl");
  QVector3D rr = this->atoms->coord[n_a];// + this->cent_;
  actdisl->rrr = rr;// + this->cent_;
  actdisl->burgstr = this->act_disl;
  actdisl->p1 = QVector3D(rr.x(), rr.y(), this->a_min_.z());
  actdisl->p2 = QVector3D(rr.x(), rr.y(), this->a_max_.z());
 qWarning("SL_newDisl - p1, p2 - (%g %g %g) (%g %g %g)",
    actdisl->p1.x(), actdisl->p1.y(), actdisl->p1.z(), actdisl->p2.x(), actdisl->p2.y(), actdisl->p2.z());

  glm::dvec3 burg_vect = rot_tensor * actcrstr->C2O * glm::dvec3(fraction*indMiller[0], fraction*indMiller[1], fraction*indMiller[2]);
//  cout << " mil.indices   " << mil.indices[0] << "    " << mil.indices[1] << "    " << mil.indices[2] << endl;
//  cout << " burgers_vector   " << burgers_vector.x << "    " << burgers_vector.y << "    " << burgers_vector.z << endl;
  glm::dvec3 cd = rot_tensor * to_dvec3(atoms->coord[n_a]);
  be = sqrt(burg_vect.x*burg_vect.x+burg_vect.y*burg_vect.y);
  bz = burg_vect.z;
//  cout << " be=" << be << "     bz=" << bz << endl;
  for (unsigned int i=0; i<atoms->n_atoms; i++) 
    atoms->coord1[i] = rot_tensor * to_dvec3(atoms->coord[i]);

  if ( sw_iter ) {

    const gsl_multiroot_fdfsolver_type *T;
    T = gsl_multiroot_fdfsolver_hybridsj;
    gsl_multiroot_fdfsolver *s; // = gsl_multiroot_fdfsolver_alloc(T, 3);
    gsl_vector *x; // = gsl_vector_alloc(3);

    size_t count;

                                 // The return value of
                                 // gsl_multiroot_fdfsolver_iterate is
                                 // an integer, and so this needs to
                                 // be an integer too to make the two
                                 // comparable.
    int status = 0;
    
    //  const size_t n = 3;
    //    struct params p;
    
    glm::dvec3 diff;
    double crit           = 0.;
    double crit_stop      = 1.e-7;
    unsigned int countN_R = 10;
    //  stringstream sss;
    
    p.be = be;
    p.bz = bz;

    for (unsigned int i=0; i<atoms->n_atoms; i++) {
      //    if ( i>=atoms->n_atoms-n_addAtoms ) continue;
      if ( i==n_a ) { cout<< "Error for n_a=" << n_a << "   i=" << i << endl;  continue; }
      count = 0;

      do {
	count++;
	p.u0x = atoms->du[i].x;
	p.u0y = atoms->du[i].y;
         p.u0z = atoms->du[i].z;
         glm::dvec3 temp = atoms->coord1[i]+atoms->du[i] - cd;
	 //       if ( count==1) fout << i+1 << "    " << temp.x << ", " << temp.y << ", " << temp.z << endl; 
         if ( (temp.x*temp.x+temp.y*temp.y)<1.e-10 ) {
           atoms->du[i] = glm::dvec3(0., 0., 0.);
           cout << " Atom " << i << " in the center of dislocation core" << endl;
	   //           n_errors++;
           goto _END;
         }
         gsl_multiroot_function_fdf f = {&(::Love_function), &(::Beta_function), &(::Love_fdf), 3, &p};
         x = gsl_vector_alloc(3);
         gsl_vector_set(x, 0, temp.x);
         gsl_vector_set(x, 1, temp.y);
         gsl_vector_set(x, 2, temp.z);
	 
         s = gsl_multiroot_fdfsolver_alloc(T, 3);
         gsl_multiroot_fdfsolver_set(s, &f, x);
	 
         status = gsl_multiroot_fdfsolver_iterate(s);

         if ( status ) break;
         diff.x = gsl_vector_get(s->f, 0);
         diff.y = gsl_vector_get(s->f, 1);
         diff.z = gsl_vector_get(s->f, 2);
         atoms->du[i] -= diff;
         status = gsl_multiroot_test_residual(s->f, crit_stop);
      } while ( status == GSL_CONTINUE && count<countN_R );

      crit += diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
//    cout << " --- i = " << i << "     count = " << count << endl;

      gsl_multiroot_fdfsolver_free(s);
      gsl_vector_free(x);
//    gsl_vector_free(s->f);
      continue;
   _END:
      cout << "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << endl;
//      n_Errors++;
    }
//  fout.close();
//    cout << " --- n_a = " << n_a << "     crit = " << crit << endl;

  } else {  // !sw_iter

    for (unsigned int i=0; i<atoms->n_atoms; i++) 
      {
	//    if ( i>=atoms->n_atoms-n_addAtoms ) continue;
	if ( i==n_a ) { cout<< "Error for n_a=" << n_a << "   i=" << i << endl;  continue; }
	glm::dvec3 dist1 = atoms->coord1[i] - cd;
	atoms->du[i] = mixed_u(i, dist1, be, bz);
      }
  }
  
  for (unsigned int i=0; i<atoms->n_atoms; i++) 
    atoms->du[i] = rot_inv*atoms->du[i];
  
  //    atoms->beta[i] = rot_inv*mixed_beta(i, coord1[i]+atoms->du[i]-cd, be, bz)*rot_tensor;} //jeżeli jest potrzebne beta
  
  qWarning("uwaga!");
  disl[ndisl++] = *actdisl;
  //  actdisl->rrr.z = Actual->cent_.z;
  qWarning("NEW DISL %d,  rrr= (%g %g %g)", ndisl-1, 
	   actdisl->rrr.x(), actdisl->rrr.y(), actdisl->rrr.z());

  return; 
}

// ------------------------------------------------

void Internal::calc_disloc(int nr_atom, int disl_num)
{

  int i0 = atomize(actdisl->rrr, nr_atom);
 qWarning("SINGLE DISL  num=%d - i0=%d - coordinates=(%g %g %g)", disl_num, i0,
              atoms->coord[i0].x(), atoms->coord[i0].y(), atoms->coord[i0].z());
  if ( i0==-1 ) qWarning("Error i0 for %d dislocation\n", ndisl);
  QVector3D dislcore;// = matvecmult(actcrstr->c2o, actdisl->dislocation_core);
//dislcore = matvecmult(actcrstr->c2o, actdisl->core);

  dislcore.setX(actdisl->dislocation_core.x()*actcrstr->c2o[0] + 
		actdisl->dislocation_core.y()*actcrstr->c2o[1]);

  dislcore.setY(actdisl->dislocation_core.x()*actcrstr->c2o[3] + 
		actdisl->dislocation_core.y()*actcrstr->c2o[4]);

  dislcore.setZ(actdisl->dislocation_core.x()*actcrstr->c2o[6] + 
		actdisl->dislocation_core.y()*actcrstr->c2o[7]);
  Mat9d rt = Mat9d(this->rot_tensor);
  actdisl->cd = atoms->coord[i0] + matvecmult(rt, dislcore);

  actdisl->i0 = i0;
 qWarning("SINGLE DISL corrected coordinates = (%g %g %g),  i0=%d", 
                     actdisl->cd.x(), actdisl->cd.y(), actdisl->cd.z(), i0);
//  for (int i=0; i<atoms->n_atoms; i++) atoms->coord1[i] = rot_tensor * to_dvec3(atoms->coord[i]);
  for (unsigned int i=0; i<atoms->n_atoms; i++) 
    {
      //    if ( i>=atoms->n_atoms-n_addAtoms ) continue;
      //      if ( i==n_a ) { cout<< "Error for n_a=" << n_a << "   i=" << i << endl;  continue; }
      atoms->coord1[i] = rot_tensor * to_dvec3(atoms->coord[i]);
      glm::dvec3 dist1 = atoms->coord1[i] - to_dvec3(actdisl->cd);
      atoms->du[i] = mixed_u(i, dist1, p.be, p.bz);
    }

  for (unsigned int i=0; i<atoms->n_atoms; i++) 
    atoms->du[i] = rot_inv*atoms->du[i];

  //    atoms->beta[i] = rot_inv*mixed_beta(i, coord1[i]+atoms->du[i]-cd, be, bz)*rot_tensor;} //jeżeli jest potrzebne beta
  
  qWarning("uwaga!");
  disl[ndisl++] = *actdisl;
  //  actdisl->rrr.z = Actual->cent_.z;
  qWarning("NEW DISL %d,  rrr= (%g %g %g)", ndisl-1, 
	   actdisl->rrr.x(), actdisl->rrr.y(), actdisl->rrr.z());
  
  qWarning("Mixed_u end");
  //  emit SIG_needDraw();
}

//---------------------------------------------------------------------

glm::dvec3 Internal::mixed_u(int i, glm::dvec3 rotdist, double be, double bz)
{
  double nu = 0.35;
  double rad_fact = 1.0;
  double r2 = rotdist.x*rotdist.x + rotdist.y*rotdist.y;
  if ( r2<1e-15 ) {
     cout << " Atom " << i << " in the center of dislocation core" << endl;
//     return  glm::dvec3(0.25*be, 0., 0.);} 
     return  glm::dvec3(0., 0., 0.);} 
  else {
     double r = sqrt(r2);
     double x = rotdist.x/r;
     double y = rotdist.y/r;
     double r02 = rad_fact*be*be; // radius of inmobile ring relative to which the atoms in the core move up
     double ux =  be /(2.*constant::pi)*(atan2(y,x)+x*y/(2.*(1.-nu)));// - 0.5*be;
     double uy = -be /(8.*constant::pi*(1.-nu)) * ((1.-nu-nu)*log(r2/r02) + (x+y)*(x-y));
     return glm::dvec3(ux, uy, bz/(2.*constant::pi)*atan2(y, x));} } // mixed_u

glm::dmat3 Internal::mixed_beta(int i, glm::dvec3 rotdist, double be, double bz)
{   
    double nu = 0.35;
    glm::dmat3 b(0.,0.,0.,  0.,0.,0.,  0.,0.,0.);
    double x=rotdist.x;
    double x2=x*x;
    double y = rotdist.y;
    double y2=y*y;
    double r2=x2+y2;
    if ( r2<1.e-15 ) cout << " Atom " << i << " in the center of dislocation core" << endl;
    else {
       double a = be/(4. * constant::pi * (1.-nu) * r2*r2);             // a = bx/(4. * pi * (1.-n) * r2*r2)
       double bxx = -a * y * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2); // u(4) = -a * y * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xx 
       double byx = -a * x * ((1.-2.*nu)*x2 + (3.-2.*nu)*y2); // u(5) = -a * x * ((1.-2.*n)*x*x + (3.-2.*n)*y*y) !yx
       double bzx = -bz/(2.*constant::pi) * y/r2;                       // u(6) = -bz/(2.*pi) * y/r2                       !zx
       double bxy =  a * x * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2); // u(7) =  a * x * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xy
       double byy =  a * y * ((1.+2.*nu)*x2 - (1.-2.*nu)*y2); // u(8) =  a * y * ((1.+2.*n)*x*x - (1.-2.*n)*y*y) !yy
       double bzy = -bz/(2.*constant::pi) * x/r2;                       // u(9) = -bz/(2.*pi) * x/r2                       !zy
//       if ( r2>=1.*(be*be+bz*bz) )                            // on the base of ~/pawel/papers/IMCE09/beta.mac
       b = glm::dmat3( bxx, bxy, 0.,   byx, byy, 0.,    bzx, bzy, 0. );
    }
    return b;
}



int Internal::atomize(QVector3D point, int nr_atom)
{
  int i0 = -1;
  double dist = 1.e33;
//  double nu = 0.35;

  if ( nr_atom>0 ) {
   for (unsigned int i=0; i<atoms->n_atoms; i++) {
     double dst = (point - atoms->coord[i]).length();
     if ( atoms->type[i]==nr_atom && dst<dist ) {
        dist = dst;
        i0 = i;
     }
   }
  } else {
   for (unsigned int i=0; i<atoms->n_atoms; i++) {
     double dst = (point - atoms->coord[i]).length();
     if ( dst<dist ) {
        dist = dst;
        i0 = i;
     }
   }
  }
  return i0;
}

void Internal::calc_disl0()
{

  int i0 = -1;
  double dist = 1.e33;

// atomize
  for (unsigned int i=0; i<atoms->n_atoms; i++) {
     double dst = (actdisl->rrr - atoms->coord[i]).length();
     if ( dst<dist ) {
        dist = dst;
        i0 = i;
     }
  }
 qWarning("SINGLE DISL  - i0=%d - coordinates=(%g %g %g)", i0,
              atoms->coord[i0].x(), atoms->coord[i0].y(), atoms->coord[i0].z());
  if ( i0==-1 ) qWarning("Error i0 for %d dislocation\n", ndisl);
/*
  QVector3D dislcore = matvecmult(actcrstr->c2o, actdisl->core);
//dislcore = matvecmult(actcrstr->c2o, actdisl->core);
  dislcore.x = actdisl->core.x*actcrstr->c2o[0] + actdisl->core.y*actcrstr->c2o[1];
  dislcore.y = actdisl->core.x*actcrstr->c2o[3] + actdisl->core.y*actcrstr->c2o[4];
  dislcore.z = actdisl->core.x*actcrstr->c2o[6] + actdisl->core.y*actcrstr->c2o[7];
*/
  actdisl->cd = atoms->coord[i0];// + matvecmult(this->rot_tensor, dislcore);
  actdisl->i0 = i0;
 qWarning("SINGLE DISL corrected coordinates = (%g %g %g),  i0=%d", 
                     actdisl->cd.x(), actdisl->cd.y(), actdisl->cd.z(), i0);
// qWarning("n_atoms =%d", atoms->n_atoms);

//  double be=sqrt(burg_vect.x*burg_vect.x+burg_vect.y*burg_vect.y);
//  double bz = burg_vect.z;


  for (unsigned int i=0; i<atoms->n_atoms; i++) 
    { 
      atoms->coord1[i] = rot_tensor * to_dvec3(atoms->coord[i]);
      glm::dvec3 dist1 = atoms->coord1[i] - to_dvec3(actdisl->cd);
      atoms->du[i] = mixed_u(i, dist1, be, bz);
    }
  
  for (unsigned int i=0; i<atoms->n_atoms; i++) 
    atoms->du[i] = rot_inv*atoms->du[i];
  //    atoms->beta[i] = rot_inv*mixed_beta(i, coord1[i]+atoms->du[i]-cd, be, bz)*rot_tensor;} //jeżeli jest potrzebne beta
  
  //  emit SIG_needDraw();
}


//---------------------------------------------------------------------

// --------------------------------------------------------------------

void Internal::do_atoms_rotation(Mat9d r_tens, QVector3D vec)
{
  for (unsigned int i=0; i<this->atoms->n_atoms; i++)
    this->atoms->coord[i] = matvecmult(r_tens, this->atoms->coord[i] - vec) + vec;
  //  find_extr_0();
  minmax3(atoms->coord, atoms->n_atoms, a_min_, a_max_);
}


void Internal::do_invis_rotation(Mat9d r_tens, QVector3D vec)
{
  for (int i=0; i<8; i++)
       this->invbox[i] = matvecmult(r_tens, this->invbox[i] - vec) + vec;
//  find_extr_0();
//  minmax(this->atoms->coord, this->atoms->n_atoms, this->a_min_, this->a_max_);
}

void Internal::do_signes_rotation(Mat9d r_tens, QVector3D vec)
{
  for (int j=0; j<this->ndisl; j++) {
     disl[j].p1 = matvecmult(r_tens, disl[j].p1 - vec) + vec;
     disl[j].p2 = matvecmult(r_tens, disl[j].p2 - vec) + vec;
  }
}

/*
int Internal::cubBox(QVector3D box1, QVector3D p0)
{
  int ret=0;
  for (int i=0; i<Actual->n_atoms; i++) {
     double ax = Actual->coord[i].x - p0.x;
     double ay = Actual->coord[i].y - p0.y;
     double az = Actual->coord[i].z - p0.z;
     bool at = ( ax<box1.x && ax>-box1.x && ay<box1.y && ay>-box1.y &&
                                                    az<box1.z && az>-box1.z );
     Actual->at_bool[i] = at;
     if ( at ) Actual->additional[ret++] = i;
  }
  return ret;
}

int Internal::hexBox(double hex_r, double hex_h, QVector3D p0)
{
  int ret = 0;
  bool at;
  double ca, cx;

  double cos30 = 0.5*sqrt(3.);
  double hr = hex_r*cos30;
  for (int i=0; i<Actual->n_atoms; i++) {
     double c_x = Actual->coord[i].x - p0.x;
     double c_y = Actual->coord[i].y - p0.y;
     double c_z = Actual->coord[i].z - p0.z;
     double rad = sqrt(c_x*c_x + c_y*c_y);
     if ( c_z>0.5*hex_h || c_z<-0.5*hex_h || rad>hex_r ) {
        at = false;
        goto endloop;
     }
     if ( rad<hr ) {
        at = true;
        goto endloop;
     }
     if ( c_y<=0.5*hex_r && c_y>=-0.5*hex_r ) {
        at = ( c_x<=hr && c_x>=-hr );
        goto endloop;
     }
     ca = ((c_x>=0. && c_y>0.) || (c_x<0. && c_y<0.))?  -0.5 : 0.5;
     cx = c_x*ca - c_y*cos30;
     at = ( cx<=hr && cx>=-hr );
 endloop:
     Actual->at_bool[i] = at;
     if ( at ) ++ret;
  }
  return ret;
}
*/


void Internal::read_img(QString iname)
{
  this->img.load(iname);
  if ( this->img.isNull() ) {
      qWarning("Unable to load file, unsupported file format");
      return;
  }
}

int Internal::lattice(int nx, int ny, int nz)
{
  int m = 0;
  for (int k=0; k<nz; k++) {
     for (int j=0; j<ny; j++) {
        for (int i=0; i<nx; i++) {
           for (int an=0; an<this->actcrstr->nchem; an++) {
//              QVector3D hic = QVector3D(double(i)+this->actcrstr->cryst[an].x(),
//                                        double(j)+this->actcrstr->cryst[an].y(),
//                                        double(k)+this->actcrstr->cryst[an].z());
//              this->atoms->coord[m] = matvecmult(this->actcrstr->c2o, hic);
              glm::dvec3 hic = glm::dvec3(double(i)+this->actcrstr->cryst[an].x(),
                                          double(j)+this->actcrstr->cryst[an].y(),
                                          double(k)+this->actcrstr->cryst[an].z());
              glm::dvec3 vvv = actcrstr->C2O * hic;
              this->atoms->coord[m] = QVector3D(vvv.x, vvv.y, vvv.z);
              this->atoms->type[m++] = this->actcrstr->cr_kind[an];
           }
        }
     }
  }
  return m;
}

int Internal::lattice2(double sx, double sy, int nz)
{
  int m = 0;
  double sg = sin(constant::deg2rad*this->actcrstr->gamma);
  int nx = int((sx+2.*sy*sqrt(1.-sg*sg))/this->actcrstr->a)+1;
  int ny = int(sy/(this->actcrstr->b*sg))+1;

  for (int k=0; k<nz; k++) {
     for (int j=0; j<ny; j++) {
        for (int i=0; i<nx; i++) {
           glm::dvec3 hic = actcrstr->C2O * glm::dvec3(double(i), double(j), double(k));
           if ( hic.x>=0.&& hic.x<sx && hic.y>=0.&& hic.y<sy ) {
              for (int an=0; an<this->actcrstr->nchem; an++) {
                 glm::dvec3 ccc = glm::dvec3(this->actcrstr->cryst[an].x(), this->actcrstr->cryst[an].y(), this->actcrstr->cryst[an].z());
                 glm::dvec3 vvv = hic + actcrstr->C2O*ccc;
                 this->atoms->coord[m] = QVector3D(vvv.x, vvv.y, vvv.z);
                 this->atoms->type[m++] = this->actcrstr->cr_kind[an];
              }
           }
        }
     }
  }
  return m;
}

void Internal::processMiller(int sw, QString result_text, QString result_text2)
{
//  QString result_text2;
  bool swb;
  Mat9d mat, rt;
  glm::dmat3 rot_in;
  if ( sw==1 ) 
     swb = parse_miller(result_text);
  else swb = (parse_miller(result_text))  && (parse_core(result_text2));
//  if ( !VS->rotChanged ) {
//    qWarning("-----------processMiller  rot not changed");
//    return;
//  }
  if ( swb ) {
     rot_in = this->rot_inv;
     compute_rotation_tensor();
//     this->rot_inv = invert(this->rot_tensor);
//     this->rot_inv = transpose(this->rot_tensor);
//     mat = matmult(this->rot_tensor, rot_in);
   this->rot_inv = glm::transpose(glm::inverse(this->rot_tensor));
   mat = Mat9d(this->rot_tensor * rot_in);
   rt = Mat9d(this->rot_tensor);

  qWarning("######   rot_tensor    ###### det = %g", determinant(this->rot_tensor));
qWarning ("rot 0 = %g %g %g", rt[0], rt[1], rt[2]); 
qWarning ("rot 1 = %g %g %g", rt[3], rt[4], rt[5]); 
qWarning ("rot 2 = %g %g %g", rt[6], rt[7], rt[8]); 

  qWarning("######   mat    ###### det = %g", determinant(mat));
qWarning ("rot 0 = %g %g %g", mat[0], mat[1], mat[2]); 
qWarning ("rot 1 = %g %g %g", mat[3], mat[4], mat[5]); 
qWarning ("rot 2 = %g %g %g", mat[6], mat[7], mat[8]); 
//     Mat9d mat1 = Mat9d(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8]);

     do_atoms_rotation(mat, this->cent_);
     do_invis_rotation(mat, this->cent_);
     do_signes_rotation(mat, this->cent_);
     do_axis_rotation(mat);
//        emit SIG_prepareRotation();
  }
}

void Internal::compute_rotation_tensor()
{
//  double fraction = double(fr_up)/double(fr_down);
//  double a = this->actcrstr->a;
//  double c = this->actcrstr->c;

  double h = double(indMiller[0]);
  double k = double(indMiller[1]);
  double l = double(indMiller[2]);
  double u = double(indMiller[3]);
  double v = double(indMiller[4]);
  double w = double(indMiller[5]);
  glm::dvec3 s = this->actcrstr->C2O * glm::dvec3(h, k, l);
  glm::dvec3 m = glm::transpose(this->actcrstr->O2C) * glm::dvec3(u, v, w);
  s = glm::normalize(s);
  m = glm::normalize(m);
  glm::dvec3 mxs = glm::normalize(glm::cross(m, s));
  glm::dvec3 mxsxm = glm::normalize(glm::cross(mxs, m));
/*
  this->rot_tensor[0] = mxsxm.x;
  this->rot_tensor[1] = mxsxm.y;
  this->rot_tensor[2] = mxsxm.z;
  this->rot_tensor[3] = mxs.x;
  this->rot_tensor[4] = mxs.y;
  this->rot_tensor[5] = mxs.z;
  this->rot_tensor[6] = m.x;
  this->rot_tensor[7] = m.y;
  this->rot_tensor[8] = m.z;
*/
  this->rot_tensor = glm::dmat3(mxsxm.x, mxs.x, m.x,
                                mxsxm.y, mxs.y, m.y,
                                mxsxm.z, mxs.z, m.z);
}

/*
  double norm_s = 1./( 2.*sqrt( (h*h-h*k+k*k)*a*a + c*c*l*l ) );
  double norm_m = 1./sqrt( (4.*u*u+2.*u*v+4.*v*v)*c*c + 3.*w*w*a*a );
  double sq3 = sqrt(3.);
  QVector3D m = QVector3D(norm_m*sq3*u*c, norm_m*(u+2.*v)*c, norm_m*sq3*w*a);
//  double nrm = 1./m.norme();
  QVector3D s = QVector3D(norm_s*(2.*h-k)*a, norm_s*sq3*k*a, norm_s*2.*l*c);
  QVector3D mxs = QVector3D::crossProduct(m, s);
  QVector3D mxsxm = QVector3D::crossProduct(mxs, m);
  m = m.normalized();
  mxs = mxs.normalized();
  mxsxm = mxsxm.normalized();
 qWarning("normes = %g, %g, %g", m.length(), mxs.length(), mxsxm.length());
//  double norm_msm = 1.0/mxsxm.norme();
  this->rot_tensor[0] = mxsxm.x();
  this->rot_tensor[1] = mxsxm.y();
  this->rot_tensor[2] = mxsxm.z();
  this->rot_tensor[3] = mxs.x();
  this->rot_tensor[4] = mxs.y();
  this->rot_tensor[5] = mxs.z();
  this->rot_tensor[6] = m.x();
  this->rot_tensor[7] = m.y();
  this->rot_tensor[8] = m.z();
// przypadek dyslokacji śrubowej - bez obrotu
//  if ( h==0. && k==0. )
//              for (int i=0; i<9; i++) this->rot_tensor[i] = constant::unit_matrix[i];
}
*/
//------------------ M I L L E R ' S  I N T E R N A L S -------------------
//-------------------------------------------------------------------------

bool Internal::parse_miller(QString line)
{ 
  Int4 mil;
  QString line1, line_a;
  line1 = line.trimmed().simplified();
  this->act_mill = line1;
 qWarning("line1=%s\n", line1.toAscii().data());
   for (int i=0; i<6; i++) oldMiller[i] = indMiller[i];
//  qWarning("i_fraction : %d\n", i_fraction);
   int i_left1 = line1.indexOf("[");
   int i_right1 = line1.indexOf("]");
   int i_left2 = line1.indexOf("(");
   int i_right2 = line1.indexOf(")");
//  qWarning("left, right : %d %d, %d r%d\n", i_left1, i_right1, i_left2, i_right2);
   fraction = (i_left1==0)? 1. : read_fraction(line1.left(i_left1));
//  qWarning ("fraction=%g\n", fraction);
    if ( i_left1>=i_right1 || i_left2>=i_right2 ) {// error
       qWarning("ERROR PARENTHESES\n");
       return false;
    }
    QString line2 = line1.mid(i_left1+1, i_right1-i_left1-1).trimmed();
//    line2 = line2.trimmed();
    if ( internal_miller(line2, 1, mil) ) {
       indMiller[0] = mil.i1;
       indMiller[1] = mil.i2;
       indMiller[2] = mil.i3;
    } else {
       qWarning("ERROR - BAD MILLER INDICES 1");
       return false;
    }
    line2 = line1.mid(i_left2+1, i_right2-i_left2-1).trimmed();
    if ( internal_miller(line2, 2, mil) ) {
       indMiller[3] = mil.i1;
       indMiller[4] = mil.i2;
       indMiller[5] = mil.i3;
    } else {
       qWarning("ERROR - BAD MILLER INDICES 2");
       return false;
    }
//    VS->rotChanged = !eqMiller(indMiller, oldMiller);
    return true;
// =========================================================================    
//  } else { //sw!=1
}

bool Internal::parse_core(QString line)
{ 
  QString line1;
  line1 = line.trimmed().simplified();
  if ( line1==QString("none") ) return false;
//  this->act_mill = line1;
  act_core = "";
  int i_left3 = line1.indexOf("{");
  if ( i_left3<0 ) {
     int nd = -1;
     for ( int i=0; i<actcrstr->ncores; i++) {
        if ( line1==actcrstr->co_name[i] ) {
             nd = i;
             this->act_core = line1;
             break;
        }
     }

     if ( !(this->act_core.isEmpty()) ) 
       {
	 actdisl->dislocation_core = actcrstr->core[nd];
	 return true;
       } 
     else    
       goto LAB_other;
     
  } 
  else 
    {
    LAB_other:
      int i_right3 = line1.indexOf("}");
      this->act_core = line1.left(i_left3).trimmed();
      QStringList list= line1.mid(i_left3+1, i_right3-i_left3-1).
	trimmed().simplified().split(QRegExp("[,{}]"));
      double oth_disl1 = read_fraction(list.at(0));
      double oth_disl2 = read_fraction(list.at(1));
      double oth_disl3 = read_fraction(list.at( 2));
      qWarning("act_core=%s  other - %g %g %g", act_core.toAscii().data(), oth_disl1, oth_disl2, oth_disl3);
      //    qWarning("act_core=%s, ncores=%d", act_core.toAscii().data(), this->actcrstr->ncores);
      for ( int ind=0; ind<this->actcrstr->ncores; ind++) {
	if ( act_core==actcrstr->co_name[ind] ) {
	  qWarning ("ERROR- act_core existing for ind=%d (%s - %s)", ind, 
		    actcrstr->co_name[ind].toAscii().data(), act_core.toAscii().data());
	  return false;
	}
      } // for
      int nc = ++(actcrstr->ncores);
      actdisl->dislocation_core = actcrstr->core[nc] = QVector3D(oth_disl1, oth_disl2, oth_disl3);
      if ( act_core.isEmpty() ) actcrstr->co_name[nc].sprintf("core_nr_%d", nc);
      else                      actcrstr->co_name[nc] = act_core;
    }
  return true;
}

bool Internal::internal_miller(QString line2, int which, Int4 &mil)
{
   int numbmill;
   QStringList fields;

// qWarning("line2=%s\n", line2.toAscii().data() );
    if ( line2.contains(",") || line2.contains(" ") ) {
       fields = line2.split(QRegExp("[ ,]"), QString::SkipEmptyParts);
       numbmill = fields.size();
       if ( numbmill<3 || numbmill>4 ) {
          qWarning("ERROR - BAD NUMBER OF MILLER INDICES 1");
          return false;
       }
       for (int i=0; i<numbmill; i++) mil[i] = fields.at(i).toInt();
    } else {
       bool minus = false;
       numbmill = 0;
       int i=0;
       while ( i<line2.size() ) {
          QChar ch = QChar(line2.at(i++));
          if ( ch==QChar('-') ) {
             minus = true;
             continue;
          }
          if ( ch.isDigit() ) {
             int n = ch.digitValue();
             mil[numbmill++] = minus? -n : n; 
             minus = false;
          }
          if ( ch==QChar('.') && numbmill==2 ) mil[numbmill++] = -mil[0]-mil[1];
       }
       if ( numbmill<3 || numbmill>4 ) {
          qWarning("ERROR - BAD NUMBER OF MILLER INDICES 2");
          return false;
       }
   }
   if ( numbmill==4 ) {
      if ( (mil[0]+mil[1])!=-mil[2] ) {
          qWarning("ERROR BAD SUM - %d %d %d\n", mil[0], mil[1], mil[2]);
          mil[2] = -mil[0]-mil[1];
//          return false;
      }
      if ( which==1 ) {
         mil[0] -= mil[2];
         mil[1] -= mil[2];
      }
      mil[2] = mil[3];
   }
//    qWarning("miller: %d, %d, %d\n", mil[0], mil[1], mil[2]);
   return true;
}



/*
void Internal::load( const QString &fileName )
{
    QFile f( fileName );
//    if ( !f.open( IO_ReadOnly ) ) {
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("ERROR -- Mainwindow::load");
        return;
    }

	CMatrixMemoryManager::initialize( );

	try{
			CFEMAppInterface FP;
			FP.RunCalculations( fileName.toStdString() );
			FEMTest( FP );
	}
	catch( CFEException &e )
	{
//		int ret = QMessageBox::warning(this, QString("FEM EXCEPTION"), QString(e.GetMessage().c_str()));
      qWarning("CFEException");
	}
	catch( CMException &e )
	{
//		int ret = QMessageBox::warning(this, QString("MOLECULAR EXCEPTION"), QString(e.GetMessage().c_str()));
      qWarning("CMException");
	}
  qWarning("CALC  O.K.");
//    statusBar()->message( tr("Successfuly calculated  %1").arg(fileName), 2000 );
}


void Internal::FEMTest( CFEMAppInterface &FP )
{

    ofstream fout("FEMTestResults.txt");
    unsigned k,l;

//    fout << "Nodal Coordinates" << endl;

    const vector<string>& names = FP.GetResultFNNames();

//    fout << "x" << "\t" << "y" << "\t" << "z";

//    fout << endl << endl;

// Printing coordinates of nodes

    const vector< CFENode*> &nodesF = FP.GetNodes();

    for (k=0; k<nodesF.size(); k++)
    {
        fout << k << "\t";
        for (l=0; l<names.size(); l++)
            fout << nodesF[k]->GetCoord(l) << "\t";

        fout << endl;

    }

    
    fout << endl << endl;

    fout << "Nodal results" << endl;

// Printing result values    
    
    for (k=0; k<names.size(); k++)
        fout << names[k] << "\t";

    fout << endl << endl;

    
    for (k=0; k<nodesF.size(); k++)
    {
        fout << k << "\t";
        for (l=0; l<names.size(); l++)
            fout << nodesF[k]->GetFNValue(l) << "\t";

        fout << endl;

    }

    fout << endl << endl;
    
    fout << "Nodes of finite elements" << endl;

    const vector< CFiniteElement* > &elements = FP.GetElements();
    for (k=0; k<elements.size(); k++)
    {
        const vector< CFENode*> &elem_nodes = elements[k]->GetNodes();
        int n_el = elem_nodes.size();
        fout << "elem. nr. " << k << "\t";
        for (int n=0; n<n_el; n++) 
            fout << elem_nodes[n]->GetNumber() << "\t";
        fout << endl;
    }

    fout << endl << endl;

    fout.close();

    FEMCopy(FP);
}

void Internal::FEMCopy( CFEMAppInterface &FP )
{

    const vector<CFENode*> &nodesF = FP.GetNodes();

    const vector<CFiniteElement*> &elements = FP.GetElements();

    const vector<string> &res_names = FP.GetResultFNNames();

    INT_nn = nodesF.size();
    INT_ne = elements.size();
    INT_nr = res_names.size();
    ++INT_nr;
    this->nodes->n_nodes = INT_nn;
    this->elems->n_elements = INT_ne;
    this->results->n_results = INT_nr;
  qWarning("n_elements, n_nodes, n_results: %d %d %d", INT_ne, INT_nn, INT_nr);
//    const vector< vector<double> > resultsF = FP.GetResultFNs();
    this->results->ires = new int[INT_nn];
    this->results->res = new double*[INT_nn];
    this->results->r_min_ = new double[INT_nr];
    this->results->r_max_ = new double[INT_nr];
    this->nodes->coords = new QVector3D[INT_nn];
 
    for (int k=0; k<INT_nn; k++) {
       results->res[k] = new double[INT_nr];
       results->ires[k] = k;
       for (int l=0; l<INT_nr; l++)
          if ( l<3 ) this->results->res[k][l] = nodesF[k]->GetFNValue(l);
          else       this->results->res[k][l] = 100.0 + (k%5)*15.;
       for (int l=0; l<3; l++)
          this->nodes->coords[k][l] = nodesF[k]->GetCoord(l);

    }
//-----------------------------------------------------
    minmax3(this->nodes->coords, INT_nn, this->e_min_, this->e_max_);
  qWarning("e_min = ( %g, %g %g )",
       this->e_min_.x, this->e_min_.y, this->e_min_.z ); 
  qWarning("e_max = ( %g, %g %g )",
       this->e_max_.x, this->e_max_.y, this->e_max_.z ); 

    for (int l=0; l<INT_nr; l++) {
       double rmn = 1.e15;
       double rmx = -1.e15;
       for (int k=0; k<INT_nn; k++) {
          double v = results->res[k][l];
          if ( v<rmn ) rmn = v;
          if ( v>rmx ) rmx = v;
       }
       results->r_min_[l] = rmn;
       results->r_max_[l] = rmx;
 qWarning("l=%d, min=%g,  max=%g", l, results->r_min_[l], results->r_max_[l]);
    }

//    --INT_nr;

// qWarning("aaa - 0");
    this->elems->el_nodes = new int*[INT_ne];
    this->elems->n_nodes = new int[INT_ne];
    this->elems->type = new int[INT_ne];
// qWarning("aaa - 1");

    ofstream fout("QT_FEM_Results2.txt");
    fout << "  #############  second!!!" << endl;

    for (int i=0; i<INT_ne; i++) {
       this->elems->type[i] = 0;
       const vector< CFENode*> &elem_nodes = elements[i]->GetNodes();
       int n_el = elem_nodes.size();
       this->elems->n_nodes[i] = n_el;
       this->elems->el_nodes[i] = new int[n_el];
//       fout << "elem. nr. " << i << "\t";
       for (int j=0; j<n_el; j++) {
          this->elems->el_nodes[i][j] = elem_nodes[j]->GetNumber();
//          fout << elem_nodes[j]->GetNumber() << "\t";
       }
//        fout << endl;

    }
    this->calc_done = true;

    ofstream fout("FEM_QT.txt");
    fout << "Nodal results" << endl;

// Printing result values    
    
//    for (int k=0; k<INT_nn; k++)
//        fout << names[k] << "\t";

//    fout << endl << endl;

    
    for (int k=0; k<INT_nn; k++)
    {
        fout << k << "\t";
        for (int l=0; l<INT_nr; l++)
            fout << this->results->res[k][l] << "\t";

        fout << endl;

    }

    fout << endl << endl;
    
    fout << "Nodes of finite elements" << endl;

//    const vector< CFiniteElement* > &elements = FP.GetElements();
    for (int k=0; k<INT_ne; k++)
    {
        const vector< CFENode*> &elem_nodes = elements[k]->GetNodes();
        int n_el = elem_nodes.size();
        fout << "elem. nr. " << k << "\t";
        for (int n=0; n<n_el; n++) 
            fout << this->elems->el_nodes[i][j] << "\t";
//            elem_nodes[n]->GetNumber()
        fout << endl;
    }

    fout << endl << endl;

    fout.close();


// qWarning("aaa - 2");
//    fout << endl;
//    fout.close();
}


void processMiller(string result_text) // liczy tensory obrotu na podstawie wskaźników Millera
{ act_disl = result_text;
  mil = parse_miller(result_text);
  if ( mil.fraction!=0. ) {
    rot_tensor = rotation_tensor();         // nowy kolejny obrót
    rot_inv = glm::transpose(rot_tensor);}   //  jego odwrotność
}

glm::dmat3 rotation_tensor() // obliczanie tensora obrotu sztywnego na podstawie wskaźników Millera 
{ glm::dvec3 s = crysCell->c2o * glm::dvec3(mil.indices[0], mil.indices[1], mil.indices[2]);
  glm::dvec3 m = glm::transpose(crysCell->o2c) * glm::dvec3(mil.indices[3], mil.indices[4], mil.indices[5]);
  s = glm::normalize(s);
  m = glm::normalize(m);
  glm::dvec3 mxs = glm::normalize(glm::cross(m, s));
  glm::dvec3 mxsxm = glm::normalize(glm::cross(mxs, m));
  return glm::dmat3(mxsxm.x, mxs.x, m.x,
                    mxsxm.y, mxs.y, m.y,
                    mxsxm.z, mxs.z, m.z);
}

//========================================================================================================
struct params {
  double be;
  double bz;
  double u0x;
  double u0y;
  double u0z;
};

int Love_function(const gsl_vector *x, void *par, gsl_vector *result_funct) {
  double be = ((struct params *) par)->be;
  double bz = ((struct params *) par)->bz;
  double u0x = ((struct params *) par)->u0x;
  double u0y = ((struct params *) par)->u0y;
  double u0z = ((struct params *) par)->u0z;
  const double distx = gsl_vector_get(x, 0);
  const double disty = gsl_vector_get(x, 1);
  const double distz = gsl_vector_get(x, 2);
  double r2 = distx*distx+disty*disty;
  double r = sqrt(r2);
  double xx = distx/r;
  double yy = disty/r;
  double r02 = rad_fact*be*be; // radius of inmobile ring relative to which the atoms in the core move up
  const double ux =  u0x - be /(2.*pi)*(atan2(yy,xx)+xx*yy/(2.*(1.-nu)));// - 0.5*be;
  const double uy = u0y + be /(8.*pi*(1.-nu)) * ((1.-nu-nu)*log(r2/r02) + (xx+yy)*(xx-yy));
  const double uz = u0z - bz/(2.*pi)*atan2(yy, xx);
  gsl_vector_set(result_funct, 0, ux);
  gsl_vector_set(result_funct, 1, uy);
  gsl_vector_set(result_funct, 2, uz);
  return GSL_SUCCESS;
}

int Beta_function(const gsl_vector *x, void *par, gsl_matrix *jac) {
  double be = ((struct params *) par)->be;
  double bz = ((struct params *) par)->bz;
  const double xx = gsl_vector_get(x, 0);
  const double yy = gsl_vector_get(x, 1);
  const double zz = gsl_vector_get(x, 2);
  const double y2 = yy*yy;
  const double x2 = xx*xx;
  const double r2 = x2+y2;
  if ( r2<1.e-15 ) {
    cout << " Atom  in the center of dislocation core" << endl;
    gsl_matrix_set(jac, 0, 0, 1.);
    gsl_matrix_set(jac, 0, 1, 0.);
    gsl_matrix_set(jac, 0, 2, 0.);
    gsl_matrix_set(jac, 1, 0, 0.);
    gsl_matrix_set(jac, 1, 1, 1.);
    gsl_matrix_set(jac, 1, 2, 0.);
    gsl_matrix_set(jac, 2, 0, 0.);
    gsl_matrix_set(jac, 2, 1, 0.);
    gsl_matrix_set(jac, 2, 2, 1.);
  } else {
    const double a = be/(4. * pi * (1.-nu) * r2*r2);                  // a = bx/(4. * pi * (1.-n) * r2*r2)
    const double bxx = 1. + a * yy * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2); // u(4) = -a * y * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xx 
    const double byx = a * xx * ((1.-2.*nu)*x2 + (3.-2.*nu)*y2);      // u(5) = -a * x * ((1.-2.*n)*x*x + (3.-2.*n)*y*y) !yx
    const double bzx = bz/(2.*pi) * yy/r2;                            // u(6) = -bz/(2.*pi) * y/r2                       !zx
    const double bxy = -a * xx * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2);     // u(7) =  a * x * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xy
    const double byy = 1. - a * yy * ((1.+2.*nu)*x2 - (1.-2.*nu)*y2); // u(8) =  a * y * ((1.+2.*n)*x*x - (1.-2.*n)*y*y) !yy
    const double bzy = bz/(2.*pi) * xx/r2;                            // u(9) = -bz/(2.*pi) * x/r2                       !zy
    gsl_matrix_set(jac, 0, 0, bxx);
    gsl_matrix_set(jac, 0, 1, bxy);
    gsl_matrix_set(jac, 0, 2, 0.);
    gsl_matrix_set(jac, 1, 0, byx);
    gsl_matrix_set(jac, 1, 1, byy);
    gsl_matrix_set(jac, 1, 2, 0.);
    gsl_matrix_set(jac, 2, 0, bzx);
    gsl_matrix_set(jac, 2, 1, bzy);
    gsl_matrix_set(jac, 2, 2, 1.); }
  return GSL_SUCCESS;
}

int Love_fdf(const gsl_vector *x, void *par, gsl_vector *result_funct, gsl_matrix *jac) {
  Love_function(x, par, result_funct);
  Beta_function(x, par, jac);
  return GSL_SUCCESS;
}

void singledisl(int n_a) { // Liczy pole przemieszczeń od dyslokacji ze rdzeniem przechodzącym przez punk n_a  

  printMat("rotation tensor", rot_tensor);
  glm::dvec3 burgers_vector = rot_tensor * (mil.fraction * crysCell->c2o * glm::dvec3(mil.indices[0], mil.indices[1], mil.indices[2]));
//  cout << " mil.indices   " << mil.indices[0] << "    " << mil.indices[1] << "    " << mil.indices[2] << endl;
//  cout << " burgers_vector   " << burgers_vector.x << "    " << burgers_vector.y << "    " << burgers_vector.z << endl;
  glm::dvec3 cd = rot_tensor * atoms->coord[n_a];
  double be=sqrt(burgers_vector.x*burgers_vector.x+burgers_vector.y*burgers_vector.y);
  double bz = burgers_vector.z;
  cout << " be=" << be << "     bz=" << bz << endl;
  for (int i=0; i<atoms->n_atoms; i++) coord1[i] = rot_tensor * atoms->coord[i];

  if ( sw_iter ) {

    const gsl_multiroot_fdfsolver_type *T;
    T = gsl_multiroot_fdfsolver_hybridsj;
    gsl_multiroot_fdfsolver *s; // = gsl_multiroot_fdfsolver_alloc(T, 3);
    gsl_vector *x; // = gsl_vector_alloc(3);
    size_t count;
//  const size_t n = 3;
    struct params p;
    size_t status;
    glm::dvec3 diff;
    double crit = 0.;
//  stringstream sss;

    p.be = be;
    p.bz = bz;

/?
  sss << "C1-" << NumLine << ".xyz";
  extPrint(sss.str(), atoms->n_atoms, coord1);
  sss.str("");
  sss << "temp-" << NumLine;
  ofstream fout(sss.str().c_str());
?/
    for (int i=0; i<n_Atoms; i++) {
//    if ( i>=atoms->n_atoms-n_addAtoms ) continue;
      if ( i==n_a ) { cout<< "Error for n_a=" << n_a << "   i=" << i << endl;  continue; }
      count = 0;

      do {
         count++;
         p.u0x = atoms->du[i].x;
         p.u0y = atoms->du[i].y;
         p.u0z = atoms->du[i].z;
         glm::dvec3 temp = coord1[i]+atoms->du[i] - cd;
//       if ( count==1) fout << i+1 << "    " << temp.x << ", " << temp.y << ", " << temp.z << endl; 
         if ( (temp.x*temp.x+temp.y*temp.y)<1.e-10 ) {
           atoms->du[i] = glm::dvec3(0., 0., 0.);
           cout << " Atom " << i << " in the center of dislocation core" << endl;
           n_errors++;
           goto _END;
         }
         gsl_multiroot_function_fdf f = {&Love_function, &Beta_function, &Love_fdf, 3, &p};
         x = gsl_vector_alloc(3);
         gsl_vector_set(x, 0, temp.x);
         gsl_vector_set(x, 1, temp.y);
         gsl_vector_set(x, 2, temp.z);

         s = gsl_multiroot_fdfsolver_alloc(T, 3);
         gsl_multiroot_fdfsolver_set(s, &f, x);

         status = gsl_multiroot_fdfsolver_iterate(s);

         if ( status ) break;
         diff.x = gsl_vector_get(s->f, 0);
         diff.y = gsl_vector_get(s->f, 1);
         diff.z = gsl_vector_get(s->f, 2);
         atoms->du[i] -= diff;
         status = gsl_multiroot_test_residual(s->f, crit_stop);
      } while ( status == GSL_CONTINUE && count<countN_R );
      crit += diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
//    cout << " --- i = " << i << "     count = " << count << endl;

      gsl_multiroot_fdfsolver_free(s);
      gsl_vector_free(x);
//    gsl_vector_free(s->f);
      continue;
   _END:
      cout << "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << endl;
      n_Errors++;
    }
//  fout.close();
    cout << " --- n_a = " << n_a << "     crit = " << crit << endl;
/?
  sss.str("");
  sss << "U-" << NumLine;
  extPrint(sss.str(), atoms->n_atoms, atoms->du);
?/
  } else {  // !sw_iter
    for (int i=0; i<n_Atoms; i++) {
//    if ( i>=atoms->n_atoms-n_addAtoms ) continue;
      if ( i==n_a ) { cout<< "Error for n_a=" << n_a << "   i=" << i << endl;  continue; }
      glm::dvec3 dist1 = coord1[i] - cd;
      atoms->du[i] = mixed_u(i, dist1, be, bz);
    }
  }

  for (int i=0; i<n_Atoms; i++) //{
    atoms->du[i] = rot_inv*atoms->du[i];
//    atoms->beta[i] = rot_inv*mixed_beta(i, coord1[i]+atoms->du[i]-cd, be, bz)*rot_tensor;} //jeżeli jest potrzebne beta
}

glm::dvec3 mixed_u(int i, glm::dvec3 rotdist, double be, double bz)
{ double r2 = rotdist.x*rotdist.x + rotdist.y*rotdist.y;
  if ( r2<1e-15 ) {
     cout << " Atom " << i << " in the center of dislocation core" << endl;
     n_errors++;
//     return  glm::dvec3(0.25*be, 0., 0.);} 
     return  glm::dvec3(0., 0., 0.);} 
  else {
     double r = sqrt(r2);
     double x = rotdist.x/r;
     double y = rotdist.y/r;
     double r02 = rad_fact*be*be; // radius of inmobile ring relative to which the atoms in the core move up
     double ux =  be /(2.*pi)*(atan2(y,x)+x*y/(2.*(1.-nu)));// - 0.5*be;
     double uy = -be /(8.*pi*(1.-nu)) * ((1.-nu-nu)*log(r2/r02) + (x+y)*(x-y));
     return glm::dvec3(ux, uy, bz/(2.*pi)*atan2(y, x));} } // mixed_u

glm::dmat3 mixed_beta(int i, glm::dvec3 rotdist, double be, double bz)
{   glm::dmat3 b(0.,0.,0.,  0.,0.,0.,  0.,0.,0.);
    double x=rotdist.x;
    double x2=x*x;
    double y = rotdist.y;
    double y2=y*y;
    double r2=x2+y2;
    if ( r2<1.e-15 ) cout << " Atom " << i << " in the center of dislocation core" << endl;
    else {
       double a = be/(4. * pi * (1.-nu) * r2*r2);             // a = bx/(4. * pi * (1.-n) * r2*r2)
       double bxx = -a * y * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2); // u(4) = -a * y * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xx 
       double byx = -a * x * ((1.-2.*nu)*x2 + (3.-2.*nu)*y2); // u(5) = -a * x * ((1.-2.*n)*x*x + (3.-2.*n)*y*y) !yx
       double bzx = -bz/(2.*pi) * y/r2;                       // u(6) = -bz/(2.*pi) * y/r2                       !zx
       double bxy =  a * x * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2); // u(7) =  a * x * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xy
       double byy =  a * y * ((1.+2.*nu)*x2 - (1.-2.*nu)*y2); // u(8) =  a * y * ((1.+2.*n)*x*x - (1.-2.*n)*y*y) !yy
       double bzy = -bz/(2.*pi) * x/r2;                       // u(9) = -bz/(2.*pi) * x/r2                       !zy
//       if ( r2>=1.*(be*be+bz*bz) )                            // on the base of ~/pawel/papers/IMCE09/beta.mac
       b = glm::dmat3( bxx, bxy, 0.,   byx, byy, 0.,    bzx, bzy, 0. );
    }
    return b;
}

void Internal::mixed_u(int i)
{
  double angle_fi = 180.;
// -----------------------------------------------
  const double nu = 0.35;

  static double bx = actdisl->burgers_vector.x ();
  static double by = actdisl->burgers_vector.y ();
  static double bz = actdisl->burgers_vector.z ();

  double x = atoms->coord[i].x() - actdisl->cd.x();
  double y = atoms->coord[i].y() - actdisl->cd.y();
  double r2 = x*x + y*y;
  if ( r2<1e-33 ) {
     qWarning(
 " ERROR: Atom in the center of dislocation core!\ni=%d, coords = (%g, %g %g), r2=%g"
               , i, atoms->coord[i].x(), atoms->coord[i].y(), atoms->coord[i].z(), r2);
     atoms->u[i] = QVector3D(0., 0., 0.);
     return;
  }
  double be = sqrt(bx*bx + by*by);
  double fi0 = angle_fi / 180.;
  double fi = constant::pi * fi0;
  double additional = be * ((function::heavyside(fi-atan2(y,x))-0.5) - 0.5*fi0);
  double ux = 0.5*be*constant::inv_pi * (atan2(y, x) + 
                               x*y/((2.-nu-nu)*r2)) + additional;
  double uy = -be/(8.*constant::pi*(1.-nu)) * ((1.-nu-nu)*log(r2) + (x*x-y*y)/r2);

//---------------------------------------------------
//  u.x += c*ux - s*uy;
//  u.y += s*ux + c*uy;
  atoms->u[i] = QVector3D(ux, uy, bz/constant::two_pi * atan2(y, x));
} // mixed_u 
*/
