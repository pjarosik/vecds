// -------------------------------------------------------------------
//
// Author: Jan Cholewinski,Pawel Dluzewski and Toby D. Young.
//
// Copyright (C) 2010, 2012 The vecds authors
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

#include <QTranslator>
 
                                 // vecds gui includes
#include <vecds/gui/main_viewer.h>
#include <vecds/gui/main_window.h>

                                 // vecds includes
#include <vecds/internal.h>


Internal::Internal ()
  :
  path (VECDS_INTERNALS)
{
                                 // Create a bunch of internals:
  this->actcrstr = new vecds::CrystalStructure ();
  this->crstr    = new vecds::CrystalStructure[10];
  this->atoms    = new vecds::Atoms ();
  this->ap       = new vecds::AtomProperties ();
  this->actdisl  = new vecds::Dislocations ();
  this->disl     = new vecds::Dislocations[100];
  this->set0     = new vecds::Settings (); 


  
                                 // Then do something here...
  this->sliderMove   = false;
  this->img_loaded   = "none";
  this->atoms_loaded = "none";


                                 // First read in the settings.
  read_settings ();  

                                 // Then initialise atoms and
                                 // structures from babel and the
                                 // structure template respectively.
  init_atoms ();
  init_structures ();

  
  for (int i=0; i<8; i++) 
    this->visible[i] = set0->vis[i];

  this->mfactor = set0->mfact;
  
                                 // Make the rotation and it's inverse
                                 // a identity matirx.
  this->rot_tensor = this->rot_inv = glm::dmat3 (1., 0., 0.,
						 0., 1., 0.,
						 0., 0., 1.);
  
  this->Mode       = 0;
  this->choice     = false;
  this->ndisl      = 0;

#ifdef DEBUG
  qWarning ("class Internal (constructor): successfully initialized.");
#endif
}

Internal::~Internal ()
{}

void Internal::init_atoms ()
{
  int i = 0;

  QFile file (path + "/atoms.babel");

  if (!file.open(QIODevice::ReadOnly | 
		 QIODevice::Text)) 
    {
      qWarning ("atoms.babel not found");
      return;
    }
  
  QTextStream in(&file);

  while (!in.atEnd()) 
    {
      QString line = in.readLine();
      QStringList fields = line.split(' ', QString::SkipEmptyParts); 

      fields.removeFirst();

      this->ap->namea[i]      = fields.takeFirst();
      this->ap->a_rad1[i]     = fields.takeFirst().toFloat();
      this->ap->a_rad2[i]     = fields.takeFirst().toFloat();
      this->ap->atom_red[i]   = fields.takeFirst().toFloat();
      this->ap->atom_green[i] = fields.takeFirst().toFloat();
      this->ap->atom_blue[i]  = fields.takeFirst().toFloat();
      
      ++i;
    }

#ifdef DEBUG
  qWarning ("class Internal (init_atoms): atoms successfully initialized");
#endif
}

void Internal::init_structures()
{
  QString line;
  QStringList fields;
  int nf;


  QFile file (path + "/crystal_structure.template");
  
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
      qWarning("Error - no crystal structure definitions!");
      return;
    }
  
  QTextStream in(&file);
  line = in.readLine();
  fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts); 
  
  this->numbcrstr = fields.takeFirst ().toInt ();
  int nl = 1;
  int index = 0;
  double c2o0, c2o1, c2o2, c2o4, c2o5, c2o7, c2o8;
  while (!in.atEnd()) 
    {
      qWarning("+-------line nr %d", nl);
      line = in.readLine(); ++nl;
      fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
      nf = fields.size();

      if (nf!=7) 
	{
	  qWarning("Error - Atom_data 1 line=%d", nl);
	}

      actcrstr->structure_name = fields.takeFirst ();
      actcrstr->a              = fields.takeFirst ().toDouble ();
      actcrstr->b              = fields.takeFirst ().toDouble ();
      actcrstr->c              = fields.takeFirst ().toDouble ();
      actcrstr->alpha          = fields.takeFirst ().toDouble ();
      actcrstr->beta           = fields.takeFirst ().toDouble ();
      actcrstr->gamma          = fields.takeFirst ().toDouble ();

     line = in.readLine(); ++nl;
     fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
     nf = fields.size();
     if (nf!=2) 
       {
	 qWarning("Error - Atom_data 2 line=%d", nl);
       }

     actcrstr->n_materials = fields.takeFirst ().toInt ();
     actcrstr->n_cores     = fields.takeFirst ().toInt ();

     for (unsigned int i=0; i<actcrstr->n_materials; ++i) 
       {
	 line = in.readLine(); ++nl;
	 fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	 nf = fields.size();
	 if ( nf!=4 ) qWarning("Error - Atom_data 3 line=%d", nl);
	 actcrstr->crystal_type[i] = which_atom (fields.takeFirst ());
	 
	 actcrstr->cryst[i].x = read_fraction(fields.takeFirst().toStdString());
	 actcrstr->cryst[i].y = read_fraction(fields.takeFirst().toStdString());
	 actcrstr->cryst[i].z = read_fraction(fields.takeFirst().toStdString());
       }
     
    for (unsigned int i=0; i<actcrstr->n_cores; ++i) 
      {
	line = in.readLine(); ++nl;
	fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	nf = fields.size();
	if ( nf!=4 ) qWarning("Error - Atom_data 4 line=%d", nl);
	actcrstr->co_name[i] = fields.takeFirst();

	actcrstr->core[i].x = read_fraction(fields.takeFirst().toStdString());
	actcrstr->core[i].y = read_fraction(fields.takeFirst().toStdString());
	actcrstr->core[i].z = read_fraction(fields.takeFirst().toStdString());
      }
    
    c2o0 = actcrstr->a;

    double cosa6 = cos (vecds::constant::deg2rad*actcrstr->gamma);
    double sina6 = sin (vecds::constant::deg2rad*actcrstr->gamma);

    if ( actcrstr->gamma==90. ) 
      {
	cosa6 = 0.;
	sina6 = 1.;
      }

    c2o1 = actcrstr->b * cosa6; //1
    c2o4 = actcrstr->b * sina6;//4
    c2o7 = 0.;//7

    double cosa5 = cos (vecds::constant::deg2rad*actcrstr->beta);
    double cosa4 = cos (vecds::constant::deg2rad*actcrstr->alpha);

    if ( actcrstr->alpha==90. ) cosa4 = 0.;
    if ( actcrstr->beta==90. ) cosa5 = 0.;
    c2o2 = actcrstr->c * cosa5;//2
    c2o5 = actcrstr->c * (cosa4-cosa5*cosa6)/sina6;//5
    c2o8 = actcrstr->c * 
      sqrt(1.+2.*cosa4*cosa5*cosa6-cosa4*cosa4-cosa5*cosa5-cosa6*cosa6)/sina6;//8
    
    actcrstr->C2O = glm::dmat3(c2o0, 0, 0., 
                               c2o1, c2o4, c2o7,
                               c2o2, c2o5, c2o8);

    actcrstr->O2C = glm::inverse (actcrstr->C2O);

    crstr[index++] = *actcrstr;
    }

  if ( index!=numbcrstr ) qWarning("Error - Atom_data - number of lines");
  actcrstr = &crstr[0];

#ifdef DEBUG
  qWarning ("class Internal (init_structures): crystal structures successfully initialized.");  
#endif
  // qWarning("============== init structures O.K.");
}

// This function reads in the settings (for something) from a file
// called settings.set0.
void Internal::read_settings ()
{

  QFile file (path + "/settings.set0");

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
      qWarning("ERROR    file 'settings.set0' not found");
    }
  
  QTextStream in (&file);
  unsigned int count = 0;
  
  while (!in.atEnd()) 
    {
      QString line = in.readLine();
      if (line.startsWith ("#")) 
	continue;

      QStringList fields = line.split (QRegExp ("\\s+"), QString::SkipEmptyParts);
      int nf = fields.size ();
      
      switch (count)
	{

	case 0:

                                           // Process 0
	  this->set0->fov = fields.takeFirst ().toFloat ();
	  break;

	case 1: 

                                           // Process 1
	  this->set0->mfact = fields.takeFirst ().toDouble ();
	  break;

	case 3: 

                                           // Process 1
	  if (nf!=8) 
	    qWarning("Settings - line 3 - error!");
	  
	  for (int i=0; i<8; ++i) 
	    {
	      this->set0->vis[i] = (fields.takeFirst ().toInt ()==0)
		? 
		false : true;
	    }
	  break;

                                           // if counter > 2, process
                                           // the colour spectrum.
	default:

                                           // For some reason, the
                                           // size of colour_spectrum
                                           // is 12, therefore, we
                                           // should never be
                                           // accessing memory beyond
                                           // that point!
	  assert (count<12);
	  
	  this->set0->colour_spectrum[count-3] = { fields.takeFirst ().toInt (),
						   fields.takeFirst ().toInt (),
						   fields.takeFirst ().toInt () };

	}
      
                                           // increment the counter.
      ++count;
      
    }
  qWarning ("class Internal: \"%d\" settings were initialized.", count);
}

void Internal::write_vector (const std::string &str, 
			     const glm::dvec3  &V)
{
  std::cout << str << " = (" 
	    << std::setw (15) << V.x << ", "
	    << std::setw (15) << V.y << ", "
	    << std::setw (15) << V.z << ")" << std::endl;
}

void Internal::write_matrix (const std::string &str, 
			     const glm::dmat3  &M)
{
  std::cout << str << ": determinant = " << glm::determinant (M) 
	    << std::endl
	    << " (" << std::setw (15) << M[0][0] << ", " << std::setw(15) << M[1][0] << ", " << std::setw(15) << M[2][0] << ")" << std::endl
	    << " (" << std::setw (15) << M[0][1] << ", " << std::setw(15) << M[1][1] << ", " << std::setw(15) << M[2][1] << ")" << std::endl
	    << " (" << std::setw (15) << M[0][2] << ", " << std::setw(15) << M[1][2] << ", " << std::setw(15) << M[2][2] << ")" << std::endl;
}

void Internal::read_alc_xyz (QString aname)
{

    this->a_min_ = glm::dvec3(1.e15, 1.e15, 1.e15);
    this->a_max_ = glm::dvec3(-1.e15, -1.e15, -1.e15);

    QFile file(aname);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
      {
	qWarning("===== ERROR  file %s not found", aname.toAscii().data());
	return;
      }

    if ( this->atoms->n_atoms>0 ) 
      {
	delete [] this->atoms->atom_type;
	delete [] this->atoms->coordinates;
      }

    if ( this->atoms->n_bonds>0 ) 
      {
	delete [] this->atoms->atom_bond;
      }
    
    QTextStream in(&file);
    QString line = in.readLine();
    QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    this->atoms->n_atoms = fields.takeFirst ().toInt ();

    if ( this->atoms->n_atoms>0 ) 
      {
	this->atoms->atom_type       = new unsigned int[this->atoms->n_atoms];
	this->atoms->coordinates     = new glm::dvec3[this->atoms->n_atoms];
	this->atoms->u               = new glm::dvec3[this->atoms->n_atoms];
//	this->atoms->coordinates_glm = new glm::dvec3[this->atoms->n_atoms];
	this->atoms->du              = new glm::dvec3[this->atoms->n_atoms];
      }

    bool alc_ = aname.contains(".alc");

    if ( alc_ ) 
      {
	fields.removeFirst();
	this->atoms->n_bonds = fields.takeFirst ().toInt ();
	if ( this->atoms->n_bonds>0 ) 
	  {
	    this->atoms->atom_bond = new vecds::Int2[this->atoms->n_bonds];
	  }
      } 
    else 
      {  // !alc_
	this->atoms->n_bonds = 0;
       line = in.readLine();
      }

    qWarning("READ_ALC     ---   n_atoms = %d", this->atoms->n_atoms);
    for (unsigned int i=0; i<this->atoms->n_atoms; i++) {
      line = in.readLine();
      fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

      if ( alc_ ) fields.removeFirst();
      QString nam_a = fields.takeFirst();
      int ak = which_atom(nam_a);
      if ( ak==0 ) qWarning("unknown atom in line nr. %d in %s",
			    i+1, aname.toAscii().data());
      this->atoms->atom_type[i] = ak;
      this->atoms->coordinates[i].x = fields.takeFirst ().toDouble ();// *1.e10;
      this->atoms->coordinates[i].y = fields.takeFirst ().toDouble ();// *1.e10;
      this->atoms->coordinates[i].z = fields.takeFirst ().toDouble ();// *1.e10;
      this->atoms->u[i] = glm::dvec3(0., 0., 0.);
    }
    minmax3(this->atoms->coordinates, this->atoms->n_atoms, this->a_min_, this->a_max_);
    this->atoms_loaded = aname;
    if ( !alc_ ) return;

    for (unsigned int i=0; i<this->atoms->n_bonds; ++i) 
      {
	line = in.readLine();
	fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	fields.removeFirst();
	this->atoms->atom_bond[i].i1 = fields.takeFirst ().toInt ();
	this->atoms->atom_bond[i].i2 = fields.takeFirst ().toInt ();
      }
}




int Internal::which_atom(QString nam_a)
{
  for (int i=1; i<125; i++)
    if (nam_a.startsWith(this->ap->namea[i], Qt::CaseInsensitive)) 
      return i;
  return 0;
}

void Internal::minmax3(glm::dvec3 *vec, int numb, glm::dvec3 &vmin, glm::dvec3 &vmax)
{
  double mxx = -1.e15;  double mxy = -1.e15;  double mxz= -1.e15;
  double mnx = 1.e15;   double mny = 1.e15;   double mnz= 1.e15;

  for (int i=0; i<numb; i++) 
    {
      glm::dvec3 temp = vec[i];
      if ( temp.x>mxx ) mxx = temp.x;
      if ( temp.y>mxy ) mxy = temp.y;
      if ( temp.z>mxz ) mxz = temp.z;
      if ( temp.x<mnx ) mnx = temp.x;
      if ( temp.y<mny ) mny = temp.y;
      if ( temp.z<mnz ) mnz = temp.z;
    }
  vmax = glm::dvec3(mxx, mxy, mxz);
  vmin = glm::dvec3(mnx, mny, mnz);
}

void Internal::minmax1 (double *vec, int numb, double &vmin, double &vmax)
{
  double mxx = -1.e15;
  double mnx = 1.e15;

  for (int i=0; i<numb; ++i) 
    {
      double temp = vec[i];
      if ( temp>mxx ) mxx = temp;
      if ( temp<mnx ) mnx = temp;
    }
  vmax = mxx;
  vmin = mnx;
}

void Internal::do_axis_rotation (glm::dmat3 r_tens)
{
  this->axeX = r_tens * this->axeX;
  this->axeY = r_tens * this->axeY;
  this->axeZ = r_tens * this->axeZ;
}

void Internal::saveAtoms (QString sname)
{
  QString line;
  QFile fil(sname);

  if (!fil.open(QIODevice::WriteOnly)) 
    { 
      qWarning("Cannot open for writing: %s", qPrintable(fil.errorString()));
      return; 
    }

  QTextStream out(&fil);
  out << line.sprintf("%d\n ---\n", atoms->n_atoms);
  for (unsigned int i=0; i<atoms->n_atoms; i++)
    out << line.sprintf("%4s %12.7f %12.7f %12.7f\n",
			ap->namea[atoms->atom_type[i]].toAscii ().data (),
			atoms->coordinates[i].x, 
			atoms->coordinates[i].y, 
			atoms->coordinates[i].z);
}


void Internal::SL_singleDisl (glm::dvec3 rr)
{
  rr += this->cent_;
  actdisl->rrr = rr;// + this->cent_;
  actdisl->burgers_name = this->act_disl;
  actdisl->p1 = glm::dvec3 (rr.x, rr.y, this->a_min_.z);
  actdisl->p2 = glm::dvec3 (rr.x, rr.y, this->a_max_.z);
  qWarning("SL_singleDisl - p1, p2 - (%g %g %g) (%g %g %g)",
	   actdisl->p1.x, actdisl->p1.y, actdisl->p1.z, 
	   actdisl->p2.x, actdisl->p2.y, actdisl->p2.z);
  
  glm::dvec3 burg_vect = 
    rot_tensor * mil.fraction * actcrstr->C2O 
    * glm::dvec3(this->mil.indices[0], this->mil.indices[1], this->mil.indices[2]);
  
  actdisl->burgers_vector  = burg_vect;
  actdisl->rotation_tensor = this->rot_tensor;
  
  calc_disl0();
  
  disl[ndisl++] = *actdisl;
  
  qWarning("SINGLE DISL %d,  rrr= (%g %g %g)", ndisl-1, 
	   actdisl->rrr.x, actdisl->rrr.y,  actdisl->rrr.z);
}


void Internal::newdisl (unsigned int n_a, bool sw_iter)
{
  qWarning("newdisl");
  glm::dvec3 rr = this->atoms->coordinates[n_a];// + this->cent_;
  actdisl->rrr = rr;// + this->cent_;
  actdisl->burgers_name = this->act_disl;
  actdisl->p1 = glm::dvec3(rr.x, rr.y, this->a_min_.z);
  actdisl->p2 = glm::dvec3(rr.x, rr.y, this->a_max_.z);
  qWarning("SL_newDisl - p1, p2 - (%g %g %g) (%g %g %g)",
	   actdisl->p1.x, actdisl->p1.y, actdisl->p1.z, actdisl->p2.x, actdisl->p2.y, actdisl->p2.z);
  
//  glm::dvec3 burg_vect = rot_tensor * actcrstr->C2O * glm::dvec3(fraction*indMiller[0], fraction*indMiller[1], fraction*indMiller[2]);
  glm::dvec3 burg_vect = 
    rot_tensor * mil.fraction * actcrstr->C2O 
    * glm::dvec3(this->mil.indices[0], this->mil.indices[1], this->mil.indices[2]);

  glm::dvec3 cd        = rot_tensor * atoms->coordinates[n_a];
  
  be = sqrt (burg_vect.x*burg_vect.x+burg_vect.y*burg_vect.y);
  bz = burg_vect.z;

  for (unsigned int i=0; i<atoms->n_atoms; i++) 
    atoms->coordinates[i] = rot_tensor * atoms->coordinates[i];
  
  if ( sw_iter ) 
    {
      
      const gsl_multiroot_fdfsolver_type *T;
      T = gsl_multiroot_fdfsolver_hybridsj;
      gsl_multiroot_fdfsolver *s;
      gsl_vector *x;
      
      size_t count;
      
                                 // The return value of
                                 // gsl_multiroot_fdfsolver_iterate is
                                 // an integer, and so this needs to
                                 // be an integer too to make the two
                                 // comparable.
      int status = 0;
    
      glm::dvec3 diff;
      double crit           = 0.;
      double crit_stop      = 1.e-7;
      unsigned int countN_R = 10;
    
      p.be = be;
      p.bz = bz;
      
      for (unsigned int i=0; i<atoms->n_atoms; i++) 
	{
	  if (i==n_a)
	    {
	      qWarning ("Error for \"%d\"", n_a);
	      continue;
	    }

	  count = 0;
	  
	  do 
	    {
	      count++;
	      p.u0x = atoms->du[i].x;
	      p.u0y = atoms->du[i].y;
	      p.u0z = atoms->du[i].z;
	      glm::dvec3 temp = atoms->coordinates[i]+atoms->du[i] - cd;
	      
	      if ((temp.x*temp.x+temp.y*temp.y)<1.e-10) 
		{
		  atoms->du[i] = glm::dvec3(0., 0., 0.);
		  qWarning (" Atom \"%d\" in the center of dislocation core.", i);
		  
		  goto _END;
		}
	      
	      gsl_multiroot_function_fdf f = {&(::vecds::function::love), 
					      &(::vecds::function::beta), 
					      &(::vecds::function::love_fdf), 3, &p};
	      
	      x = gsl_vector_alloc (3);
	      
	      gsl_vector_set(x, 0, temp.x);
	      gsl_vector_set(x, 1, temp.y);
	      gsl_vector_set(x, 2, temp.z);
	      
	      s = gsl_multiroot_fdfsolver_alloc (T, 3);
	      
	      gsl_multiroot_fdfsolver_set (s, &f, x);
	      
	      status = gsl_multiroot_fdfsolver_iterate (s);
	      
	      if (status) 
		break;
	      
	      diff.x = gsl_vector_get(s->f, 0);
	      diff.y = gsl_vector_get(s->f, 1);
	      diff.z = gsl_vector_get(s->f, 2);
	      
	      atoms->du[i] -= diff;
	      
	      status = gsl_multiroot_test_residual(s->f, crit_stop);
	      
	    } while ((status == GSL_CONTINUE) && (count<countN_R));
	  
	  crit += diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
	  
	  gsl_multiroot_fdfsolver_free (s);
	  gsl_vector_free (x);
	  
	  continue;
	_END:

	  qWarning ();	  
	}
      
    } 
  else 
    {  // !sw_iter
      
      for (unsigned int i=0; i<atoms->n_atoms; i++) 
	{
	  if (i==n_a)
	    {
	      qWarning ("Error for \"%d\"", n_a);
	      continue;
	    }

	  glm::dvec3 dist1 = atoms->coordinates[i] - cd;
	  atoms->du[i] = mixed_u(i, dist1, be, bz);
	}
    }
  
  for (unsigned int i=0; i<atoms->n_atoms; i++) 
    {
      atoms->du[i] = rot_inv*atoms->du[i];
      atoms->u[i] += atoms->du[i];
    }
  
  qWarning ("uwaga!");
  disl[ndisl++] = *actdisl;

  qWarning ("NEW DISL %d,  rrr= (%g %g %g)", ndisl-1, 
	    actdisl->rrr.x, actdisl->rrr.y, actdisl->rrr.z);
  
}

// ------------------------------------------------

void Internal::addDisplacements ()
{
  for (unsigned int i=0; i<atoms->n_atoms; ++i) 
    {
      atoms->coordinates[i] += atoms->u[i];
      atoms->u[i] = glm::dvec3 (0., 0., 0.);
    }
}



void Internal::calc_disloc (int nr_atom, 
			    int disl_num)
{
  
  int i0 = atomize (actdisl->rrr, nr_atom);
  qWarning("SINGLE DISL  num=%d - i0=%d - coordinates=(%g %g %g)", disl_num, i0,
	   atoms->coordinates[i0].x, 
	   atoms->coordinates[i0].y, 
	   atoms->coordinates[i0].z);

  if (i0==-1) 
    qWarning("Error i0 for %d dislocation\n", ndisl);

  glm::dvec3 dislocation_core;
  
  dislocation_core.x = actdisl->dislocation_core.x*actcrstr->C2O[0][0] + 
			actdisl->dislocation_core.y*actcrstr->C2O[0][1];
  
  dislocation_core.y = actdisl->dislocation_core.x*actcrstr->C2O[1][0] + 
			actdisl->dislocation_core.y*actcrstr->C2O[1][1];
  
  dislocation_core.z = actdisl->dislocation_core.x*actcrstr->C2O[2][0] + 
			actdisl->dislocation_core.y*actcrstr->C2O[2][1];

//  vecds::Mat9d rt = vecds::Mat9d (this->rot_tensor);
  actdisl->cd = atoms->coordinates[i0] + this->rot_tensor * dislocation_core;
  
  actdisl->i0 = i0;
  qWarning("SINGLE DISL corrected coordinates = (%g %g %g),  i0=%d", 
	   actdisl->cd.x, actdisl->cd.y, actdisl->cd.z, i0);

  for (unsigned int i=0; i<atoms->n_atoms; ++i) 
    {
      atoms->coordinates[i] = rot_tensor * atoms->coordinates[i];
      glm::dvec3 dist1 = atoms->coordinates[i] - actdisl->cd;
      atoms->du[i] = mixed_u(i, dist1, p.be, p.bz);
    }
  
  for (unsigned int i=0; i<atoms->n_atoms; i++) 
    atoms->du[i] = rot_inv*atoms->du[i];
  
  qWarning("uwaga!");
  disl[ndisl++] = *actdisl;

  qWarning("NEW DISL %d,  rrr= (%g %g %g)", ndisl-1, 
	   actdisl->rrr.x, actdisl->rrr.y, actdisl->rrr.z);
  
  qWarning("Mixed_u end");
}

//---------------------------------------------------------------------

glm::dvec3 Internal::mixed_u(int i, glm::dvec3 rotdist, double be, double bz)
{
  double nu = 0.35;
  double rad_fact = 1.0;
  double r2 = rotdist.x*rotdist.x + rotdist.y*rotdist.y;
  if ( r2<1e-15 ) 
    {
      qWarning (" Atom \"%d\" is in the center of dislocation core", i);

      return  glm::dvec3(0., 0., 0.);} 
  else 
    {
      double r = sqrt(r2);
      double x = rotdist.x/r;
      double y = rotdist.y/r;
      double r02 = rad_fact*be*be; // radius of inmobile ring relative to which the atoms in the core move up
      double ux =  be /(2.*vecds::constant::pi)*(atan2(y,x)+x*y/(2.*(1.-nu)));// - 0.5*be;
      double uy = -be /(8.*vecds::constant::pi*(1.-nu)) * ((1.-nu-nu)*log(r2/r02) + (x+y)*(x-y));
      return glm::dvec3(ux, uy, bz/(2.*vecds::constant::pi)*atan2(y, x));
    } 
} // mixed_u

glm::dmat3 Internal::mixed_beta (int i, glm::dvec3 rotdist, double be, double bz)
{   

  glm::dmat3 b(0., 0., 0.,  
	       0., 0., 0.,  
	       0., 0., 0.);

  double nu = 0.35;
  double x  = rotdist.x;
  double x2 = x*x;
  double y  = rotdist.y;
  double y2 = y*y;
  double r2 = x2+y2;

  if (r2<1.e-15) 
    {
      qWarning (" Atom \"%d\" is in the center of dislocation core", i);
    }
  else 
    {
      double a   = be/(4. * vecds::constant::pi * (1.-nu) * r2*r2);   // a = bx/(4. * pi * (1.-n) * r2*r2)
      double bxx = -a * y * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2);          // u(4) = -a * y * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xx 
      double byx = -a * x * ((1.-2.*nu)*x2 + (3.-2.*nu)*y2);          // u(5) = -a * x * ((1.-2.*n)*x*x + (3.-2.*n)*y*y) !yx
      double bzx = -bz/(2.*vecds::constant::pi) * y/r2;               // u(6) = -bz/(2.*pi) * y/r2                       !zx
      double bxy =  a * x * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2);          // u(7) =  a * x * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xy
      double byy =  a * y * ((1.+2.*nu)*x2 - (1.-2.*nu)*y2);          // u(8) =  a * y * ((1.+2.*n)*x*x - (1.-2.*n)*y*y) !yy
      double bzy = -bz/(2.*vecds::constant::pi) * x/r2;               // u(9) = -bz/(2.*pi) * x/r2                       !zy

      b = glm::dmat3 (bxx, bxy, 0.,   
		      byx, byy, 0.,    
		      bzx, bzy, 0. );
    }

  return b;
}



int Internal::atomize (const glm::dvec3    point, 
		       const unsigned int atom_number)
{
  int i0          = -1;
  double infinity = 1.e33;
  
  for (unsigned int i=0; i<atoms->n_atoms; ++i) 
    {
      double distance = (point-atoms->coordinates[i]).length ();
      
      if ((atoms->atom_type[i]==atom_number) && (distance<infinity)) 
	{
	  infinity = distance;
	  i0       = i;
	}
    }

  return i0;
}

void Internal::calc_disl0 ()
{
  
  int    i0       = 0;
  double infinity = 1.e33;
  bool   ok       = false;

  for (unsigned int i=0; i<atoms->n_atoms; ++i) 
    {
      double distance = (actdisl->rrr-atoms->coordinates[i]).length ();

      if (distance<infinity) 
	{
	  infinity = distance;
	  i0       = i;
	  ok       = true;
	}
    }

  if (!ok) 
    qWarning("Error i0 for %d dislocation\n", ndisl);
  else
    qWarning("SINGLE DISL  - i0=%d - coordinates=(%g %g %g)", i0,
	     atoms->coordinates[i0].x, 
	     atoms->coordinates[i0].y, 
	     atoms->coordinates[i0].z);
  
  actdisl->cd = atoms->coordinates[i0];
  actdisl->i0 = i0;
  qWarning("SINGLE DISL corrected coordinates = (%g %g %g),  i0=%d", 
	   actdisl->cd.x, actdisl->cd.y, actdisl->cd.z, i0);

  for (unsigned int i=0; i<atoms->n_atoms; ++i) 
    { 
      atoms->coordinates[i] = rot_tensor * atoms->coordinates[i];
      glm::dvec3 dist1          = atoms->coordinates[i] - actdisl->cd;
      atoms->du[i]              = mixed_u (i, dist1, be, bz);
    }
  
  for (unsigned int i=0; i<atoms->n_atoms; ++i) 
    atoms->du[i] = rot_inv*atoms->du[i];

}


// --------------------------------------------------------------------

void Internal::do_atoms_rotation (glm::dmat3 r_tens, glm::dvec3 vec)
{
  for (unsigned int i=0; i<this->atoms->n_atoms; ++i)
    this->atoms->coordinates[i] = (r_tens * (this->atoms->coordinates[i] - vec)) + vec;

  minmax3(atoms->coordinates, atoms->n_atoms, a_min_, a_max_);
}


void Internal::do_invis_rotation (glm::dmat3 r_tens, glm::dvec3 vec)
{
  for (int i=0; i<8; i++)
    this->invbox[i] = (r_tens * (this->invbox[i] - vec)) + vec;
}

void Internal::do_signes_rotation (glm::dmat3 r_tens, glm::dvec3 vec)
{
  for (int j=0; j<this->ndisl; j++) 
    {
      disl[j].p1 = (r_tens * (disl[j].p1 - vec)) + vec;
      disl[j].p2 = (r_tens * (disl[j].p2 - vec)) + vec;
    }
}


void Internal::read_img(QString iname)
{
  this->img.load(iname);
  if ( this->img.isNull() ) 
    {
      qWarning("Unable to load file, unsupported file format");
      return;
    }
}

int Internal::lattice (int nx, 
		       int ny, 
		       int nz)
{
  int m = 0;

  for (int k=0; k<nz; k++) 
    for (int j=0; j<ny; j++) 
      for (int i=0; i<nx; i++) 
	{
	  for (unsigned int an=0; an<this->actcrstr->n_materials; an++) 
	    {
	      
	      glm::dvec3 hic = glm::dvec3 (double (i)+this->actcrstr->cryst[an].x,
					   double (j)+this->actcrstr->cryst[an].y,
					   double (k)+this->actcrstr->cryst[an].z);
	      
	      this->atoms->coordinates[m] = (actcrstr->C2O * hic);
	      this->atoms->atom_type[m++] = this->actcrstr->crystal_type[an];
	    }
	}

  return m;
}

int Internal::lattice2 (double       sx, 
			double       sy, 
			unsigned int nz)
{
  unsigned int m = 0;
  double sg      = sin (vecds::constant::deg2rad*this->actcrstr->gamma);

  unsigned int nx = static_cast<unsigned int>((sx+2.*sy*sqrt(1.-sg*sg))/this->actcrstr->a)+1;
  unsigned int ny = static_cast<unsigned int>(sy/(this->actcrstr->b*sg))+1;

  for (unsigned int k=0; k<nz; ++k) 
    for (unsigned int j=0; j<ny; ++j) 
      for (unsigned int i=0; i<nx; ++i) 
	{
	  glm::dvec3 hic = actcrstr->C2O * glm::dvec3 (double (i), double (j), double (k));
	  
	  if ((hic.x>=0.) && (hic.x<sx) && (hic.y>=0.) && (hic.y<sy)) 
	    {
	      for (unsigned int an=0; an<this->actcrstr->n_materials; an++) 
		{
		  glm::dvec3 ccc = this->actcrstr->cryst[an];
		  
		  this->atoms->coordinates[m] = (actcrstr->C2O*ccc + hic);
		  this->atoms->atom_type[m++] = this->actcrstr->crystal_type[an];
		}
	    }
	}
  return m;
}

void Internal::processMiller(int sw, 
			     QString result_text, 
			     QString result_text2)
{
  glm::dmat3 mat;
  glm::dmat3 rot_in;
  
  this->mil = parse_miller(result_text.toStdString());
  if (sw!=1) 
    parse_core (result_text2);

  rot_in = this->rot_inv;
  compute_rotation_tensor ();
  
  this->rot_inv = glm::transpose (glm::inverse (this->rot_tensor));
  mat = this->rot_tensor * rot_in;
  
  do_atoms_rotation (mat, this->cent_);
  do_invis_rotation (mat, this->cent_);
  do_signes_rotation (mat, this->cent_);
  do_axis_rotation (mat);
}

void Internal::compute_rotation_tensor()
{
  glm::dvec3 s = this->actcrstr->C2O * glm::dvec3 (mil.indices[0], mil.indices[1], mil.indices[2]);
  glm::dvec3 m = glm::transpose (this->actcrstr->O2C) * glm::dvec3 (mil.indices[3], mil.indices[4], mil.indices[5]);

  s = glm::normalize(s);
  m = glm::normalize(m);

  glm::dvec3 mxs   = glm::normalize (glm::cross (m, s));
  glm::dvec3 mxsxm = glm::normalize (glm::cross (mxs, m));

  this->rot_tensor = glm::dmat3(mxsxm.x, mxs.x, m.x,
                                mxsxm.y, mxs.y, m.y,
                                mxsxm.z, mxs.z, m.z);
}


//------------------ M I L L E R ' S  I N T E R N A L S -------------------

bool Internal::parse_core (QString line)
{ 
  QString line1;
  line1 = line.trimmed().simplified();
  if ( line1==QString("none") ) return false;
  
  act_core = "";
  int i_left3 = line1.indexOf("{");
  if ( i_left3<0 ) 
    {
      int nd = -1;

      for (unsigned int i=0; i<actcrstr->n_cores; ++i) 
	{
	  if (line1==actcrstr->co_name[i]) 
	    {
	      nd = i;
	      this->act_core = line1;
	      break;
	    }
	}
      
      if (!(this->act_core.isEmpty())) 
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
      QStringList list = line1.mid(i_left3+1, i_right3-i_left3-1).trimmed().simplified().split(QRegExp("[,{}]"));
      double oth_disl1 = read_fraction(list.at(0).toStdString());
      double oth_disl2 = read_fraction(list.at(1).toStdString());
      double oth_disl3 = read_fraction(list.at(2).toStdString());
      qWarning("act_core=%s  other - %g %g %g", act_core.toAscii().data(), oth_disl1, oth_disl2, oth_disl3);

      for (unsigned int ind=0; ind<this->actcrstr->n_cores; ++ind) 
	{
	  if ( act_core==actcrstr->co_name[ind] ) 
	    {
	      qWarning ("ERROR- act_core existing for ind=%d (%s - %s)", ind, 
			actcrstr->co_name[ind].toAscii().data(), act_core.toAscii().data());
	      return false;
	    }
	} // for

      int nc = ++(actcrstr->n_cores);
      actdisl->dislocation_core = actcrstr->core[nc] = glm::dvec3(oth_disl1, oth_disl2, oth_disl3);
      if (act_core.isEmpty()) 
	actcrstr->co_name[nc].sprintf("core_nr_%d", nc);
      else                      
	actcrstr->co_name[nc] = act_core;
    }
  return true;
}


bool Internal::internal_miller(std::string  line2, 
			       int          which, 
			       int         *miller_indices)
{
  unsigned int n_miller_indices = 0;

  if (line2.find_first_of (" ,")!=std::string::npos) 
    {
      std::vector<std::string> fields = tokenize (line2, " ,");
      n_miller_indices                = fields.size ();
      
      if (n_miller_indices<3 || n_miller_indices>4) 
	{
	  std::cout << "ERROR - BAD NUMBER OF MILLER INDICES 1\n";
	  return false;
	}
      
      for (unsigned int i=0; i<n_miller_indices; ++i) 
	{
	  miller_indices[i] = toInt (fields.at (i));
	}

    } 
  else 
    {
      bool minus       = false;
      n_miller_indices = 0;
      size_t i         = 0;
      while (i<line2.size ()) 
	{
	  char ch = line2.at(i++);
	  if ( ch=='-' ) 
	    {
	      minus = true;
	      continue;
	    }
	  if (isdigit (ch)) 
	    {
	      int n = int(ch)-int('0');
	      miller_indices[++n_miller_indices] = minus ? -n : n; 
	      minus = false;
	    }
	  if ( ch=='.' && n_miller_indices==2 ) miller_indices[n_miller_indices++] = -miller_indices[0]-miller_indices[1];
	}
      if (n_miller_indices<3 || n_miller_indices>4) 
	{
	  std::cout << "ERROR - BAD NUMBER OF MILLER INDICES 2\n";
	  return false;
	}
    }
  if (n_miller_indices==4) 
    {
      if ((miller_indices[0]+miller_indices[1])!=-miller_indices[2])
	{
	  std::cout << "ERROR BAD SUM - " << miller_indices[0] << "  " << miller_indices[1] << "  " << miller_indices[2] << std::endl;
	}
      
      if (which==1) 
	{
	  miller_indices[0] -= miller_indices[2];
	  miller_indices[1] -= miller_indices[2];
	}

      miller_indices[2] = miller_indices[3];
    }

  return true;
}


vecds::miller Internal::parse_miller (std::string line)
{
  vecds::miller result;
  result.fraction = 0;

  int mill[4];
  int indMiller[6];
  double fract;
  std::string line1;
  line1 = stripBlanks(line);

  size_t i_left1  = line1.find('[');
  size_t i_right1 = line1.find(']');
  size_t i_left2  = line1.find('(');
  size_t i_right2 = line1.find(')');

  fract = (i_left1==std::string::npos)? 1. : read_fraction(line1.substr(0, i_left1));

  if ( i_left1>=i_right1 || i_left2>=i_right2 ) 
    {
      std::cout << "ERROR PARENTHESES\n";
      return result;
    }
  std::string line2 = stripBlanks(line1.substr(i_left1+1, i_right1-i_left1-1));

  if (internal_miller (line2, 1, mill)) 
    {
      indMiller[0] = mill[0];
      indMiller[1] = mill[1];
      indMiller[2] = mill[2];
    } 
  else 
    { 
      std::cout << "ERROR - BAD MILLER INDICES 1\n";
      return result;
    }
  line2 = stripBlanks (line1.substr (i_left2+1, i_right2-i_left2-1));

  if (internal_miller (line2, 2, mill)) 
    {
      indMiller[3] = mill[0];
      indMiller[4] = mill[1];
      indMiller[5] = mill[2];
    } 
  else 
    {
      std::cout << "ERROR - BAD MILLER INDICES 2\n";
      return result;
    }

  result.fraction   = fract;
  result.indices[0] = double (indMiller[0]);
  result.indices[1] = double (indMiller[1]);
  result.indices[2] = double (indMiller[2]);
  result.indices[3] = double (indMiller[3]);
  result.indices[4] = double (indMiller[4]);
  result.indices[5] = double (indMiller[5]);

  return result;
}

std::string Internal::stripBlanks (std::string StringToModify)
{
   if (StringToModify.empty ()) 
     return "";

   size_t startIndex      = StringToModify.find_first_not_of (" ");
   size_t endIndex        = StringToModify.find_last_not_of (" ");
   std::string tempString = StringToModify;

   return tempString.substr (startIndex, endIndex-startIndex+1);
}

                                 // czyta ułamek np. w zapisie wektora
                                 // Burgersa, przelicza na liczbę
                                 // dziesiętną
double Internal::read_fraction (std::string line) 
{
  line = stripBlanks (line);
  size_t i_fr = line.find ('/');

  std::istringstream ins;
  ins.clear ();
  ins.str (line);

  double result, temp;
  char c;

  if (i_fr==std::string::npos) 
    ins >> result;
  else 
    {
      ins >> result >> c >> temp; 
      result /= temp;
    }

  return result;
}



int Internal::identify (const std::string s1, 
			const int         size, 
			const std::string words[])
{
  for (int i=0; i<size; ++i) 
    if (s1.compare (words[i])==0) 
      return i;

  return -1;
}

std::vector<std::string> Internal::tokenize (const std::string& str, 
					std::string        del)
{
  std::vector<std::string> tokens;
  std::string delimiters = del;
  size_t lastPos         = str.find_first_not_of (delimiters, 0);         // Skip delimiters at beginning.
  size_t pos             = str.find_first_of (delimiters, lastPos);       // Find first "non-delimiter".
  tokens.clear ();

  while (pos!=std::string::npos || lastPos!=std::string::npos) 
    {
      tokens.push_back (str.substr(lastPos, pos-lastPos));            // Found a token, add it to the vector.
      lastPos = str.find_first_not_of (delimiters, pos);              // Skip delimiters. Note the "not_of"
      pos = str.find_first_of (delimiters, lastPos);                  // Find next "non-delimiter"
    }
  return tokens;
}

int Internal::toInt (std::string word)
{
  int i;
  std::istringstream ins;
  ins.clear ();
  ins.str (word);
  ins >> i;
  return i;
}

