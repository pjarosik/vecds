
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010)
// Affiliation: Polish Academy of Sciences
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


#ifndef INTERNALDATA_H
#define INTERNALDATA_H

#include "algebra.h"
#include "additional.h"
#include <qglobal.h>

class MaimViewer;
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
  CrystalStructure *actcrstr;
  CrystalStructure *crstr;
  int numbcrstr;
  Dislocations *actdisl;
  
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
  
  Dislocations *disl;
  int ndisl;
  
  int  indMiller[6], oldMiller[6];
  double fraction;
  
//  Mat9d rot_tensor, rot_inv;
  glm::dmat3 rot_tensor, rot_inv;
/*  
  struct params {
    double be;
    double bz;
    double u0x;
    double u0y;
    double u0z;
  };
*/
  bool visible[10];
  double mfactor;
  Settings *set0;
  
  void init_atoms();
  void init_structures();
  void read_alc_xyz(QString namea);
  void read_img(QString iname);
  void read_settings();
  int which_atom(QString nam_a);
  //    void invert33(const double m1[9], double m2[9]);
  //    double det33(const double m1[9]);
  double read_fraction(QString line);
  int lattice(int, int, int);
  int lattice2(double, double, int);
  void minmax3(QVector3D *vec, int numb, QVector3D &vmin, QVector3D &vmax);
  void minmax1(double *vec, int numb, double &vmin, double &vmax);
  
  void processMiller(int sw, QString rtext, QString rtext2="");
  bool parse_miller(QString line);
  bool parse_core(QString line);
  bool internal_miller(QString line2, int which, Int4 &mil);
  
  void compute_rotation_tensor();
  
  void do_atoms_rotation(Mat9d r_tens, QVector3D vec);
  void do_invis_rotation(Mat9d r_tens, QVector3D vec);
  void do_signes_rotation(Mat9d r_tens, QVector3D vec);
  void do_axis_rotation(Mat9d r_tens);
  
//  int Love_function(const gsl_vector *x, void *par, gsl_vector *result_funct);
//  int Beta_function(const gsl_vector *x, void *par, gsl_matrix *jac);
//  int Love_fdf(const gsl_vector *x, void *par, gsl_vector *result_funct, gsl_matrix *jac);
  
  void calc_disloc(int nr_atom, int d_num);
  void calc_disl0();
  int atomize(QVector3D point, int wh_at);
  void SL_singleDisl(QVector3D r);
//  void mixed_u(int i);
  void newdisl(int n_a, bool sw_iter);
  //    void mixed_u1(int disl_num, int i);
  //    void mixed_u2(int disl_num, int i);
  
  bool eqMiller(int m1[6], int m2[6]);
  void saveAtoms(QString sname);

  glm::dvec3 mixed_u(int i, glm::dvec3 rotdist, double be, double bz);
  glm::dmat3 mixed_beta(int i, glm::dvec3 rotdist, double be, double bz);



};
#endif 

