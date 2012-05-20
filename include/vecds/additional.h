
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010)
// Affiliation: Polish Academy of Sciences
//
// Copyright (C) 2010 The vecds authors
//
// This  program is  free  software: you  can  redistribute it  and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with this program.  If not, see
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

                                 /* TODO: The structure params does
				    what? */
struct params 
{
  double be;
  double bz;
  double u0x;
  double u0y;
  double u0z;
};



                                 /* The structure Atoms holds
				    information needed to describe the
				    location and type of an atom in a
				    matrix. */
struct Atoms
{

                                 /* Empty constructor. */
  Atoms ()
  {
    n_atoms = 0;
    n_bonds = 0; 
  }

                                 /* Number of atoms. */
  unsigned int n_atoms; 

                                 /* Number of bonds between atoms. */
  unsigned int n_bonds;

                                 /* Array holding a set of coordinates */
  QVector3D *coordinates;

                                 /* TODO: WHat are these? */
  glm::dvec3 *coord1;
  glm::dvec3 *du;
  QVector3D  *u;

                                 /* An array of integers describing
				    the atom type */
  unsigned int *atom_type;

                                 /* An array of two-integers that
				    describe the bonds between two
				    atoms. */
  Int2 *atom_bond;
};


struct AtomsProperties
{

  AtomsProperties ()
  {}
  
  float a_rad1[125]; 
  float a_rad2[125];
  
  float atom_red[125]; 
  float atom_green[125]; 
  float atom_blue[125];
  
  QString namea[125];
};

struct Settings
{

  Settings()
  {}
  
  float fov;
  double mfact;
  bool vis[15];

                                 /* Variable poining to the RGB colour
				    spectrum. */
  Int3 colour_spectrum[12];
  
};

                                 /* A structure that dewxcribes the
				    crystallographic make-up of a
				    crystal system. */
struct CrystalStructure
{
                                 /* The structure name */
  QString structure_name;
  double a, b, c;
  double alpha, beta, gamma;
  Mat9d c2o;
  Mat9d o2c;
  glm::dmat3 C2O;
  glm::dmat3 O2C;

                                 /* The number of materials / chemical
				    species */
  unsigned int n_materials;

                                 /* The number of dislocation cores */
  unsigned int n_cores;


  int cr_kind[20];
  QVector3D cryst[20];
  QString co_name[30];
  QVector3D core[30];
};

struct Dislocations
{
  Dislocations ()
  {}
  
  QVector3D rrr; // współrz. punktu przkazanego z viewera (Mviewer::SIG_actPoint, Mainwindow::SL_actPoint
  
                                 /* Vector defining the relative
				    coordinates of the "top" of a line
				    dislocation. */
  QVector3D p1; 
  
                                 /* Vector defining the relative
				    coordinates of the "tbottom" of a
				    line dislocation. */
  QVector3D p2;
  
  QVector3D cd; // wpółrzędne dyslokacji brane do obliczeń /po znalezieniu punktu i0/
  
                                 /* Three-dimensional vector that
				    describes the orientation of the
				    Burgers vector of a
				    dislocation. */
  QVector3D burgers_vector;

                                 /* Three-dimensional vector that
				    describes the position of the core
				    of a dislocation. */
  QVector3D dislocation_core;

                                 /* A 3x3 Cartesian rotation tensor. */
  glm::dmat3 rotation_tensor;

                                 /* Text string that identifies a
				    Burgers vector. */
  QString burgers_name;

                                 /* Text string that identifies a
				    dislocation core name */
  QString core_name;

  int i0; // znaleziony punkt
};
#endif
