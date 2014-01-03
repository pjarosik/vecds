
// -------------------------------------------------------------------
//
// Copyright (C) 2010 The vecds authors
//
// This  program is  free  software: you  can  redistribute it  and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software  Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along      with      this      program.       If      not,      see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------

#include <QTranslator>
 
                                 // vecds gui includes
#include <vecds/gui/main_viewer.h>
#include <vecds/gui/main_window.h>

                                 // vecds includes
#include <vecds/base/internal.h>
#include <vecds/numerics/function.h>


Internal::Internal ()

{
  // Create a bunch of internals:
  gsl_vector *proba;
  glm::dvec3 proba1;
  this->ap       = new AtomsProperties ();
  this->actcrcell = new CrystalStructure ();
  this->crstr    = new CrystalStructure[10];
  this->set0     = new Settings (); 
  this->atoms    = new Atoms ();
  this->nodes    = new Nodes ();
  this->elems    = new Elements ();
  this->results  = new Results ();
  this->actdisl  = new Dislocations ();
  this->disl     = new Dislocations[100];
  
  // Then do something here...
  this->sliderMove   = false;
  this->res_loaded   = "none";
  this->img_loaded   = "none";
  this->fems_loaded  = "none";
  this->atoms_loaded = "none";
  this->showU = false;

  INT_nn = nodes->n_nodes = INT_ne = elems->n_elements = 0;

  this->current_dir = QDir::currentPath();
  this->current_dir.replace(QString("/bin"), QString(""));
//  this->outputFile = fopen("POLE-1a", "w");
  init_atoms();
  init_structures();
  read_settings();

  for (int i=0; i<8; i++) 
    this->visible[i] = set0->vis[i];
  this->mfactor = set0->mfact;

  // Initialize the e0 eleemnt.
  init_e0();
  
  // Finally reset out a bunch of things:
  this->rot_tensor     = constant::unit_matrix;
  this->rot_inv        = constant::unit_matrix;
//  this->mat            = constant::unit_matrix;
  this->Mode           = 0;
  this->choice         = false;
  this->n_dislocations = 0;
}

Internal::~Internal ()
{}

void Internal::init_atoms ()
{
  int i = 0;

  QString cd0 = this->current_dir;
// std::cout << "cd0=" << cd0.toStdString() << std::endl;
  QFile file (cd0.append("/share/vecds/internals/atoms.babel"));
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
  QFile file(cd0.append("/share/vecds/internals/structures.definitions"));

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
  while (!in.atEnd()) 
    {
      qWarning("+-------line nr %d", nl);
      line = in.readLine(); ++nl;
      fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
      nf = fields.size();
      if ( nf!=7 ) qWarning("Error - Atom_data 1 line=%d", nl);
      actcrcell->struct_name = fields.takeFirst();
      actcrcell->a = fields.takeFirst().toDouble();
      actcrcell->b = fields.takeFirst().toDouble();
      actcrcell->c = fields.takeFirst().toDouble();
      actcrcell->alpha = fields.takeFirst().toDouble();
      actcrcell->beta  = fields.takeFirst().toDouble();
      actcrcell->gamma = fields.takeFirst().toDouble();
      //    qWarning("a=%g, b=%g, c=%g"
      line = in.readLine(); ++nl;
      fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
      nf = fields.size();
      if ( nf!=2 ) qWarning("Error - Atom_data 2 line=%d", nl);
      actcrcell->nchem = fields.takeFirst().toInt();
      actcrcell->ncores = fields.takeFirst().toInt();
      if ( actcrcell->nchem<=0 || actcrcell->ncores<=0 ) 
        qWarning("nchem ncores");

      for (int i=0; i<actcrcell->nchem; i++ ) 
	{
	  line = in.readLine(); ++nl;
	  fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	  nf = fields.size();
	  if ( nf!=4 ) qWarning("Error - Atom_data 3 line=%d", nl);
	  actcrcell->cr_kind[i] = which_atom(fields.takeFirst());
	  //     qWarning("***  ***   i=%d, kind=%d", i, actcrcell->cr_kind[i]);
	  actcrcell->cryst[i].setX(read_fraction(fields.takeFirst()));
	  actcrcell->cryst[i].setY(read_fraction(fields.takeFirst()));
	  actcrcell->cryst[i].setZ(read_fraction(fields.takeFirst()));
	  //     qWarning("%d - (%g %g %g)", actcrcell->cr_kind[i],
	  //      actcrcell->cryst[i].x,
	  //      actcrcell->cryst[i].y,
	  //      actcrcell->cryst[i].z);
	}
      
      for (int i=0; i<actcrcell->ncores; i++ ) 
	{
	  line = in.readLine(); ++nl;
	  fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	  nf = fields.size();
	  if ( nf!=4 ) qWarning("Error - Atom_data 4 line=%d", nl);
	  actcrcell->co_name[i] = fields.takeFirst();
	  actcrcell->core[i].setX(read_fraction(fields.takeFirst()));
	  actcrcell->core[i].setY(read_fraction(fields.takeFirst()));
	  actcrcell->core[i].setZ(read_fraction(fields.takeFirst()));
	}
      
      actcrcell->c2o[0] = actcrcell->a; //0
      actcrcell->c2o[3] = 0.;   //3
      actcrcell->c2o[6] = 0.;   //6
      double cosa6 = cos(constant::deg2rad*actcrcell->gamma);
      double sina6 = sin(constant::deg2rad*actcrcell->gamma);
      if ( actcrcell->gamma==90. ) 
	{
	  cosa6 = 0.;
	  sina6 = 1.;
	}
      actcrcell->c2o[1] = actcrcell->b * cosa6; //1
      actcrcell->c2o[4] = actcrcell->b * sina6;//4
      actcrcell->c2o[7] = 0.;//7
      double cosa5 = cos(constant::deg2rad*actcrcell->beta);
      double cosa4 = cos(constant::deg2rad*actcrcell->alpha);
      if ( actcrcell->alpha==90. ) cosa4 = 0.;
      if ( actcrcell->beta==90. ) cosa5 = 0.;
      actcrcell->c2o[2] = actcrcell->c * cosa5;//2
      actcrcell->c2o[5] = actcrcell->c * (cosa4-cosa5*cosa6)/sina6;//5
      actcrcell->c2o[8] = actcrcell->c * 
	sqrt(1.+2.*cosa4*cosa5*cosa6-cosa4*cosa4-cosa5*cosa5-cosa6*cosa6)/sina6;//8
      
      actcrcell->o2c = invert(actcrcell->c2o);
      /*
	qWarning(":::::::::::::::::: matrix c2o:");
	qWarning("%g %g %g", 
	actcrcell->c2o[0], actcrcell->c2o[1], actcrcell->c2o[2]);
	qWarning("%g %g %g", 
	actcrcell->c2o[3], actcrcell->c2o[4], actcrcell->c2o[5]);
	qWarning("%g %g %g", 
	actcrcell->c2o[6], actcrcell->c2o[7], actcrcell->c2o[8]);
	qWarning("::::::::::::::::::  matrix o2c:");
	qWarning("%g %g %g", 
	actcrcell->o2c[0], actcrcell->o2c[1], actcrcell->o2c[2]);
	qWarning("%g %g %g", 
	actcrcell->o2c[3], actcrcell->o2c[4], actcrcell->o2c[5]);
	qWarning("%g %g %g", 
	actcrcell->o2c[6], actcrcell->o2c[7], actcrcell->o2c[8]);
      */
      crstr[index++] = *actcrcell;
      //    ++index;
      /*-------------
	qWarning(":::::::::::::::::: matrix test:");
	Mat9d test = matmult(actcrcell->c2o, actcrcell->o2c);
	qWarning("%g %g %g", 
	test[0], test[1], test[2]);
	qWarning("%g %g %g", 
	test[3], test[4], test[5]);
	qWarning("%g %g %g", 
	test[6], test[7], test[8]);
      */
    }
  if (index!=numbcrstr) 
    qWarning("Error - Atom_data - number of lines");

  actcrcell = &crstr[0];
  qWarning("============== init structures O.K.");
}

void Internal::read_settings ()
{
  // Path to the current directory:
  QString cd0 = this->current_dir;

  // Path to the settings file:
  QFile file (cd0.append ("/share/vecds/internals/settings.set0"));

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
      qWarning("===== ERROR    file 'settings.set0' not found");
      return;
    }

  // Initialize the text stream:
  QTextStream in (&file);
  int count = 0;

  // Read in the file settings set0;
  while (!in.atEnd()) 
    {
      QString line = in.readLine();

      // Ignore comments:
      if (line.startsWith("#")) 
	continue;

      QStringList fields = line.split (QRegExp("\\s+"), 
				       QString::SkipEmptyParts);

      switch (count)
	{
	case 0:
	  this->set0->fov = fields.takeFirst().toFloat();
	  goto endloop;
	  break;
	  
	case 1:
	  this->set0->mfact = fields.takeFirst().toDouble();
	  goto endloop;
	  break;

	case 2:
	  if (fields.size() != 8) 
	    qWarning("Settings - line 3 - error!");
	  for (int i=0; i<8; i++) 
	    this->set0->vis[i] = 
	      (fields.takeFirst().toInt()==0) ? false : true;
	  goto endloop;
	  break;

	default:
	  // This should be a warning or crash!
	  goto endloop;
	  break;
	}

      // Fix the RGB colour spectrum:
      this->set0->colour_spectrum[count-3] = 
	QColor (fields.takeFirst ().toInt (),
		fields.takeFirst ().toInt (),
		fields.takeFirst ().toInt (),
		/* alpha = */ 255);
      
    endloop:

      ++count;
    }
  qWarning("Settings : count=%d", count);  
}

void Internal::read_alchemy_xyz(QString aname)
{
//  qWarning(" read_alc_xyz");
  this->a_min_ = QVector3D (1.e15, 1.e15, 1.e15);
  this->a_max_ = QVector3D (-1.e15, -1.e15, -1.e15);

  // If there is old data in storage, get rid of it:
  if (this->atoms->n_atoms != 0) {
     delete [] this->atoms->type;
     delete [] this->atoms->coord;
     delete [] this->atoms->marked;
     delete [] this->atoms->add;
     delete [] this->atoms->add1;
  }
  if ( this->atoms->n_bonds>0 ) this->atoms->bonds.clear(); //delete [] this->atoms->bonds;

//  qWarning("old atoms deleted  -- bonds");

  // Try to open the file (r**):

  QFile file(aname);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       qWarning("===== ERROR  file %s not found", aname.toAscii().data());
       return;
  }

  QTextStream in(&file);
  QString line = in.readLine();
  QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
  this->atoms->n_marked = this->atoms->n_atoms = fields.takeFirst().toInt();
// Initialise data fields
  this->atoms->type   = new int[this->atoms->n_atoms];
  this->atoms->coord  = new QVector3D[this->atoms->n_atoms];
  this->atoms->marked = new bool[this->atoms->n_atoms];
  this->atoms->u      = new QVector3D[this->atoms->n_atoms];
  this->atoms->add    = new QVector3D[this->atoms->n_atoms];
  this->atoms->add1   = new QVector3D[this->atoms->n_atoms];
  QVector<int> bonds(2*this->atoms->n_bonds);

  bool alc_ = aname.contains(".alc");
  if ( alc_ ) {
      // Checks on the header information:
//      Q_ASSERT (fields.size () == 6);

      qWarning ("Found an alchemy file.");
      qWarning ("   Number of fields:  %d", fields.size ());      

      fields.removeFirst();
      this->atoms->n_bonds = fields.takeFirst().toInt();
      if ( this->atoms->n_bonds>0 )
           qWarning("read_alc::atoms->n_bonds = %d", atoms->n_bonds);
  } else {  // !alc_
      this->atoms->n_bonds = 0;
      line = in.readLine();
  }
 qWarning("READ_ALC     ---   n_atoms = %d", this->atoms->n_atoms);
  for (int i=0; i<this->atoms->n_atoms; i++) {
       line = in.readLine();
       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
//       int nf = fields.size(); 
       if ( alc_ ) fields.removeFirst();
       QString nam_a = fields.takeFirst();
       int ak = which_atom(nam_a);
       if ( ak==0 ) qWarning("unknown atom in line nr. %d in %s",
                                         i+1, aname.toAscii().data());
       this->atoms->type[i] = ak;
       this->atoms->coord[i].setX (fields.takeFirst ().toDouble ()); // *1.e10;
       this->atoms->coord[i].setY (fields.takeFirst ().toDouble ()); // *1.e10;
       this->atoms->coord[i].setZ (fields.takeFirst ().toDouble ()); // *1.e10;

       this->atoms->marked[i] = true;
  }
  minmax3(this->atoms->coord, this->atoms->n_atoms, this->a_min_, this->a_max_);
  this->atoms_loaded = aname;
  // If this is not an alchemy file, we're finished:
  if ( !alc_ ) return;

  for (int i=0; i<this->atoms->n_bonds; i++) {
       line = in.readLine();
       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
       fields.removeFirst();
       this->atoms->bonds << fields.takeFirst ().toInt ()
                          << fields.takeFirst ().toInt ();
//       qWarning("bonds: %d %d", atoms->bonds[i], Actual->bonds2[i]);
  }

}

void Internal::init_e0()
{
  // initialize this finite element as a 27-node element.
  init_c27();
  qWarning("+++++++++++++++  E0 INITIALIZED  +++++++++++++++++++");
}

void Internal::init_c27()
{/*
  c27.e0_name    = "c27";
  c27.e0_n_faces = 6;
  c27.e0_n_nodes = 27;
  c27.e0_faces   = new Face[6];

  for (int i=0; i<6; i++) 
    {
      c27.e0_faces[i].f_nodes     = new int[9];
      c27.e0_faces[i].f_extlines  = new QVector<int>[8];
      c27.e0_faces[i].f_intlines  = new QVector<int>[4];
      c27.e0_faces[i].f_triangles = new QVector<int>[8];
    }

  for (int i=0; i<6; i++) 
    {
      //     (c27.e0_faces[i]) = new Face();
      c27.e0_faces[i].n_nodes     = 9;
      c27.e0_faces[i].n_extlines  = 8;
      c27.e0_faces[i].n_intlines  = 4;
      c27.e0_faces[i].n_triangles = 8;
    }

  // face[0] - front
  
  // Enumerate the nodes on each of the faces of this element:
  c27.e0_faces[0].f_nodes[0] = 0;
  c27.e0_faces[0].f_nodes[1] = 8;
  c27.e0_faces[0].f_nodes[2] = 1;
  c27.e0_faces[0].f_nodes[3] = 17;
  c27.e0_faces[0].f_nodes[4] = 5;
  c27.e0_faces[0].f_nodes[5] = 12;
  c27.e0_faces[0].f_nodes[6] = 4;
  c27.e0_faces[0].f_nodes[7] = 16;
  c27.e0_faces[0].f_nodes[8] = 23;

  c27.e0_faces[0].f_quad        = Int4(0, 4, 5, 1);
  c27.e0_faces[0].f_extlines[0] = QVector<int> (0, 8);
  c27.e0_faces[0].f_extlines[1] = QVector<int> (8, 1);
  c27.e0_faces[0].f_extlines[2] = QVector<int> (1, 17);
  c27.e0_faces[0].f_extlines[3] = QVector<int> (17, 5);
  c27.e0_faces[0].f_extlines[4] = QVector<int> (5, 12);
  c27.e0_faces[0].f_extlines[5] = QVector<int> (12, 4);
  c27.e0_faces[0].f_extlines[6] = QVector<int> (4, 16);
  c27.e0_faces[0].f_extlines[7] = QVector<int> (16, 0);
  c27.e0_faces[0].f_intlines[0] = QVector<int> (8, 23);
  c27.e0_faces[0].f_intlines[1] = QVector<int> (17, 23);
  c27.e0_faces[0].f_intlines[2] = QVector<int> (12, 23);
  c27.e0_faces[0].f_intlines[3] = QVector<int> (16, 23);

  // face[1] - back
  c27.e0_faces[1].f_nodes[0] = 2;
  c27.e0_faces[1].f_nodes[1] = 10;
  c27.e0_faces[1].f_nodes[2] = 3;
  c27.e0_faces[1].f_nodes[3] = 19;
  c27.e0_faces[1].f_nodes[4] = 7;
  c27.e0_faces[1].f_nodes[5] = 14;
  c27.e0_faces[1].f_nodes[6] = 6;
  c27.e0_faces[1].f_nodes[7] = 18;
  c27.e0_faces[1].f_nodes[8] = 22;
  c27.e0_faces[1].f_quad     = Int4(7, 3, 2, 6);

  c27.e0_faces[1].f_extlines[0] = QVector<int> (2, 10);
  c27.e0_faces[1].f_extlines[1] = QVector<int> (10, 3);
  c27.e0_faces[1].f_extlines[2] = QVector<int> (3, 19);
  c27.e0_faces[1].f_extlines[3] = QVector<int> (19, 7);
  c27.e0_faces[1].f_extlines[4] = QVector<int> (7, 14);
  c27.e0_faces[1].f_extlines[5] = QVector<int> (14, 6);
  c27.e0_faces[1].f_extlines[6] = QVector<int> (6, 18);
  c27.e0_faces[1].f_extlines[7] = QVector<int> (18, 2);
  c27.e0_faces[1].f_intlines[0] = QVector<int> (10, 22);
  c27.e0_faces[1].f_intlines[1] = QVector<int> (19, 22);
  c27.e0_faces[1].f_intlines[2] = QVector<int> (14, 22);
  c27.e0_faces[1].f_intlines[3] = QVector<int> (18, 22);

  // face[2] - left
  c27.e0_faces[2].f_nodes[0] = 3;
  c27.e0_faces[2].f_nodes[1] = 11;
  c27.e0_faces[2].f_nodes[2] = 0;
  c27.e0_faces[2].f_nodes[3] = 16;
  c27.e0_faces[2].f_nodes[4] = 4;
  c27.e0_faces[2].f_nodes[5] = 15;
  c27.e0_faces[2].f_nodes[6] = 7;
  c27.e0_faces[2].f_nodes[7] = 19;
  c27.e0_faces[2].f_nodes[8] = 20;
  c27.e0_faces[2].f_quad = Int4(0, 3, 7, 4);
  c27.e0_faces[2].f_extlines[0] = QVector<int> (3, 11);
  c27.e0_faces[2].f_extlines[1] = QVector<int> (11, 0);
  c27.e0_faces[2].f_extlines[2] = QVector<int> (0, 16);
  c27.e0_faces[2].f_extlines[3] = QVector<int> (16, 4);
  c27.e0_faces[2].f_extlines[4] = QVector<int> (4, 15);
  c27.e0_faces[2].f_extlines[5] = QVector<int> (15, 7);
  c27.e0_faces[2].f_extlines[6] = QVector<int> (7, 19);
  c27.e0_faces[2].f_extlines[7] = QVector<int> (19, 3);
  c27.e0_faces[2].f_intlines[0] = QVector<int> (11, 20);
  c27.e0_faces[2].f_intlines[1] = QVector<int> (16, 20);
  c27.e0_faces[2].f_intlines[2] = QVector<int> (15, 20);
  c27.e0_faces[2].f_intlines[3] = QVector<int> (19, 20);

  // face[3] - right
  c27.e0_faces[3].f_nodes[0] = 1;
  c27.e0_faces[3].f_nodes[1] = 9;
  c27.e0_faces[3].f_nodes[2] = 2;
  c27.e0_faces[3].f_nodes[3] = 18;
  c27.e0_faces[3].f_nodes[4] = 6;
  c27.e0_faces[3].f_nodes[5] = 13;
  c27.e0_faces[3].f_nodes[6] = 5;
  c27.e0_faces[3].f_nodes[7] = 17;
  c27.e0_faces[3].f_nodes[8] = 21;

  c27.e0_faces[3].f_quad = Int4(5, 6, 2, 1);
  c27.e0_faces[3].f_extlines[0] = QVector<int> (1, 9);
  c27.e0_faces[3].f_extlines[1] = QVector<int> (9, 2);
  c27.e0_faces[3].f_extlines[2] = QVector<int> (2, 18);
  c27.e0_faces[3].f_extlines[3] = QVector<int> (18, 6);
  c27.e0_faces[3].f_extlines[4] = QVector<int> (6, 13);
  c27.e0_faces[3].f_extlines[5] = QVector<int> (13, 5);
  c27.e0_faces[3].f_extlines[6] = QVector<int> (5, 17);
  c27.e0_faces[3].f_extlines[7] = QVector<int> (17, 1);
  c27.e0_faces[3].f_intlines[0] = QVector<int> (9, 21);
  c27.e0_faces[3].f_intlines[1] = QVector<int> (18, 21);
  c27.e0_faces[3].f_intlines[2] = QVector<int> (13, 21);
  c27.e0_faces[3].f_intlines[3] = QVector<int> (17, 21);

  // face[4] - top
  c27.e0_faces[4].f_nodes[0] = 4;
  c27.e0_faces[4].f_nodes[1] = 12;
  c27.e0_faces[4].f_nodes[2] = 5;
  c27.e0_faces[4].f_nodes[3] = 13;
  c27.e0_faces[4].f_nodes[4] = 6;
  c27.e0_faces[4].f_nodes[5] = 14;
  c27.e0_faces[4].f_nodes[6] = 7;
  c27.e0_faces[4].f_nodes[7] = 15;
  c27.e0_faces[4].f_nodes[8] = 25;
  c27.e0_faces[4].f_quad = Int4(4, 7, 6, 5);
  c27.e0_faces[4].f_extlines[0] = QVector<int> (4, 12);
  c27.e0_faces[4].f_extlines[1] = QVector<int> (12, 5);
  c27.e0_faces[4].f_extlines[2] = QVector<int> (5, 13);
  c27.e0_faces[4].f_extlines[3] = QVector<int> (13, 6);
  c27.e0_faces[4].f_extlines[4] = QVector<int> (6, 14);
  c27.e0_faces[4].f_extlines[5] = QVector<int> (14, 7);
  c27.e0_faces[4].f_extlines[6] = QVector<int> (7, 15);
  c27.e0_faces[4].f_extlines[7] = QVector<int> (15, 4);
  c27.e0_faces[4].f_intlines[0] = QVector<int> (12, 25);
  c27.e0_faces[4].f_intlines[1] = QVector<int> (13, 25);
  c27.e0_faces[4].f_intlines[2] = QVector<int> (14, 25);
  c27.e0_faces[4].f_intlines[3] = QVector<int> (15, 25);

  // face[5] - bottom
  c27.e0_faces[5].f_nodes[0] = 3;
  c27.e0_faces[5].f_nodes[1] = 10;
  c27.e0_faces[5].f_nodes[2] = 2;
  c27.e0_faces[5].f_nodes[3] = 9;
  c27.e0_faces[5].f_nodes[4] = 1;
  c27.e0_faces[5].f_nodes[5] = 8;
  c27.e0_faces[5].f_nodes[6] = 0;
  c27.e0_faces[5].f_nodes[7] = 11;
  c27.e0_faces[5].f_nodes[8] = 24;
  c27.e0_faces[5].f_quad = Int4(3, 0, 1, 2);
  c27.e0_faces[5].f_extlines[0] = QVector<int> (3, 10);
  c27.e0_faces[5].f_extlines[1] = QVector<int> (10, 2);
  c27.e0_faces[5].f_extlines[2] = QVector<int> (2, 9);
  c27.e0_faces[5].f_extlines[3] = QVector<int> (9, 1);
  c27.e0_faces[5].f_extlines[4] = QVector<int> (1, 8);
  c27.e0_faces[5].f_extlines[5] = QVector<int> (8, 0);
  c27.e0_faces[5].f_extlines[6] = QVector<int> (0, 11);
  c27.e0_faces[5].f_extlines[7] = QVector<int> (11, 3);
  c27.e0_faces[5].f_intlines[0] = QVector<int> (10, 24);
  c27.e0_faces[5].f_intlines[1] = QVector<int> (11, 24); //(19, 24);
  c27.e0_faces[5].f_intlines[2] = QVector<int> (8, 24);  //(14, 24);
  c27.e0_faces[5].f_intlines[3] = QVector<int> (9, 24);  //(18, 24);

  for (int i=0; i<6; ++i) 
    for (int j=0; j<4; ++j) 
      qWarning("f=%d, l=%d --- %d - %d ", i, j, 
	       c27.e0_faces[i].f_intlines[j][1], 
	       c27.e0_faces[i].f_intlines[j][2]);

  // triangles for all faces
  for (int i=0; i<c27.e0_n_faces; i++) 
    {
      c27.e0_faces[i].tex = 0;

      c27.e0_faces[i].f_triangles[0].push_back (c27.e0_faces[i].f_nodes[0]);
      c27.e0_faces[i].f_triangles[0].push_back (c27.e0_faces[i].f_nodes[1]);
      c27.e0_faces[i].f_triangles[0].push_back (c27.e0_faces[i].f_nodes[8]);

      c27.e0_faces[i].f_triangles[1].push_back (c27.e0_faces[i].f_nodes[0]);
      c27.e0_faces[i].f_triangles[1].push_back (c27.e0_faces[i].f_nodes[8]);
      c27.e0_faces[i].f_triangles[1].push_back (c27.e0_faces[i].f_nodes[7]);

      c27.e0_faces[i].f_triangles[2].push_back (c27.e0_faces[i].f_nodes[1]);
      c27.e0_faces[i].f_triangles[2].push_back (c27.e0_faces[i].f_nodes[2]);
      c27.e0_faces[i].f_triangles[2].push_back (c27.e0_faces[i].f_nodes[3]);

      c27.e0_faces[i].f_triangles[3].push_back (c27.e0_faces[i].f_nodes[1]);
      c27.e0_faces[i].f_triangles[3].push_back (c27.e0_faces[i].f_nodes[3]);
      c27.e0_faces[i].f_triangles[3].push_back (c27.e0_faces[i].f_nodes[8]);

      c27.e0_faces[i].f_triangles[4].push_back (c27.e0_faces[i].f_nodes[8]);
      c27.e0_faces[i].f_triangles[4].push_back (c27.e0_faces[i].f_nodes[3]);
      c27.e0_faces[i].f_triangles[4].push_back (c27.e0_faces[i].f_nodes[4]);

      c27.e0_faces[i].f_triangles[5].push_back (c27.e0_faces[i].f_nodes[8]);
      c27.e0_faces[i].f_triangles[5].push_back (c27.e0_faces[i].f_nodes[4]);
      c27.e0_faces[i].f_triangles[5].push_back (c27.e0_faces[i].f_nodes[5]);

      c27.e0_faces[i].f_triangles[6].push_back (c27.e0_faces[i].f_nodes[7]);
      c27.e0_faces[i].f_triangles[6].push_back (c27.e0_faces[i].f_nodes[8]);
      c27.e0_faces[i].f_triangles[6].push_back (c27.e0_faces[i].f_nodes[5]);

      c27.e0_faces[i].f_triangles[7].push_back (c27.e0_faces[i].f_nodes[7]);
      c27.e0_faces[i].f_triangles[7].push_back (c27.e0_faces[i].f_nodes[5]);
      c27.e0_faces[i].f_triangles[7].push_back (c27.e0_faces[i].f_nodes[6]);
    }   */
}


int Internal::which_atom(QString nam_a)
{
  for (int i=1; i<125; i++)
     if ( nam_a.startsWith(this->ap->namea[i], Qt::CaseInsensitive) ) 
       return i;
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

  for (int i=0; i<numb; i++) 
    {
      QVector3D temp = vec[i];
      if (temp.x()>mxx) mxx = temp.x();
      if (temp.y()>mxy) mxy = temp.y();
      if (temp.z()>mxz) mxz = temp.z();
      if (temp.x()<mnx) mnx = temp.x();
      if (temp.y()<mny) mny = temp.y();
      if (temp.z()<mnz) mnz = temp.z();
    }
  vmax = QVector3D (mxx, mxy, mxz);
  vmin = QVector3D (mnx, mny, mnz);
}

void Internal::minmax (double *vec, int numb, 
		       double &vmin, double &vmax)
{
  // To get the min and max of the entries in the vector, first set
  // "min" and "max" variables to the first entry:
  double max = vec[0];
  double min = vec[0];

  // Then loop over all other entries:
  for (int i=1; i<numb; ++i) 
    {
      if (vec[i] < min) 
	min = vec[i];

      if (vec[i] > max) 
	max = vec[i];
    }

  vmin = min;
  vmax = max;

}


bool Internal::func (Int4 k1, Int4 k2) 
{

  // This can be changed after QVector<int> is implemented:
  //
  // bool identical_entry = false;
  //
  // for (unsigned int i=0; i<k1.size (); ++i)
  //   for (unsigned int j=0; j<k1.size (); ++j)
  //     {
  // 	if (k1[i] == k2[j])
  // 	  identical_entry = true;
  //     }
  // return identical_entry;

  if ( (k1.i1 == k2.i1 || 
	k1.i1 == k2.i2 || 
	k1.i1 == k2.i3 || 
	k1.i1 == k2.i4) &&
       
       (k1.i2 == k2.i1 || 
	k1.i2 == k2.i2 || 
	k1.i2 == k2.i3 || 
	k1.i2 == k2.i4) &&
       
       (k1.i3 == k2.i1 || 
	k1.i3 == k2.i2 || 
	k1.i3 == k2.i3 || 
	k1.i3 == k2.i4) &&
       
       (k1.i4 == k2.i1 || 
	k1.i4 == k2.i2 || 
	k1.i4 == k2.i3 || 
	k1.i4 == k2.i4) )
    
    return true;

  else 
    return false;
}

void Internal::calc_faces()
{   
 Int4 *qs;
 bool *qb;
 
// qWarning("calc_faces --- begin");
   int ind = 0;
   qs = new Int4[6*elems->n_elements];
   qb = new bool[6*elems->n_elements];
 qWarning("calc_faces --- 1");
 for (unsigned int i=0; i<elems->n_elements; i++) 
   {
     for (int j=0; j<6; j++) 
       {
	 Int4 fq = this->c27.e0_faces[j].f_quad;

	 int p1 = this->elems->el_nodes[i][fq.i1];
	 int p2 = this->elems->el_nodes[i][fq.i2];
	 int p3 = this->elems->el_nodes[i][fq.i3];
	 int p4 = this->elems->el_nodes[i][fq.i4];
	 
	 qs[ind]   = Int4(p1, p2, p3, p4);
	 qb[ind++] = true;
       }
   }

 qWarning("calc_faces --- ************* numbE=%d", elems->n_elements);
 //           elems->el_nodes[i][j]

 for (unsigned int i=0; i<elems->n_elements; i++) 
   {
     //        qWarning("calc_faces --- 2 ==== %d", i);
     unsigned int j0 = i+1;
     if ( j0>=this->elems->n_elements ) continue;
     for (unsigned int j=j0; j<this->elems->n_elements; j++) 
       {
	 for (int k1=0; k1<6; k1++) {
	   for (int k2=0; k2<6; k2++) {
	     if ( func(qs[6*i+k1], qs[6*j+k2]) ) {
	       qb[6*i + k1] = false;
	       qb[6*j + k2] = false;
	       //                qWarning("calc_faces --- bad  (%d %d)=(%d %d)", i, k1, j, k2);
	       goto next;
	     }
	   }
	 }
       next: continue;
       }
   }
 this->numbFaces = 0;
 
 for (unsigned int i = 0; i<6*this->elems->n_elements; i++) if ( qb[i] ) ++this->numbFaces;
 // qWarning("calc_faces --- 2 ==== nFaces=%d", this->numbFaces);
 this->faces = new Face[this->numbFaces];
 qWarning("calc_faces --- numb faces = %d", this->numbFaces);
 ind = 0;
 for (unsigned int l = 0; l<6*this->elems->n_elements; l++) {
   int i = l/6;
   int j = l%6;
   if ( qb[l] ) {
     //      qWarning ("l= %d ---- i=%d, j=%d", l, i, j);
     this->faces[ind].nr_elem = i;
     this->faces[ind].nr_face = j;
     int nl_nod = this->faces[ind].n_nodes = 
       this->c27.e0_faces[j].n_nodes;
     int nl_ext = this->faces[ind].n_extlines = 
       this->c27.e0_faces[j].n_extlines;
     int nl_int = this->faces[ind].n_intlines = 
       this->c27.e0_faces[j].n_intlines;
     int nl_tr = this->faces[ind].n_triangles = 
       this->c27.e0_faces[j].n_triangles;
     
     Int4 quad = this->c27.e0_faces[j].f_quad;
     this->faces[ind].f_quad = Int4(quad.i1, quad.i2, quad.i3, quad.i4);
     //  qWarning ("1 ----- nl_int= %d nl_ext=%d", nl_int, nl_ext);
     this->faces[ind].f_nodes = new int[nl_nod];
     for (int k=0; k<nl_nod; k++) {
       int nod = this->c27.e0_faces[j].f_nodes[k];
       this->faces[ind].f_nodes[k] = nod;
     }
/*     //  qWarning ("2 ----- nl_int= %d nl_ext=%d", nl_int, nl_ext);
     this->faces[ind].f_intlines = new QVector<int>[nl_int];
     for (int k=0; k<nl_int; k++) {
       QVector<int> intl = this->c27.e0_faces[j].f_intlines[k];
       this->faces[ind].f_intlines[k] = intl;
     }
     this->faces[ind].f_extlines = new QVector<int>[nl_ext];
     for (int k=0; k<nl_ext; k++) {
       QVector<int> extl = this->c27.e0_faces[j].f_extlines[k];
       this->faces[ind].f_extlines[k] = extl;
     }
     this->faces[ind].f_triangles = new QVector<int> [nl_tr];
     for (int k=0; k<nl_tr; k++) {
       QVector<int>  tr = this->c27.e0_faces[j].f_triangles[k];
       this->faces[ind].f_triangles[k] = tr;
     }                                                    */
     ++ind;
   }
 }
 qWarning("calc_faces --- 3");
 if ( ind!=this->numbFaces ) qWarning("ERROR - numbFaces");
 delete[] qs;
 delete[] qb;
 
 //   this->spectrograms = new QImage* [this->numbFaces];
}

void Internal::do_axis_rotation(Mat9d r_tens)
{
  this->axeX = matvecmult(r_tens, this->axeX);
  this->axeY = matvecmult(r_tens, this->axeY);
  this->axeZ = matvecmult(r_tens, this->axeZ);
}

void Internal::saveAtoms(QString sname)
{
  extPrint(sname, atoms->coord);
/*
  QString line;
//  QString cd0=this->current_dir;
//  QFile fil(cd0.append("/data/atoms/").append(sname));
  QFile fil(sname);

// qWarning("file: %s, numChoosedAtoms=%d", sname.toAscii().data(), Actual->n_markedAtoms);  
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
*/
}


void Internal::extPrint(QString file_name, QVector3D *vec)
{
  QString line;
//  QString cd0=this->current_dir;
//  QFile fil(cd0.append("/data/atoms/").append(sname));
  QFile fil(file_name);

// qWarning("file: %s, numChoosedAtoms=%d", sname.toAscii().data(), Actual->n_markedAtoms);  
  if (!fil.open(QIODevice::WriteOnly)) { 
     qWarning("Cannot open for writing: %s", qPrintable(fil.errorString()));
     return; 
  }
  QTextStream out(&fil);
  out << line.sprintf("%d\n ---\n", atoms->n_atoms);
  for (unsigned int i=0; i<atoms->n_atoms; i++)
     out << line.sprintf("%s  %12.7f %12.7f %12.7f\n", 
                               ap->namea[atoms->type[i]].toAscii().data(),
                                       vec[i].x(), vec[i].y(), vec[i].z());
}

void Internal::saveMarkedAtoms(QString sname)
{

  QString line;
//  QString cd0=this->cur_dir;
//  QFile fil(cd0.append("/atoms/").append(sname));
  QFile fil(sname);

// qWarning("file: %s, numMarkedAtoms=%d", sname.toAscii().data(), Actual->num_markedAtoms);  
  if (!fil.open(QIODevice::WriteOnly)) { 
     qWarning("Cannot open for writing: %s", qPrintable(fil.errorString()));
     return; 
  }
  QTextStream out(&fil);
  out << line.sprintf("%d\n ---\n", atoms->n_marked);
  for (int i=0; i<atoms->n_atoms; i++)
     if ( atoms->marked[i] )
        out << line.sprintf("%4s %12.7f %12.7f %12.7f\n",
                      ap->namea[atoms->type[i]].toAscii().data(),
                      atoms->coord[i].x(), atoms->coord[i].y(), atoms->coord[i].z());

}

void Internal::SL_singleDisl(QVector3D rr)
{
  double a = actcrcell->a;
  double c = actcrcell->c;
 
  rr += this->cent_;
  actdisl->rrr = rr;// + this->cent_;
  actdisl->burgers_name = this->act_disl;
  actdisl->p1 = QVector3D(rr.x(), rr.y(), this->a_min_.z());
  actdisl->p2 = QVector3D(rr.x(), rr.y(), this->a_max_.z());
 qWarning("SL_singleDisl - p1, p2 - (%g %g %g) (%g %g %g)",
    actdisl->p1.x(), actdisl->p1.y(), actdisl->p1.z(), actdisl->p2.x(), actdisl->p2.y(), actdisl->p2.z());
  actdisl->burgers_vector.setX (fraction * indMiller[0] * a);
  actdisl->burgers_vector.setY (fraction * indMiller[1] * a);
  actdisl->burgers_vector.setZ (fraction * indMiller[2] * c);
//  actdisl->rotation_tensor = this->rot_tensor;

  calc_disl0();
  
  disl[n_dislocations++] = *actdisl;
  //  actdisl->rrr.z = Actual->cent_.z;
  qWarning("SINGLE DISL %d,  rrr= (%g %g %g)", n_dislocations-1, 
	   actdisl->rrr.x(), actdisl->rrr.y(), actdisl->rrr.z());
  return; 
}

void Internal::newdisl(int n_a)
{
// FILE *fout, *fout1;
qWarning("newdisl");
  double a = actcrcell->a;
  double c = actcrcell->c;
 
  QVector3D rr = this->atoms->coord[n_a];// + this->cent_;
  actdisl->rrr = rr;// + this->cent_;
  actdisl->burgers_name = this->act_disl;
  actdisl->p1 = QVector3D(rr.x(), rr.y(), this->a_min_.z());
  actdisl->p2 = QVector3D(rr.x(), rr.y(), this->a_max_.z());
  qWarning("SL_newDisl - p1, p2 - (%g %g %g) (%g %g %g)",
	   actdisl->p1.x(), actdisl->p1.y(), actdisl->p1.z(), 
	   actdisl->p2.x(), actdisl->p2.y(), actdisl->p2.z());

  actdisl->burgers_vector.setX(fraction * indMiller[0] * a);
  actdisl->burgers_vector.setY(fraction * indMiller[1] * a);
  actdisl->burgers_vector.setZ(fraction * indMiller[2] * c);

  qWarning ("SL_newDisl - burgers vector -BEFORE- (%g %g %g)",
	    actdisl->burgers_vector.x(), 
	    actdisl->burgers_vector.y(), 
	    actdisl->burgers_vector.z());
/*
  actdisl->burgers_vector = matvecmult(this->mat, actdisl->burgers_vector);

  qWarning ("SL_newDisl - burgers vector -AFTER- (%g %g %g)",
	    actdisl->burgers_vector.x(), 
	    actdisl->burgers_vector.y(), 
	    actdisl->burgers_vector.z());
*/
//  actdisl->rotation_tensor = this->rot_tensor;

//  calc_disl0();
  actdisl->cd = atoms->coord[n_a];// + matvecmult(this->rot_tensor, dislcore);
  actdisl->i0 = n_a;
  actdisl->dislocation_core = QVector3D(0., 0., 0.);
  actdisl->core_name = QString("none");

  QString  nazwa;
  nazwa.sprintf("poleU-%02d", n_dislocations+1);
  extPrint(QString("POLE-0a"), atoms->coord);
  
  for (int i=0; i<atoms->n_atoms; i++) { 
     atoms->u[i] = mixed_u(i);
     atoms->coord[i] += atoms->u[i];
  }
  extPrint(nazwa, atoms->u);
  extPrint(QString("POLE-1a"), atoms->add);
  extPrint(QString("POLE-2a"), atoms->add1);
// qWarning("uwaga!");
  disl[n_dislocations++] = *actdisl;
//  actdisl->rrr.z = Actual->cent_.z;
  qWarning("NEW DISL %d,  rrr= (%g %g %g)", n_dislocations-1, 
	   actdisl->rrr.x(), actdisl->rrr.y(), actdisl->rrr.z());
  return; 
}


// ------------------------------------------------

void Internal::calc_disloc(int nr_atom, int disl_num)
{

  int i0 = atomize(actdisl->rrr, nr_atom);
 qWarning("SINGLE DISL  num=%d - i0=%d - coordinates=(%g %g %g)", disl_num, i0,
              atoms->coord[i0].x(), atoms->coord[i0].y(), atoms->coord[i0].z());
  if ( i0==-1 ) qWarning("Error i0 for %d dislocation\n", n_dislocations);
  QVector3D dislcore = matvecmult(actcrcell->c2o, actdisl->dislocation_core);
//dislcore = matvecmult(actcrcell->c2o, actdisl->core);
  dislcore.setX(actdisl->dislocation_core.x()*actcrcell->c2o[0] + 
		actdisl->dislocation_core.y()*actcrcell->c2o[1]);

  dislcore.setY(actdisl->dislocation_core.x()*actcrcell->c2o[3] + 
		actdisl->dislocation_core.y()*actcrcell->c2o[4]);

  dislcore.setZ(actdisl->dislocation_core.x()*actcrcell->c2o[6] + 
		actdisl->dislocation_core.y()*actcrcell->c2o[7]);

  actdisl->cd = atoms->coord[i0] + matvecmult(this->rot_tensor, dislcore);
  actdisl->i0 = i0;
 qWarning("SINGLE DISL corrected coordinates = (%g %g %g),  i0=%d", 
                     actdisl->cd.x(), actdisl->cd.y(), actdisl->cd.z(), i0);

  for (unsigned int i=0; i<atoms->n_atoms; i++) { 
     atoms->u[i] = mixed_u(i);
     atoms->coord[i] += atoms->u[i];
  }
 qWarning("Mixed_u end");
//  emit SIG_needDraw();
}
//---------------------------------------------------------------------

int Internal::atomize(QVector3D point, int nr_atom)
{
  int i0 = -1;
  double dist = 1.e33;

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
  if (i0==-1) 
    qWarning("Error i0 for %d dislocation\n", n_dislocations);

  actdisl->cd = atoms->coord[i0];// + matvecmult(this->rot_tensor, dislcore);
  actdisl->i0 = i0;
 qWarning("SINGLE DISL corrected coordinates = (%g %g %g),  i0=%d", 
                     actdisl->cd.x(), actdisl->cd.y(), actdisl->cd.z(), i0);
// qWarning("n_atoms =%d", atoms->n_atoms);
  for (unsigned int i=0; i<atoms->n_atoms; i++) { 
     atoms->u[i] = mixed_u(i);
     atoms->coord[i] += atoms->u[i];
  }
//  emit SIG_needDraw();
}
/*
      subroutine mixed_u(b,cd,coord,u)

!     Input:         b - value of burgers vector (parallel to x-axis)
!                cd(2) - x,y coordinates of dislocation
!             coord(2) - reference position of given atom in a perfect lattice
!     Ouput:      u(2) - displacement field modified by dislocation, i.e. u(i) = u(i) + du(i)
!     written by Paweł Dłużewski (utf8), February 2007, www.ippt.gov.pl/pdluzew                   

      implicit none
      real(8) b(3),cd(2),coord(3),u(3),be,nu,r,x,y,pi,c,s,ux,uy
      parameter (pi=acos(-1.d0))
      nu = 0.35
      r = sqrt((coord(1) - cd(1))**2+(coord(2) - cd(2))**2)
      if(r.lt.1.d-33) stop
     $                ('ERROR: Atom in the center of dislocation core!')
      be = sqrt(b(1)*b(1)+b(2)*b(2))
      if(be.gt.1.e-16) then
         c = b(1)/be
         s = b(2)/be
      else
         c = 1.d0
         s = 0.d0
      endif
      x = c*(coord(1)-cd(1))/r +s*(coord(2)-cd(2))/r
      y =-s*(coord(1)-cd(1))/r +c*(coord(2)-cd(2))/r

      ux =  be  /(2.*pi)*(atan2(y,x)+x*y/(2.*(1.-nu))) - 0.5*be  ! Hirth,Lothe(1982): Eqn.(3-45) (modified by PD)
      uy = -be  /(8.*pi*(1.-nu))*((1.-nu-nu)*log(r*r) + x*x-y*y) ! Eqn.(3-46) for normalized x,y

      u(1) = u(1) +c*ux -s*uy
      u(2) = u(2) +s*ux +c*uy
      u(3) = u(3) +b(3)/(2.*pi)*atan2(y,x)                       ! Eqn.(3-2)
      end 
*/

//---------------------------------------------------------------------
QVector3D Internal::mixed_u(int i)
{
//  double angle_fi = 180.;
// -----------------------------------------------
  const double nu = 0.35;
  double c, s;
  QString line;

  double bx = actdisl->burgers_vector.x();
  double by = actdisl->burgers_vector.y();
  double bz = actdisl->burgers_vector.z();

  double xx = atoms->coord[i].x() - actdisl->cd.x();
  double yy = atoms->coord[i].y() - actdisl->cd.y();
  double r2 = xx*xx + yy*yy;
  if ( r2<1e-33 ) {
     qWarning(
 " ERROR: Atom in the center of dislocation core!\ni=%d, coords = (%g, %g %g), r2=%g",
                 i, atoms->coord[i].x(), atoms->coord[i].y(), atoms->coord[i].z(), r2);
     return QVector3D(0., 0., 0.);
  }
  double be = sqrt(bx*bx + by*by);
  double r = sqrt(r2);
/*
  if ( be>1.e-16 ) {
         c = bx/be;
         s = by/be;
  } else {
         c = 1.;
         s = 0.;
  }
  double x =  c*xx/r + s*yy/r;
  double y = -s*xx/r + c*yy/r;
  if ( c!= 1. || s!= 0. ) qWarning("QQQQQQQQQQQQQQQQ---------   i=%d: c=%g s=%g  (%g %g %g)",
                                     i, c, s, atoms->coord[i].x(), atoms->coord[i].y(), atoms->coord[i].z());
*/
  double x = xx/r;
  double y = yy/r;
  atoms->add[i] = QVector3D(x, y, nu);
//  x = c*(coord(1)-cd(1))/r +s*(coord(2)-cd(2))/r
//  y =-s*(coord(1)-cd(1))/r +c*(coord(2)-cd(2))/r
//  double ux = 0.5*be*constant::inv_pi * (atan2(y, x) + x*y/((2.-nu-nu))) - 0.5*be;
//  double uy = -be/(8.*constant::pi*(1.-nu)) * ((1.-nu-nu)*log(r2) + x*x-y*y);
//  ux =  be  /(2.*pi)*(atan2(y,x)+x*y/(2.*(1.-nu))) - 0.5*be  ! Hirth,Lothe(1982): Eqn.(3-45) (modified by PD)
//  uy = -be  /(8.*pi*(1.-nu))*((1.-nu-nu)*log(r*r) + x*x-y*y) ! Eqn.(3-46) for normalized x,y
//  be *= 1.e-10;
  double r02 = 0.25*be*be;                  // invariant ring radius with respect to uy shift
  double ux =  be /(2.*constant::pi)*(atan2(y,x)+x*y/(2.*(1.-nu))) - 0.5*be;
  double uy = -be /(8.*constant::pi*(1.-nu))*((1.-nu-nu)*log(r2/r02) + x*x-y*y + 1.);
//  be *= 1.e10;
  atoms->add1[i] = QVector3D(ux, uy, nu);
//---------------------------------------------------
//    u(1) = u(1) +c*ux -s*uy
//    u(2) = u(2) +s*ux +c*uy
//    u(3) = u(3) +b(3)/(2.*pi)*atan2(y,x)                       ! Eqn.(3-2)
/*
  double uxx = c*ux - s*uy;
  double uyy = s*ux + c*uy;
  return QVector3D(uxx, uyy, bz/(2.*constant::pi)*atan2(y, x));
*/
  return QVector3D(ux, uy, bz/(2.*constant::pi)*atan2(y, x));
} // mixed_u 

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
  for (int j=0; j<this->n_dislocations; j++) 
    {
      disl[j].p1 = matvecmult(r_tens, disl[j].p1 - vec) + vec;
      disl[j].p2 = matvecmult(r_tens, disl[j].p2 - vec) + vec;
    }
}

int Internal::cubBox(QVector3D box1, QVector3D p0)
{
  int ret=0;
  for (int i=0; i<atoms->n_atoms; i++) {
     double ax = atoms->coord[i].x() - p0.x();
     double ay = atoms->coord[i].y() - p0.y();
     double az = atoms->coord[i].z() - p0.z();
     bool at = ( ax<box1.x() && ax>-box1.x() && ay<box1.y() && ay>-box1.y() &&
                                                    az<box1.z() && az>-box1.z() );
     atoms->marked[i] = at;
     if ( at ) ++ret;
  }
//  atoms->num_marked = ret;
 qWarning("num_marked = %d", ret);
  return ret;
}

int Internal::hexBox(double hex_r, double hex_h, QVector3D p0)
{
  int ret = 0;
  bool at;
  double ca, cx;

  double cos30 = 0.5*sqrt(3.);
  double hr = hex_r*cos30;
  for (int i=0; i<atoms->n_atoms; i++) {
     double c_x = atoms->coord[i].x() - p0.x();
     double c_y = atoms->coord[i].y() - p0.y();
     double c_z = atoms->coord[i].z() - p0.z();
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
     atoms->marked[i] = at;
     if ( at ) ++ret;
  }
//  atoms->num_marked = ret;
  return ret;
}

void Internal::read_fems(QString fname)
{ 
  qWarning(" read_fems  nodes=%d, elems=%d /old/", this->nodes->n_nodes, this->elems->n_elements);
    this->e_min_ = QVector3D(1.e15, 1.e15, 1.e15);
    this->e_max_ = QVector3D(-1.e15, -1.e15, -1.e15);

    QFile file(fname);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       qWarning("===== ERROR  file %s not found", fname.toAscii().data());
       return;
    }

    if ( this->nodes->n_nodes>0) {
       delete [] this->nodes->coords;
       this->nodes->coords = 0;
    }
    if ( this->elems->n_elements>0 ) {
       delete[] this->elems->n_nodes;
       delete[] this->elems->type;
       for (unsigned int i=0; i<this->elems->n_elements; i++) 
          delete[] this->elems->el_nodes[i];
       delete[] this->elems->el_nodes;
       this->elems->el_nodes = 0;
    }
// qWarning("aafemsdel_");

    QTextStream in(&file);
    QString line = in.readLine();
    QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    INT_nn = this->nodes->n_nodes = fields.takeFirst().toInt();
    INT_ne = this->elems->n_elements = fields.takeFirst().toInt();
  qWarning("numbElements, n_nodes: %d %d", INT_ne, INT_nn);
    this->nodes->coords = new QVector3D[INT_nn];
//    this->fems->elems = new Elem[fems->n_elements];
 qWarning("aafemsnew___1");
/*
    QFile file1("wynik2");
    if (!file1.open(QIODevice::WriteOnly | QIODevice::Text)) {
       qWarning("===== ERROR  wynik");
       return;
    }
    QTextStream out(&file1);
*/
 
// qWarning("nodes=%d, elements=%d", fems->n_nodes, fems->n_elements);
//    this->fems->nodes = new Node();
    for (unsigned int i=0; i<INT_nn; i++) {
       line = in.readLine();
       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
//       int nf = fields.size(); 
       fields.removeFirst();
       this->nodes->coords[i].setX(fields.takeFirst().toDouble());//*1.e10;
       this->nodes->coords[i].setY(fields.takeFirst().toDouble());//*1.e10;
       this->nodes->coords[i].setZ(fields.takeFirst().toDouble());//*1.e10;
//  qWarning("node=%d --- coords=(%g, %g, %g)", i, ax, ay, az);
//       out<<"i="<<i<<" ---  "<<ax<<"  "<<ay<<"  "<<az<<"\n";
    }
    minmax3(this->nodes->coords, INT_nn, this->e_min_, this->e_max_);
//  qWarning("aa-----nodes");
    line = in.readLine();
    this->elems->el_nodes = new unsigned int*[INT_ne];
    this->elems->n_nodes = new unsigned int[INT_ne];
    this->elems->type = new int[INT_ne];
//    el_nodes
    for (unsigned int i=0; i<INT_ne; i++) {
       line = in.readLine();
       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
//   int nf = fields.size(); 
       fields.removeFirst();
       fields.removeFirst();
       fields.removeFirst();

       this->elems->n_nodes[i] = 27;
       this->elems->type[i] = 1;
       this->elems->el_nodes[i] = new unsigned int[27];

       for (int j=0; j<8; j++) this->elems->el_nodes[i][j] =
                                           fields.takeFirst().toInt() - 1;
       line = in.readLine();
       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
       for (int j=8; j<16; j++) this->elems->el_nodes[i][j] =
                                            fields.takeFirst().toInt() - 1;
       line = in.readLine();
       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
       for (int j=16; j<24; j++) this->elems->el_nodes[i][j] =
                                            fields.takeFirst().toInt() - 1;
       line = in.readLine();
       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
       for (int j=24; j<27; j++) this->elems->el_nodes[i][j] = 
                                            fields.takeFirst().toInt() - 1;

//       out << "i="<<i<<"\n";
//       for (int j=0; j<27; j++) out <<"  " <<elems->el_nodes[i][j];
//       out<<"\n";
//   qWarning("=========== element = %d ===============", i);
//      for (int j=0; j<27; j++) 
//         qWarning("i=%d, j=%d ----  %d", i, j, elems->el_nodes[i][j]);
//   qWarning("%d %d %d %d %d", elems->el_nodes[i][0], elems->el_nodes[i][1],
//      elems->el_nodes[i][2], elems->el_nodes[i][3], elems->el_nodes[i][4]);
//      for (int j=0; j<this->nodes->n_nodes; j++)
//         qWarning("j =%d, node=%d, ev_node=%d", j, elems->el_nodes[i][j], c27.e0_rev_nodes[elems->el_nodes[i][j]]);
    }
//    file1.close();
// qWarning("============= elems ========");
}

void Internal::read_res(QString fname)
{
    int count = 0;
    if ( INT_nn==0 ) INT_nn = 8;
  qWarning(" read_res, INT_nn=%d", INT_nn);
    QFile file(fname);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       qWarning("===== ERROR  file %s not found", fname.toAscii().data());
       return;
    }
    QTextStream in(&file);
    if ( this->results->n_results>0 ) {
       for (int k=0; k<results->n_results; k++) delete[] results->res[k];
       delete[] results->res;
       delete[] results->ires;
       delete[] results->r_min_;
       delete[] results->r_max_;
    }
    QString line = in.readLine();
    QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    results->n_results = INT_nr = fields.takeFirst().toInt();
  qWarning(" read_res, INT_nr=%d", INT_nr);

    results->res = new double*[INT_nn];
    results->ires = new int[INT_nn];
    results->r_min_ = new double[INT_nr];
    results->r_max_ = new double[INT_nr];
//    count = 0;
    for (unsigned int k=0; k<INT_nn; k++) results->ires[k] = -1;
       qWarning("===== ++++++++++++++ =============");

    while ( !in.atEnd() ) {
//       double temp[17];
       QString line = in.readLine();
       QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
//       int nf = fields.size(); 
       int j = fields.takeFirst().toInt();
       results->ires[count] = j-1;
//     qWarning("===== +++   j=%d", j);
       results->res[count] = new double[INT_nr];
       results->res[count][0] = fields.takeFirst().toDouble();
       results->res[count][1] = fields.takeFirst().toDouble();
       results->res[count][2] = fields.takeFirst().toDouble();
       results->res[count][3] = fields.takeFirst().toDouble();
//     qWarning("===== +++   j=%d", j);
       line = in.readLine();

       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
       results->res[count][4] = fields.takeFirst().toDouble();
       results->res[count][5] = fields.takeFirst().toDouble();
       results->res[count][6] = fields.takeFirst().toDouble();

       line = in.readLine();

       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
       results->res[count][7] = fields.takeFirst().toDouble();
       results->res[count][8] = fields.takeFirst().toDouble();
       results->res[count][9] = fields.takeFirst().toDouble();
       results->res[count][10] = fields.takeFirst().toDouble();
       results->res[count][11] = fields.takeFirst().toDouble();
       results->res[count][12] = fields.takeFirst().toDouble();

       line = in.readLine();

       fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
       results->res[count][13] = fields.takeFirst().toDouble();
       results->res[count][14] = fields.takeFirst().toDouble();
       results->res[count][15] = fields.takeFirst().toDouble();
       results->res[count][16] = fields.takeFirst().toDouble();

       line = in.readLine();
       ++count;
//       out<<"i="<<i<<" ---  "<<ax<<"  "<<ay<<"  "<<az<<"\n";
    }
    for (unsigned int k=0; k<INT_nr; k++) {
       double rmin = 1.e15;
       double rmax = -1.e15;
       for (unsigned int i=0; i<INT_nn; i++) {
           double temp = results->res[i][k];
           if ( temp<rmin ) rmin = temp;
           if ( temp>rmax ) rmax = temp;
       }
       results->r_min_[k] = rmin;
       results->r_max_[k] = rmax;
  qWarning("min - max = %g - %g", results->r_min_[k], results->r_max_[k]);
    }

//    file1.close();
 qWarning("================   count=%d", count);
}

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
           for (int an=0; an<this->actcrcell->nchem; an++) {
              QVector3D hic = QVector3D(double(i)+this->actcrcell->cryst[an].x(),
                                        double(j)+this->actcrcell->cryst[an].y(),
                                        double(k)+this->actcrcell->cryst[an].z());
              this->atoms->coord[m] = matvecmult(this->actcrcell->c2o, hic);
              this->atoms->marked[m] = true;
              this->atoms->type[m++] = this->actcrcell->cr_kind[an];
           }
        }
     }
  }
  return m;
}

int Internal::lattice2(double sx, double sy, int nz)
{
  int m = 0;
  double sg = sin(constant::deg2rad*this->actcrcell->gamma);
  int nx = int((sx+2.*sy*sqrt(1.-sg*sg))/this->actcrcell->a)+1;
  int ny = int(sy/(this->actcrcell->b*sg))+1;
  QVector3D coord0[20];
  for (int an=0; an<this->actcrcell->nchem; an++)
      coord0[an]= matvecmult(this->actcrcell->c2o, this->actcrcell->cryst[an]);

  for (int k=0; k<nz; k++) {
     for (int j=0; j<ny; j++) {
        for (int i=0; i<nx; i++) {
           QVector3D hic = matvecmult(this->actcrcell->c2o,
                                  QVector3D(double(i), double(j), double(k)));
           if ( hic.x()>=0.&& hic.x()<sx && hic.y()>=0.&& hic.y()<sy ) {
              for (int an=0; an<this->actcrcell->nchem; an++) {
                 this->atoms->coord[m] = hic + coord0[an];
                 this->atoms->marked[m] = true;
                 this->atoms->type[m++] = this->actcrcell->cr_kind[an];
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
  Mat9d mat, rot_in;
  if ( sw==1 ) 
     swb = parse_miller(result_text);
  else swb = (parse_miller(result_text))  && (parse_core(result_text2));
//  if ( !VS->rotChanged ) {
//    qWarning("-----------processMiller  rot not changed");
//    return;
//  }
  if ( swb ) {
     rot_in = this->rot_inv;
     this->rot_tensor = rotation_tensor();
//     this->rot_inv = invert(this->rot_tensor);
     this->rot_inv = transpose(this->rot_tensor);
//     this->mat = matmult(this->rot_tensor, rot_in);

  qWarning("######   rot_tensor    ###### det = %g", determinant(this->rot_tensor));
qWarning ("rot 0 = %g %g %g", this->rot_tensor[0], this->rot_tensor[1], this->rot_tensor[2]); 
qWarning ("rot 1 = %g %g %g", this->rot_tensor[3], this->rot_tensor[4], this->rot_tensor[5]); 
qWarning ("rot 2 = %g %g %g", this->rot_tensor[6], this->rot_tensor[7], this->rot_tensor[8]); 

//        emit SIG_prepareRotation();
  }
}

Mat9d Internal::rotation_tensor() // obliczanie tensora obrotu sztywnego na podstawie wskaźników Millera 
{
  Mat9d result;
//  qreal a = actcrcell->a;
//  qreal c = actcrcell->c;
  qreal h = qreal(indMiller[0]);
  qreal k = qreal(indMiller[1]);
  qreal l = qreal(indMiller[2]);
  qreal u = qreal(indMiller[3]);
  qreal v = qreal(indMiller[4]);
  qreal w = qreal(indMiller[5]);

  QVector3D hkl_xyz = matvecmult(actcrcell->c2o,QVector3D(qreal(indMiller[0]),qreal(indMiller[1]),qreal(indMiller[2])));;
  QVector3D uvw_xyz = matvecmult(transpose(actcrcell->o2c), QVector3D(qreal(indMiller[3]),qreal(indMiller[4]),qreal(indMiller[5])));;
//  printVec(QString("hkl "), hkl_xyz);
//  printVec(QString("uvw "), uvw_xyz);
  qreal norm_hkl = hkl_xyz.length();
  qreal norm_uvw = uvw_xyz.length();
//  qreal norm_s = sqrt( (h*h-h*k+k*k)*a*a + c*c*l*l );
//  qreal norm_m = sqrt( u*u/(a*a)+1./3.*(u+v+v)*(u+v+v)/(a*a)+w*w/(c*c) );
// qWarning ("dif1=%g,  dif2=%g",  norm_hkl-norm_s, norm_uvw-norm_m);

  QVector3D m = uvw_xyz.normalized();
  QVector3D s = hkl_xyz.normalized();
  QVector3D mxs = QVector3D::crossProduct(m, s).normalized();
  QVector3D mxsxm = QVector3D::crossProduct(mxs, m).normalized();
//  printVec(QString("m before rotation"), m);
//  printVec(QString("s before rotation"), s);
  result = Mat9d( mxsxm.x(),mxsxm.y(),mxsxm.z(), 
                  mxs.x(),  mxs.y(),  mxs.z(), 
                  m.x(),    m.y(),    m.z());
//  printVec(QString("m after rotation"), matvecmult(result, m));
//  printVec(QString("s after rotation"), matvecmult(result, s));
  return result;
}

/*
void Internal::compute_rotation_tensor()
{
//  double fraction = double(fr_up)/double(fr_down);
  double a = this->actcrcell->a;
  double c = this->actcrcell->c;

  double h = double(indMiller[0]);
  double k = double(indMiller[1]);
  double l = double(indMiller[2]);
  double u = double(indMiller[3]);
  double v = double(indMiller[4]);
  double w = double(indMiller[5]);
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
  QList<int> mil;
  mil << 0 << 0 << 0 << 0;
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
       indMiller[0] = mil.at(0);
       indMiller[1] = mil.at(1);
       indMiller[2] = mil.at(2);
    } else {
       qWarning("ERROR - BAD MILLER INDICES 1");
       return false;
    }
    line2 = line1.mid(i_left2+1, i_right2-i_left2-1).trimmed();
    if ( internal_miller(line2, 2, mil) ) {
       indMiller[3] = mil.at(0);
       indMiller[4] = mil.at(1);
       indMiller[5] = mil.at(2);
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
     for ( int i=0; i<actcrcell->ncores; i++) {
        if ( line1==actcrcell->co_name[i] ) {
             nd = i;
             this->act_core = line1;
             break;
        }
     }

     if ( !(this->act_core.isEmpty()) ) 
       {
	 actdisl->dislocation_core = actcrcell->core[nd];
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
      //    qWarning("act_core=%s, ncores=%d", act_core.toAscii().data(), this->actcrcell->ncores);
      for ( int ind=0; ind<this->actcrcell->ncores; ind++) {
	if ( act_core==actcrcell->co_name[ind] ) {
	  qWarning ("ERROR- act_core existing for ind=%d (%s - %s)", ind, 
		    actcrcell->co_name[ind].toAscii().data(), act_core.toAscii().data());
	  return false;
	}
      } // for
      int nc = ++(actcrcell->ncores);
      actdisl->dislocation_core = actcrcell->core[nc] = QVector3D(oth_disl1, oth_disl2, oth_disl3);
      if ( act_core.isEmpty() ) actcrcell->co_name[nc].sprintf("core_nr_%d", nc);
      else                      actcrcell->co_name[nc] = act_core;
    }
  return true;
}

bool Internal::internal_miller(QString line2, int which, QList<int> &miller_indices)
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
       for (int i=0; i<numbmill; i++) miller_indices[i] = fields.at(i).toInt();
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
             miller_indices[numbmill++] = minus? -n : n; 
             minus = false;
          }
          if ( ch==QChar('.') && numbmill==2 ) miller_indices[numbmill++] = -miller_indices[0]-miller_indices[1];
       }
       if ( numbmill<3 || numbmill>4 ) {
          qWarning("ERROR - BAD NUMBER OF MILLER INDICES 2");
          return false;
       }
   }
   if ( numbmill==4 ) {
      if ( (miller_indices[0]+miller_indices[1])!=-miller_indices[2] ) {
          qWarning("ERROR BAD SUM - %d %d %d\n", miller_indices[0], miller_indices[1], miller_indices[2]);
          miller_indices[2] = -miller_indices[0]-miller_indices[1];
//          return false;
      }
      if ( which==1 ) {
         miller_indices[0] -= miller_indices[2];
         miller_indices[1] -= miller_indices[2];
      }
      miller_indices[2] = miller_indices[3];
   }
//    qWarning("miller: %d, %d, %d\n", miller_indices[0], miller_indices[1], miller_indices[2]);
   return true;
}

/*
bool Internal::internal_miller (QString line2, int which, 
				Int4 &miller_indices)
{
  // Put this as zero for now, this will be filled in later depending
  // on what format the line2 comes in.
  int n_miller_indices = 0;
  
  QStringList fields;
  
  if (line2.contains(",") || line2.contains(" ")) 
    {
      fields = line2.split (QRegExp("[ ,]"), QString::SkipEmptyParts);
      
      if (fields.size () != 4) 
	{
	  qWarning("ERROR - BAD NUMBER OF MILLER INDICES 1");
	  return false;
	}
      
      for (int i=0; i<n_miller_indices; ++i) 
	miller_indices[i] = fields.at(i).toInt();
    } 
  else 
    {
      bool minus = false;
      
      for (int i=0; i<line2.size(); ++i) 
	{
	  QChar ch = QChar (line2.at(i++));
	  
	  // If there contains a negative Miller index:
	  if (ch==QChar('-')) 
	    {
	      minus = true;
	      continue;
	    }
	  
	  // Store this Miller index in miller_indices with the
	  // correct sign:
	  if (ch.isDigit()) 
	    {
	      int n = ch.digitValue();
	      miller_indices[n_miller_indices++] = minus ? -n : n; 
	      minus = false;
	    }
	  
	  // No idea what this does....
	  if (ch==QChar('.') && n_miller_indices==2) 
	    miller_indices[n_miller_indices++] = 
	      -miller_indices[0]-miller_indices[1];
	}
      
      if (n_miller_indices!=4) 
	{
	  qWarning("ERROR - BAD NUMBER OF MILLER INDICES 2");
	  return false;
	}
    }
  
  if (n_miller_indices == 4) 
    {
      if ((miller_indices[0]+miller_indices[1]) != -miller_indices[2]) 
	{
	  qWarning("ERROR BAD SUM - %d %d %d\n", 
		   miller_indices[0], miller_indices[1], miller_indices[2]);
	  
	  // This is very dangerous! We should not "correct" the user
	  // input; they have that responsibility...
	  miller_indices[2] = -miller_indices[0]-miller_indices[1];
	}
      
      if (which == 1) 
	{
	  miller_indices[0] -= miller_indices[2];
	  miller_indices[1] -= miller_indices[2];
	}
      miller_indices[2] = miller_indices[3];
    }
  
  return true;
}
*/

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
*/

