
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

                                 /* TODO: The structure params does
				    what? */
				 /*
				  * The structure params holds
				  * information needed to perform solving nonlinear equations by GSL routines
				  */
struct params 
{
  double be;
  double bz;
  double u0x;
  double u0y;
  double u0z;
};

namespace vecds
{
  struct AtomProperties
  {
    
    AtomProperties ()
    {}
    
    float a_rad1[125]; 
    float a_rad2[125];
    
    float atom_red[125]; 
    float atom_green[125]; 
    float atom_blue[125];
  
    QString namea[125];
  };
}

namespace vecds
{
  struct miller
  {
    double fraction;
    double indices[6];
  };
}

namespace vecds
{

  struct Settings
  {
    
    Settings()
    {}
    
    float fov;
    double mfact;
    bool vis[15];

                                  /**
				   * Variable poining to the RGB
				   * colour spectrum. The integer
				   * vector stores RGB numbers.
				   */
    vecds::IntVector<3> colour_spectrum[12];
  };
}



namespace vecds
{

                                 /**
				  * The structure Atoms holds
				  * information needed to describe the
				  * location and type of an atom in a
				  * matrix and resultant displacement field /new coordinates/
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
				  * Number of atoms in this crystal
				  * structure.
				  */
    unsigned int n_atoms; 

                                 /**
				  * Number of bonds between atoms.
				  */
    unsigned int n_bonds;

                                 /**
				  * Array holding a set of coordinates 
				  */
    glm::dvec3 *coordinates;

                                 /* TODO: WHat are these?  - incremental and total displacement */
    glm::dvec3 *du;
    glm::dvec3  *u;
    
                                 /**
				  * An array of integers describing
				  * the atom type.
				  */
    unsigned int *atom_type;

                                 /**
				  * An array of two-integers that
				  * describe the bonds between two
				  * atoms.
				  */
    vecds::IntVector<2> *atom_bond;
//    int *atom_bond1;
//    int *atom_bond2;
  };

}                                /* namespace vecds */


namespace vecds
{


                                 /**
				  * A structure that dewxcribes the
				  * crystallographic make-up of a
				  * crystal system.
				  */
  struct CrystalStructure
  {
                                 /**
				  * This particular crystal structure
				  * name.
				  */
    QString structure_name;

                                 /**
				  * Crystallographic lengths. 
				  */
    double a, b, c;

                                 /**
				  * Crystallographic angles. 
				  */
    double alpha, beta, gamma;

                                 /* TODO: WHat are these things?  transformation matrices from Crystallographic to Orthogonal - C2O and inverse - O2C*/
    glm::dmat3 C2O;
    glm::dmat3 O2C;

                                 /**
				  * The number of material / chemical species.
				  */
    unsigned int n_materials;

                                 /**
				  * The number of dislocation cores.
				  */
    unsigned int n_cores;

                                 /**
				  * Type of crystal denoted by an
				  * array of unsigned integers
				  */
    unsigned int crystal_type[20];

                                 /* TODO: WHat are these things? */
    QString co_name[30];         /* table of introduced names of dislocation cores */

    glm::dvec3 cryst[20];        /* crystallographic coordinates of each atom in unit cell */
    glm::dvec3 core[30];         /* crystallographic positions of dislocation cores with corrresponding 'co_name' in unit cell */
  };

}                                /* namespace vecds */



namespace vecds
{

  struct Dislocations
  {
    Dislocations ()
    {}
  
    glm::dvec3 rrr; // współrz. punktu przkazanego z viewera (Mviewer::SIG_actPoint, Mainwindow::SL_actPoint
  
                                 /**
				  * Vector defining the relative
				  * coordinates of the "top" of a line
				  * dislocation.
				  */
    glm::dvec3 p1; 
  
                                 /**
				  * Vector defining the relative
				  * coordinates of the "bottom" of a
				  * line dislocation.
				  */
    glm::dvec3 p2;
  
    glm::dvec3 cd; // wpółrzędne dyslokacji brane do obliczeń /po znalezieniu punktu i0/
  
                                 /**
				  * Three-dimensional vector that
				  * describes the orientation of the
				  * Burgers vector of a dislocation.
				  */
    glm::dvec3 burgers_vector;

                                 /**
				  * Three-dimensional vector that
				  * describes the position of the core
				  * of a dislocation.
				  */
    glm::dvec3 dislocation_core;

                                 /**
				  * A 3x3 Cartesian rotation tensor.
				  */
    glm::dmat3 rotation_tensor;

                                 /**
				  * Text string that identifies the
				  * name of this particular Burgers
				  * vector.
				  */
    QString burgers_name;

                                 /**
				  * Text string that identifies this
				  * particular dislocation core name
				  */
    QString core_name;

    int i0; // znaleziony punkt
  };

                                 /**
				  * Inline conversion of vector types.
				  */
  inline glm::dvec3 to_dvec3 (QVector3D x)  // for me 'to_dvec3' is a better name
  {
    return glm::dvec3 (x.x (), x.y (), x.z ()); 
  }
  
  inline glm::dvec2 to_dvec2 (QVector2D x)
  {
    return glm::dvec2 (x.x (), x.y ()); 
  }
  
                                 /**
				  * Inline conversion of vector types.
				  */
  inline QVector3D to_QV3 (glm::dvec3 x)
  {
    return QVector3D (x.x, x.y, x.z); 
  }

  inline QVector2D to_QV2 (glm::dvec2 x)
  {
    return QVector2D (x.x, x.y); 
  }


}                                /* namespace vecds */

#endif

