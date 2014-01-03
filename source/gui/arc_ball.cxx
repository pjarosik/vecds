
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

//#include ".//arcball.h" // ArcBall header
#include <vecds/gui/arc_ball.h> 

//Arcball sphere constants:
//Diameter is       2.0f
//Radius is         1.0f
//Radius squared is 1.0f


/**
 * Constructor. Create an empty ArcBall.
 */
ArcBall::ArcBall()
{
  this->epsilon  = 1.e-8;
  this->startVec = this->endVec = QVector3D(0., 0., 0.);
  this->is_used = true;
  this->mouseQuat    = 
    this->q_down     = 
    this->quaternion = 
    QQuaternion (1., 0., 0., 0.);
    get_matrix();
//  this->rotM = new double(16);
}

QVector2D ArcBall::normalizeMouse(QPoint qp)
{
  return QVector2D (this->inv_width *(double(qp.x()) - 0.5*this->w),
                             this->inv_height*(0.5*this->h - double(qp.y())));
}

QVector3D ArcBall::mapToSphere(QVector2D mousePt)
{
  QVector3D res  = QVector3D (mousePt.x(), mousePt.y(), 0.);
  double length2 = res.x()*res.x() + res.y()*res.y();
  if ( length2>1.0 ) {
      double norm = sqrt(length2);
      res = QVector3D (res.x()/norm, res.y()/norm, 0.);
  } else  res.setZ (sqrt(1.96-length2));
  return res;
}

void ArcBall::click(QVector2D mousePt)
{
  this->is_used = true;
  this->startVec  = mapToSphere(mousePt);
  this->q_down    = this->quaternion;
  this->mouseQuat = QQuaternion(1., 0., 0., 0.);
}

void ArcBall::drag(QVector2D mousePt)
{
  this->endVec     = mapToSphere(mousePt);
  this->mouseQuat  = get_quaternion(this->startVec, this->endVec);
  this->quaternion = this->mouseQuat * this->q_down;
}
/*
void ArcBall::release(QVector2D mousePt)
{ 
  this->is_used = false;
//  this->endVec     = mapToSphere(mousePt);
//  this->mouseQuat  = get_quaternion(this->startVec, this->endVec);
//  this->quaternion = this->mouseQuat * this->q_down;
}
*/
QQuaternion ArcBall::get_quaternion(QVector3D v1, QVector3D v2)
{
  QVector3D perp = QVector3D::crossProduct (v2, v1);
  return (perp.x()*perp.x()+perp.y()*perp.y()+perp.z()*perp.z()>this->epsilon) ?
          QQuaternion (QVector3D::dotProduct(v1, v2),
                        perp.x(), perp.y(), perp.z()).normalized()
          :
          QQuaternion(1., 0., 0., 0.);
}

void ArcBall::get_matrix()
{
    double sqw = this->quaternion.scalar()*this->quaternion.scalar();
    double sqx = this->quaternion.x()*this->quaternion.x();
    double sqy = this->quaternion.y()*this->quaternion.y();
    double sqz = this->quaternion.z()*this->quaternion.z();

    double invs = 1 / (sqx + sqy + sqz + sqw);
    rotM[0] = (sqx - sqy - sqz + sqw)*invs;
    rotM[5] = (-sqx + sqy - sqz + sqw)*invs;
    rotM[10] = (-sqx - sqy + sqz + sqw)*invs;
    
    double tmp1 = this->quaternion.x()*this->quaternion.y();
    double tmp2 = this->quaternion.z()*this->quaternion.scalar();
    rotM[4] = 2.*(tmp1 + tmp2)*invs;
    rotM[1] = 2.*(tmp1 - tmp2)*invs;
    tmp1 = this->quaternion.x()*this->quaternion.z();
    tmp2 = this->quaternion.y()*this->quaternion.scalar();
    rotM[8] = 2.*(tmp1 - tmp2)*invs;
    rotM[2] = 2.*(tmp1 + tmp2)*invs;
    tmp1 = this->quaternion.y()*this->quaternion.z();
    tmp2 = this->quaternion.x()*this->quaternion.scalar();
    rotM[9] = 2.*(tmp1 + tmp2)*invs;
    rotM[6] = 2.*(tmp1 - tmp2)*invs;
    rotM[3] = rotM[7] = rotM[11] = 
    rotM[12] = rotM[13] = rotM[14] = 0.;
    rotM[15] = 1.;
}

