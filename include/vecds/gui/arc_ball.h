
// -------------------------------------------------------------------
//
// Author: 
//    Jan Cholewinski 2010
//    Pawel Dluzewski 2010
//
// Copyright (C) 2010, 2012 The vecds authors
//
// This  program is  free  software: you  can  redistribute it  and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software Foundation, either version 3 of the
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

#ifndef ARC_BALL_H
#define ARC_BALL_H

                                 // standard C/C++
#include <math.h>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
                                 // Qt includes
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>

namespace vecds
{


                                 /**
				  * The ArcBall class defines tools
				  * for rotating and viewing
				  * three-dimensional images.
                                  *
				  * When viewing three-dimensional
				  * objects there are many different
				  * ways to choose a viewpoint. The
				  * basic principle is to create a
				  * sphere around a chosen object and
				  * then, by choosing a point on that
				  * sphere, drag that point to a
				  * different location and have the
				  * object rotate in a way defined by
				  * a rotation around the arc-ball.
				  *
				  * Rotations are mapped onto a
				  * quaternion. 
				  */
  class ArcBall
  {
  protected:
    
    inline glm::dvec3 mapToSphere (glm::dvec2 mouse_coordinate);
    
  public:

                                 /**
				  * Constructor. Create an empty
				  * (null) ArcBall by initialisation
				  * of certain variables.
				  */
    ArcBall ();

                                 /**
				  * Destructor 
				  */
    ~ArcBall ();
  
                                 /**
				  * Definition of what to do when
				  * "click" is done with the mouse.
				  */
    void click (const glm::dvec2 &mouse_coordinate);

                                 /**
				  * Definition of what to do when
				  * "drag" is done with the mouse.
				  */
    void drag (const glm::dvec2 &mouse_coordinate);

                                 /**
				  * Return this quaternion. 
				  */
    QQuaternion get_quaternion (const glm::dvec3 &vec_1, 
				const glm::dvec3 &vec_2);

    glm::dvec3
      vector_begin,
      vector_end;
  
//    glm::dquaternion
    QQuaternion
      quaternion,
      q_down, 
      mouseQuat;
    
    double 
      invHeight, 
      invWidth;
        
    double epsilon;

                                 // Inline and other functions
    inline void set_bounds (const int newWidth, 
			    const int newHeight)
    {
      this->invWidth  = 2. / double (newWidth  - 1);
      this->invHeight = 2. / double (newHeight - 1);
    }
    
  };                            // class ArcBall;

}                               // namespace vecds

#endif                          // ARCH_BALL_H
