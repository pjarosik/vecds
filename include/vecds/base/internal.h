
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
#ifndef INTERNAL_H
#define INTERNAL_H

#include <QTranslator>
#include <QVector3D>
#include <QFile>
#include <qglobal.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

                                 // GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

                                 // vecds base headers
#include <vecds/base/config.h>
#include <vecds/base/additional.h>
#include <vecds/base/constant.h>
#include <vecds/numerics/function.h>

                                 // forward declarations
class MainViewer;
class MainWindow;

class Internal
{
 public:
  Internal();
  ~Internal();
  
  friend class MainViewer;
  friend class MainWindow;
  
 private:
  
  /**
   * The current directory being used.
   */
  QString current_dir;
  Atoms* atoms;
  
  AtomsProperties *ap;
  CrystalStructure *actcrcell;
  CrystalStructure *crstr;
  int numbcrstr;
  Dislocations *actdisl;
  
  QString img_loaded;
  QString atoms_loaded;
  QString res_loaded;
  QString fems_loaded;
  
  QString act_disl;
  QString act_core;
  QString act_mill;
  bool choice;
  bool showU;
  
  QPixmap img;
  
  
  Nodes* nodes;
  Elements* elems;
  Face* faces;
  int numbFaces;
  Results* results;
  
  E0 c27;
  
  QVector3D e_min_, e_max_;
  QVector3D a_min_, a_max_;
  QVector3D min_, max_, cent_;
  QVector3D invbox[8];
  QVector3D axeX, axeY, axeZ;
  // ===================================================
  bool sliderMove;
  int Mode;
//  double sliderValue;
  int sliderValue;
  // ===================================================
  QVector3D actPoint;
  double rad_scene;
  
  Dislocations *disl;
  int n_dislocations;
  
  int  indMiller[6], oldMiller[6];
  double fraction;
  
  Mat9d rot_tensor, rot_inv; //, mat;
  
  bool visible[10];
  double mfactor;
  Settings *set0;
  
  unsigned int INT_nn, INT_ne, INT_nr;
  FILE *outputFile;

  // =====================  coordinates   ====================
  /*
    double** results;
    int *ires;
    int numbRes;
    double r_min_[17];
    double r_max_[17];
  */
  // =====================  fems   ====================
  // ===================================================
  
  void init_atoms();
  void init_structures();
  void init_e0();
  void init_c27();

  /**
   * Read a file in as alchemy format.
   */
  void read_alchemy_xyz (QString namea);


  void read_fems (QString fname);
  void read_res (QString resname);
  void read_img (QString iname);

  /**
   * Read in settings: This actually sets the RGB colour to be used
   * that is defined by the user.
   */
  void read_settings ();

  /** 
   * Some kind of integer that tells us which atom we are looking at:
   */
  int which_atom (QString nam_a);

  //    void invert33(const double m1[9], double m2[9]);
  //    double det33(const double m1[9]);
  double read_fraction(QString line);
  int lattice(int, int, int);
  int lattice2(double, double, int);

  /**
   * Find the \f${\rm min}[vec]\f$ and \f${\rm max}[vec]\f$ given all
   * the entries in <tt>vec</tt>.
   */
  void minmax3(QVector3D *vec, int numb, QVector3D &vmin, QVector3D &vmax);

  /**
   * Find the \f${\rm min}[vec]\f$ and \f${\rm max}[vec]\f$ given all
   * the entries in <tt>vec</tt>.
   */
  void minmax (double *vec, int numb, double &vmin, double &vmax);

  /**
   * This function does what?
   */
  bool func (Int4, Int4); 
//  bool func (QVector<int>&, QVector<int>&);

  /**
   * Calculate faces:
   */
  void calc_faces();
  
  void processMiller(int sw, QString rtext, QString rtext2="");
  bool parse_miller(QString line);
  bool parse_core(QString line);

  /**
   * This function does what?
   */
//  bool internal_miller (QString line2, int which, Int4 &mil);
  bool internal_miller (QString line2, int which, QList<int> &miller_indices);
//			QVector<int>&miller_indices); 
  
  Mat9d rotation_tensor();
  
  void do_atoms_rotation(Mat9d r_tens, QVector3D vec);
  void do_invis_rotation(Mat9d r_tens, QVector3D vec);
  void do_signes_rotation(Mat9d r_tens, QVector3D vec);
  void do_axis_rotation(Mat9d r_tens);
  
  void calc_disloc(int nr_atom, int d_num);
  void calc_disl0();
  int atomize(QVector3D point, int wh_at);
  void SL_singleDisl(QVector3D r);
  QVector3D mixed_u(int i);
  void newdisl(int n_a);
  //    void mixed_u1(int disl_num, int i);
  //    void mixed_u2(int disl_num, int i);
  
  bool eqMiller(int m1[6], int m2[6]);
  void extPrint (QString file_name, QVector3D *vec);
  void saveAtoms (QString sname);
  void saveMarkedAtoms(QString sname);
  int cubBox(QVector3D, QVector3D);
  int hexBox(double, double, QVector3D);

};
#endif 

