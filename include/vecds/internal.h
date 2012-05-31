
// -------------------------------------------------------------------
//
// Author: 
//    Jan Cholewinski (2010)
//    Pawel Dluzewski (2010)
//    Toby D. Young (2010, 2012)
//
// Copyright (C) 2010, 2012 The vecds authors
//
// This program is free software: you can redistribute it and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software  Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------


#ifndef INTERNAL_H
#define INTERNAL_H


#include <QTranslator>
#include <QVector3D>
#include <QFile>
#include <qglobal.h>


                                 // vecds base headers
#include <vecds/base/config.h>
#include <vecds/base/integer.h>
#include <vecds/base/matrix.h>
#include <vecds/base/additional.h>
#include <vecds/base/constant.h>
#include <vecds/base/function.h>

                                 // forward declarations
class MaimViewer;
class MainWindow;

class Internal
{
 public:

                                 // constructor
  Internal ();

                                 // destructor
  ~Internal ();
  
                                 // friendly clases
  friend class MainViewer;
  friend class MainWindow;
  


  QString current_dir;

  vecds::CrystalStructure *actcrstr;
  vecds::CrystalStructure *crstr;
  vecds::Atoms            *atoms;
  vecds::AtomProperties   *ap;
  vecds::Dislocations     *actdisl;
  vecds::Dislocations     *disl;
  vecds::Settings         *set0;

  int numbcrstr;

  
  QString img_loaded;
  QString atoms_loaded;
  QString act_disl;
  QString act_core;
  QString act_mill;

  double be, bz;
  bool choice;
  
  QPixmap img;
  
  struct params p;
  QVector3D a_min_, a_max_;
  QVector3D min_, max_, cent_;
  QVector3D invbox[8];
  QVector3D axeX, axeY, axeZ;
  // ===================================================
  bool sliderMove;
  int Mode;
  double sliderValue;
  // ===================================================
  QVector3D actPoint;
  double rad_scene;
  

  int ndisl;
  
  int  indMiller[6], oldMiller[6];
  double fraction;
  
  glm::dmat3 rot_tensor, rot_inv;
  bool visible[10];
  double mfactor;



  
  void init_atoms();
  void init_structures();
  void read_alc_xyz(QString namea);
  void read_img(QString iname);
  void read_settings();
  int which_atom(QString nam_a);

  double read_fraction (QString line);
  int lattice (int, int, int);
  int lattice2 (double, double, unsigned int);
  void minmax3 (QVector3D *vec, int numb, QVector3D &vmin, QVector3D &vmax);
  void minmax1 (double *vec, int numb, double &vmin, double &vmax);
  
  void processMiller(int sw, QString rtext, QString rtext2="");
  bool parse_miller(QString line);
  bool parse_core(QString line);
  bool internal_miller(QString line2, int which, vecds::Int4 &mil);
  
  void compute_rotation_tensor();
  
  void do_atoms_rotation (vecds::Mat9d r_tens, QVector3D vec);
  void do_invis_rotation (vecds::Mat9d r_tens, QVector3D vec);
  void do_signes_rotation (vecds::Mat9d r_tens, QVector3D vec);
  void do_axis_rotation (vecds::Mat9d r_tens);
  
  void calc_disloc(int nr_atom, int d_num);
  void calc_disl0();

                                /* Does something with atom numbers. */
  int atomize (const QVector3D point, 
	       const unsigned int atom_number);

  void SL_singleDisl(QVector3D r);
  void addDisplacements();
  void newdisl(unsigned int n_a, bool sw_iter);

  bool eqMiller(int m1[6], int m2[6]);
  void saveAtoms(QString sname);

  glm::dvec3 mixed_u(int i, glm::dvec3 rotdist, double be, double bz);
  glm::dmat3 mixed_beta(int i, glm::dvec3 rotdist, double be, double bz);

 private:

                                 /* This is the path to internal files
				    needed by vecds to run
				    correctly. */
  QString path; 


};

#endif 

