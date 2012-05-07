
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

#ifndef ADDITIONAL_H
#define ADDITIONAL_H

#include <qglobal.h>

#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multiroots.h>

using namespace std;



struct params {
  double be;
  double bz;
  double u0x;
  double u0y;
  double u0z;
};


/**
 * The structure Atoms holds information needed to describe the
 * location and type of an atom in a matrix.
 */

struct Atoms
{

  /**
   * Constructor.
   */
  Atoms ()
  {
    n_atoms = 0;
    n_bonds = 0; 
  }

  /**
   * Number of atoms.
   */
  unsigned int n_atoms; 

  /**
   * Number of bonds between atoms.
   */
  unsigned int n_bonds;

  QVector3D* coord;
  glm::dvec3* coord1;
  glm::dvec3* du;
  QVector3D* u;
  int*   type;
  Int2*  bonds;
};


struct AtomsProperties
{

  /**
   * Constructor.
   */
  AtomsProperties()
  {
  }
  
  float a_rad1[125]; 
  float a_rad2[125];
  
  float atom_red[125]; 
  float atom_green[125]; 
  float atom_blue[125];
  
  QString namea[125];
  
};

struct Settings
{

  /**
   * Constructor.
   */
  Settings()
  {
  }
  
  float fov;
  double mfact;
  bool vis[15];

  /**
   * Variable poining to the RGB colour spectrum.
   */
  Int3 colour_spectrum[12];
  
};



struct CrystalStructure
{
  QString struct_name;
  double a, b, c;
  double alpha, beta, gamma;
  Mat9d c2o;
  Mat9d o2c;
  glm::dmat3 C2O;
  glm::dmat3 O2C;
  int nchem, ncores;
  int cr_kind[20];
  QVector3D cryst[20];
  QString co_name[30];
  QVector3D core[30];
};

struct Dislocations
{
  /**
   * Constructor.
   */
  Dislocations ()
  {}
  
  QVector3D rrr; // współrz. punktu przkazanego z viewera (Mviewer::SIG_actPoint, Mainwindow::SL_actPoint
  
  /**
   * Vector defining the relative coordinates of the "top" of a line
   * dislocation.
   */
  QVector3D p1; // "góra"  zaznaczenia dyslokacji
  
  /**
   * Vector defining the relative coordinates of the "bottom" of a
   * line dislocation.
   */
  QVector3D p2; // "dół" zaznaczenia dyslokacji
  
  QVector3D cd; // wpółrzędne dyslokacji brane do obliczeń /po znalezieniu punktu i0/
  
  /**
   * Three-dimensional vector that describes the orientation of the
   * Burgers vector of a dislocation.
   */
  QVector3D burgers_vector; // wektor Burg. dla dyslokacji

  /**
   * Three-dimensional vector that describes the position of the core
   * of a dislocation.
   */
  QVector3D dislocation_core; // współrz. rdzenia

  /**
   * A \f$3\times3\f$ Cartesian rotation tensor.
   */
//  Mat9d rotation_tensor; // tensor obrotu dla układu z którego były odzczytane współrz. dyslokacji
  glm::dmat3 rotation_tensor; // tensor obrotu dla układu z którego były odzczytane współrz. dyslokacji

  /**
   * Text string that identifies a Burgers vector.
   */
  QString burgstr; // tekstowy zapis wektora Burgersa

  /**
   * Text string that identifies a dislocation core name
   */
  QString core_name; // tekstowy zapis nazwy rdzenia

  int i0; // znaleziony punkt
};
#endif
/*
#include "prog1u.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multiroots.h>

using namespace std;
*/
