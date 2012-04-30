
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

#ifndef _ArcBall_H
#define _ArcBall_H


#include <math.h>
#include <QQuaternion>
#include <QVector3D>
#include <QVector2D>

/**
 * The ArcBall class defines tools for rotating and viewing
 * three-dimensional images.
 *
 * When viewing three-dimensional objects there are many different
 * ways to choose a viewpoint. The basic principle is to create a
 * sphere around a chosen object and then, by choosing a point on that
 * sphere, drag that point to a different location and have the object
 * rotate in a way defined by a rotation around the arc-ball.
 *
 * Rotations are mapped onto a quaternion.
 */
class ArcBall
{
 protected:
  inline QVector3D mapToSphere (QVector2D mousePt);
  
 public:
  ArcBall();
  ~ArcBall() {};
  
  inline void set_bounds (int newWidth, int newHeight)
  {
    this->invWidth  = 2. / double (newWidth - 1);
    this->invHeight = 2. / double (newHeight - 1);
  }

  void click (QVector2D mousePt);
  void drag (QVector2D mousePt);

  /**
   * Return this quaternion.
   */
  QQuaternion get_quaternion (QVector3D v1, 
			      QVector3D v2);
  
  QQuaternion  quaternion;
  double invWidth, invHeight;
  
  QVector3D  startVec, endVec;
  QQuaternion  q_down, mouseQuat;
  double epsilon;

}; // class ArcBall;

#endif // _ArcBall_h
