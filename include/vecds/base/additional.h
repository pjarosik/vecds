// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010), Toby D. Young
// (2012).
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

#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multiroots.h>

                                 /* vecds includes */
//#include <./integer_vector.h>
#include <vecds/numerics/integer_vector.h>
#include <qglobal.h>
#include <QVector>
#include <QVector3D>

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
    n_marked = 0;
  }

  /**
   * Number of atoms.
   */
  unsigned int n_atoms; 

  /**
   * Number of bonds between atoms.
   */
  unsigned int n_bonds;

  /**
   * Number of marked atoms.
   */
  unsigned int n_marked;

  /**
   * Atomic coordinates \f$(x,y,z)\f$.
   */
  QVector3D* coord;

  /**
   * Information whether atom is marked .
   */
  bool* marked;

  /**
   * Atomic displacements \f$(x,y,z)\f$.
   */
  QVector3D* u;
  QVector3D* add;
  QVector3D* add1;
  int* type;
  QVector<int> bonds;
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
   * Variable pointing to the RGB colour spectrum.
   */
  QColor colour_spectrum[12];
  
};


struct Elements
{
  /**
   * Total number of elements used in this context.
   */
  unsigned int n_elements;

  /**
   * Total number of nodes on this element used in this context.
   */
  unsigned int* n_nodes;

  /**
   * Type of elemt used in this context.
   */
  int* type;

  unsigned int** el_nodes;
  
  /**
   * Constructor.
   */
  Elements ()
  {
    n_elements = 0;
    el_nodes   = 0;
  }
  
  /**
   * Virtual destructor.
   */
  ~Elements ()
  {}
};


struct Results
{

  /**
   * Constructor.
   */
  Results()
  {
    n_results = 0;
  }
  
  /**
   * Destructor.
   */
  ~Results()
  {
  }
  
  /**
   * Number of resultsin this context.
   */
  int n_results;

  int* ires;
  double** res;
  double* r_max_;
  double* r_min_;
  
};

struct Nodes
{
  /**
   * Number of nodes.
   */
  unsigned int n_nodes;

  /**
   * Three-dimensional coordinates of a node.
   */
  QVector3D* coords;
  
  /**
   * Constructor.
   */
  Nodes()
  {
    n_nodes  = 0;
    coords   = 0;
  }
  
};

struct Face
{

  /**
   * Number of nodes on this face.
   */
  int n_nodes;

  int n_extlines, n_intlines, n_triangles;
  Int4 f_quad;
  int* f_nodes;

  /**
   * ?
   */
  QVector<int>* f_triangles;
  int tex;
  int nr_elem;
  int nr_face;

  QVector<int>* f_extlines;
  QVector<int>* f_intlines;

  Face()
  {
    tex = 0;
  }
  
};


struct E0
{
  /**
   * Given name for this element.
   */
  QString e0_name;
  int e0_n_nodes;
  int e0_n_faces;
  Face* e0_faces;
};

/**
 * The structure FiniteElementMesh contains the data required to
 * specify a finite element mesh.
 */
struct FiniteElementMesh
{
  QVector3D e_min_, e_max_;
  
  /**
   * Number of elements on this mesh.
   */
  int n_elements;
  
  /**
   * Number of nodes on this mesh.
   */
  int n_nodes;
  
  /**
   * Number of faces on this mesh.
   */
  int n_faces;
  
  /**
   * Pointer to these nodes.
   */
  Nodes* nodes;

  /**
   * Pointer to these elements.
   */
  Elements* elements;
  
  /**
   * Constructor for a finite element mesh.
   */
  FiniteElementMesh ()
  {
    n_elements = 0;
    n_nodes    = 0;
    n_faces    = 0;
  }

  /**
   * Destructor.
   */
  ~FiniteElementMesh () 
  {}
};

struct CrystalStructure
{
  QString struct_name;
  double a, b, c;
  double alpha, beta, gamma;
  Mat9d c2o;
  Mat9d o2c;
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

  /**
   * Text string that identifies a Burgers vector.
   */
  QString burgers_name; // tekstowy zapis wektora Burgersa

  /**
   * Text string that identifies a dislocation core name
   */
  QString core_name; // tekstowy zapis nazwy rdzenia

  int i0; // znaleziony punkt
};
#endif
