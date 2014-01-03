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

#ifndef _ArcBall_H
#define _ArcBall_H


//#include <math.h>
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

  /**
   * Constructor.
   */
  ArcBall();

  /**
   * Destructor.
   */
  ~ArcBall() {};
  
  /**
   * Set the bounds to this width and this height.
   */
  inline void set_bounds (int width, int height)
  {
    this->inv_width  = 2. / double (width - 1);
    this->inv_height = 2. / double (height - 1);
    this->w = width;
    this->h = height;
  }

  QVector2D normalizeMouse(QPoint qp);
  void click(QVector2D mousePt);
  void drag(QVector2D mousePt);
//  void release(QVector2D mousePt);

  /**
   * Return this quaternion.
   */
  QQuaternion get_quaternion(QVector3D v1, QVector3D v2);
  void get_matrix();

  QQuaternion quaternion;
  /**
   * ?
   */
  double inv_height;
  /**
   * ?
   */
  double rotM[16];
  double inv_width;
  bool is_used;
  QVector3D  startVec, endVec;
  QQuaternion  q_down, mouseQuat;
  int w, h;
  double epsilon;

}; // class ArcBall;

#endif // _ArcBall_h
