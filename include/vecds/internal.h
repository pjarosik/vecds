
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

#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>



                                 // vecds base headers
#include <vecds/base/config.h>
#include <vecds/base/additional.h>
#include <vecds/base/constant.h>
#include <vecds/base/function.h>

                                 // forward declarations
class MainViewer;
class MainWindow;

/**
 * This is a class called Internal.
 */
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


                                 // general types needed...
  vecds::CrystalStructure *actcrstr;
  vecds::CrystalStructure *crstr;
  vecds::Atoms            *atoms;
  vecds::AtomProperties   *ap;
  vecds::Dislocations     *actdisl;
  vecds::Dislocations     *disl;
  vecds::Settings         *set0;

  bool choice;
  
  int numbcrstr;
  
  double be;
  double bz;
  
  QString img_loaded;
  QString atoms_loaded;
  QString act_disl;
  QString act_core;
  QString act_mill;
  vecds::miller mil;
  
  glm::dvec3 a_min_;
  glm::dvec3 a_max_;
  glm::dvec3 min_;
  glm::dvec3 max_;
  glm::dvec3 cent_;
  glm::dvec3 axeX;
  glm::dvec3 axeY;
  glm::dvec3 axeZ;

  glm::dvec3 invbox[8];
  
  QPixmap img;
  
  struct params p;

                                  /**
				   * Read settings from disk.
				   */
  void read_settings ();

                                  /**
				   * Write a complete vector to
				   * <code>std::cout</code>.
				   */
  void write_vector (const std::string &str, 
		     const glm::dvec3  &V);

                                  /**
				   * Write a complete matrix to
				   * <code>std::cout</code>.
				   */
  void write_matrix (const std::string &str, 
		     const glm::dmat3  &M);


                                  /**
				   * Returns a boolean somehow...
				   */
  bool internal_miller (std::string  line2, 
			int          which, 
			int         *miller_indices);

  // ===================================================
  bool sliderMove;
  int Mode;
  double sliderValue;
  // ===================================================

  bool visible[10];
  
  int ndisl;
    
  double mfactor;
  double rad_scene;
  double fraction;
  
  QVector3D actPoint;

  glm::dmat3 rot_tensor, rot_inv;
  
  std::string stripBlanks (std::string StringToModify);

  int identify(const std::string s1, 
	       const int         size, 
	       const std::string words[]);

  std::vector<std::string> tokenize (const std::string &str, 
				     std::string        del);



  int toInt (std::string word);

  void init_atoms ();
  void init_structures ();
  void read_alc_xyz (QString namea);
  void read_img (QString iname);


  void minmax1 (double *vec, 
		int numb, 
		double &vmin, 
		double &vmax);

  void minmax3 (glm::dvec3 *vec, 
		int numb, 
		glm::dvec3 &vmin, 
		glm::dvec3 &vmax);

  void processMiller (int sw, 
		      QString rtext, 
		      QString rtext2="");

  int which_atom (QString nam_a);

  int lattice (int, int, int);
  int lattice2 (double, double, unsigned int);

  double read_fraction (std::string line);

  vecds::miller parse_miller (std::string line);
  bool parse_core (QString line);



  
  void compute_rotation_tensor();
  
  void do_atoms_rotation (glm::dmat3 r_tens, glm::dvec3 vec);
  void do_invis_rotation (glm::dmat3 r_tens, glm::dvec3 vec);
  void do_signes_rotation (glm::dmat3 r_tens, glm::dvec3 vec);
  void do_axis_rotation (glm::dmat3 r_tens);
  
  void calc_disloc (int nr_atom, int d_num);
  void calc_disl0 ();

                                /* Does something with atom numbers. */
  int atomize (const glm::dvec3 point, 
	       const unsigned int atom_number);

  void SL_singleDisl (glm::dvec3 r);
  void addDisplacements ();
  void newdisl (unsigned int n_a, bool sw_iter);

  void saveAtoms (QString sname);

  glm::dvec3 mixed_u(int i, glm::dvec3 rotdist, double be, double bz);

  glm::dmat3 mixed_beta(int i, glm::dvec3 rotdist, double be, double bz);
  
  


 private:

                                 /* This is the path to internal files
				    needed by vecds to run
				    correctly. */
  QString path; 




};

#endif 

