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


#include <QtGui>
#include <QtOpenGL>
#include <GL/glut.h>
#include "../include/vecds/main_viewer.h"
#include "../include/vecds/internal.h"
#include "../include/vecds/constant.h"


extern Internal *ActualData;
QString str1;
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 

GLfloat mat_ambient[] = { 0.4, 0.4, 0.4, 0.8 };
GLfloat colorA[] = { 0.8, 0.8, 0.2, 0.7 };

GLfloat light0_position[] = { 1000.0, 1000.0, 1000.0, 0.0 };
GLfloat light1_position[] = { 1000.0, -1000.0, -1000.0, 0.0 };
GLfloat cube_mat[] = { 0.3, 0.3, 0.3, 0.2 };
GLfloat cube_mat0[] = { 0.57, 0.57, 0.585, 0.22 };
GLfloat cube_mat1[] = { 0.22, 0.75, 0.22, 0.43 };
GLfloat cube_mat11[] = { 0.75, 0.22, 0.22, 0.43 };
GLfloat mat_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat mat_specular[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat mat_emission[] = {0.1, 0.1, 0.1, 0.0};

GLfloat colorRed[] = {0.7, 0.1, 0.1, 1.0};
GLfloat colorGreen[] = {0.1, 0.7, 0.1, 1.0};
GLfloat colorBlue[] = {0.1, 0.1, 0.7, 1.0};

GLfloat mat_shininess[] = { 10.0 };

GLfloat signesAdd[] = {0.9, 0.9, 0.9, 0.7};
GLfloat disloc_sign[4];// = { 0.7, 0.1, 0.1, 0.4 };

GLfloat grey[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat invis[] = { 0.15, 0.38, 0.15, 0.1 };
QColor qcol;

//GLint viewport[4];

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
MainViewer::MainViewer(QWidget *parent)
    : QGLWidget(parent)
{
    makeCurrent();
    this->phiRot = this->thetaRot = this->psiRot = 0.;
    this->d_x = this->d_y = 0.;
    this->d_0 = 180.;

    this->n_atoms = this->n_bonds = 0;
    this->bg_red = 0.99;
    this->bg_green = 0.99;
    this->bg_blue = 1.0;
    this->VIEW_rad_fact = 0.25;
    this->VIEW_whichRadius = 1;
    this->mousePos = QVector3D(0., 0., 0.);

    init_spheres(8);

//    color_spectr = new QColor[12];
    int alfa = 255;
    set_defaults(); 

    prepare_scene();
    setFont(QFont("Times", 12));
    arcb = new ArcBall();
    transformM = new double[16];
//    thisRot = lastRot = constant::unit_matr33;
    for (int i=1; i<15; i++) transformM[i] = 0.;
    for (int i=0; i<16; i+=5) transformM[i] = 1.;
//    transformM[0] = transformM[5] =transformM[10] =transformM[15] = 1.;
// qWarning ("viewer: atoms=%d", n_atoms);
//     m_x = m_y =0.;
}

MainViewer::~MainViewer()
{
    makeCurrent();
    for (int i=0; i<125; i++) glDeleteLists(sphereList[i], 1);
}

QSize MainViewer::minimumSizeHint() const
{
    return QSize(300, 225);
}

QSize MainViewer::sizeHint() const
{
    return QSize(800, 600);
}

//-----------------------------------------------------------------------------------

void MainViewer::set_defaults()
{
//  distance = -5.0;
  this->dist0 = -1.8;
  this->fov = ActualData->set0->fov;
}



void MainViewer::prepare_scene()
{
//  calculate_size();
  if ( ActualData->atoms_loaded=="none" ) {
    qWarning("******** none atoms, none fems *********");
    min_ = QVector3D(-5., -5., -5);
    max_ = QVector3D(5., 5., 5.);
    rad_scene = 10.;
  }
  distance0 = distance = (dist0/tan(fov*constant::deg2rad))*rad_scene;
  small = 0.01 * rad_scene;
  smaller = 0.22 * small;
  ActualData->rad_scene = rad_scene;
  ActualData->min_ = min_;
  ActualData->max_ = max_;

  xl = min_.x() - cent_.x();
  xr = max_.x() - cent_.x();
  yd = min_.y() - cent_.y();
  yu = max_.y() - cent_.y();
  ActualData->cent_ = cent_;
// qWarning(" ---------- prepare_scene_2");
  prepare_invbox(min_ , max_);
//  prepare_invbox(min_ - cent_, max_ - cent_);
  prepare_axis();
}

void MainViewer::prepare_invbox(QVector3D xmin, QVector3D xmax)
{
// qWarning ("prepare_invbox");
  double x1 = xmin.x();
  double x2 = xmax.x();
  double y1 = xmin.y();
  double y2 = xmax.y();
  double z1 = xmin.z();
  double z2 = xmax.z();
  ActualData->invbox[0] = xmin;//QVector3D(x1, y1, z1);
  ActualData->invbox[1] = QVector3D(x1, y1, z2);
  ActualData->invbox[2] = QVector3D(x1, y2, z2);
  ActualData->invbox[3] = QVector3D(x1, y2, z1);
  ActualData->invbox[4] = QVector3D(x2, y1, z1);
  ActualData->invbox[5] = QVector3D(x2, y1, z2);
  ActualData->invbox[6] = xmax;//QVector3D(x2, y2, z2);
  ActualData->invbox[7] = QVector3D(x2, y2, z1);
}

void MainViewer::prepare_axis()
{
// qWarning ("prepare_axis");
  double ll = 0.1 * rad_scene;
  ActualData->axeX = QVector3D(ll, 0.0, 0.0);
  ActualData->axeY = QVector3D(0.0, ll, 0.0);
  ActualData->axeZ = QVector3D(0.0, 0.0, ll);
}

void MainViewer::init_spheres( int numbOfSubdiv )
{ 
// qWarning ("init_spheres");
  for (int i=0; i<125; i++ ) {
     GLUquadricObj *obj = gluNewQuadric();
     gluQuadricNormals(obj, GL_SMOOTH);
     gluQuadricDrawStyle(obj, GLU_FILL);
     sphereList[i] = glGenLists(1);
 
     glNewList(sphereList[i], GL_COMPILE);
     if ( VIEW_whichRadius==1 )
        gluSphere(obj, 
            ActualData->ap->a_rad1[i]*VIEW_rad_fact, numbOfSubdiv*2, numbOfSubdiv);
     else
        gluSphere(obj, 
            ActualData->ap->a_rad2[i]*VIEW_rad_fact, numbOfSubdiv*2, numbOfSubdiv);
     glEndList();

     gluDeleteQuadric(obj);
  }
  cubeList = glGenLists(1);
 
  glNewList(cubeList, GL_COMPILE);
  
   drawBox0(smaller);
  
  glEndList();

}

//-----------------------------------------------------------------------------------

void MainViewer::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    mousePt = normalizeMouse(lastPos);

    if ( ActualData->Mode==2 ) {
       QVector3D res = getOGLPos(event->x(), event->y());
       if (event->buttons() & Qt::LeftButton) {
// qWarning("res: %g %g %g", res.x, res.y, res.z);
          emit SIG_actPoint(res);
          updateGL();
       }
       if (event->buttons() & Qt::RightButton) {
          emit SIG_actPosition(res);
// qWarning("mviewer:res: %g %g %g", res.x, res.y, res.z);
//          updateGL();
       } 
    } else {
       if (event->buttons() & Qt::LeftButton) {
          mousePos = getMousePos(event->x(), event->y());
          arcb->click(mousePt);//Update Start Vector And Prepare For Dragging
// qWarning("<--------  startVec = (%g %g %g)", arcb->startVec.x, arcb->startVec.y, arcb->startVec.z);
//  qWarning(" Mouse = (%g %g %g)", mousePos.x, mousePos.y, mousePos.z);
       }// else {
//          arcb->quaternion = Quat(0., 0., 0., 1.);
//          quat2matr(arcb->quaternion);
//       }
    }
//    thisRot = lastRot;

}

void MainViewer::mouseMoveEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    mousePt = normalizeMouse(lastPos);
    if (event->buttons() & Qt::LeftButton) {
       arcb->drag(mousePt);
// qWarning("--------> endVec = (%g %g %g)",
//                 arcb->startVec.x, arcb->startVec.y, arcb->startVec.z,
//                           arcb->endVec.x, arcb->endVec.y, arcb->endVec.z);
       quat2matr(arcb->quaternion);
       QVector3D result = quat2euler(arcb->quaternion)*constant::rad2deg;
// qWarning("Angles = %g %g %g",
//          constant::rad2deg*result.x, rad2deg*result.y, rad2deg*result.z);

       if ( thetaRot!=result.x() ) {
          thetaRot = result.x();
          if ( thetaRot<-180. ) thetaRot += 360.;
          if ( thetaRot>180. ) thetaRot -= 360.;
          emit SIG_thetaRotationChanged(int(thetaRot));
       }
       if ( phiRot!=result.z() ) {
          phiRot = result.z();
          if ( phiRot<-90. ) phiRot += 180.;
          if ( phiRot>90. ) phiRot -= 180.;
          emit SIG_phiRotationChanged(int(phiRot));
       }
       if ( psiRot!=result.y() ) {
          psiRot = result.y();
          if ( psiRot<-180. ) psiRot += 180.;
          if ( psiRot>180. ) psiRot -= 180.;
          emit SIG_psiRotationChanged(int(psiRot));
       }
// qWarning("Angles = %g %g %g", thetaRot, phiRot, psiRot);
       updateGL();
//        }
    } else if ( event->buttons() & Qt::RightButton && ActualData->Mode!=2 ) {
       int mmx = event->x() - lastPos.x();
       int mmy = event->y() - lastPos.y();
       if ( mmx!=0 ) {
           d_x += 0.002*mmx;
           emit SIG_xMovementChanged(int(100*d_x));
           updateGL();
       }
       if ( mmy!=0 ) {
           d_y -= 0.002*mmy;
           emit SIG_yMovementChanged(int(100*d_y));
           updateGL();
       }
    }
}

void MainViewer::wheelEvent(QWheelEvent *event)
{
   if ( event->delta()<0 )  distance *= 0.99;
   else                     distance *= 1.01;
   if ( distance<distance0 ) {
      distance = distance0;
      return;
   }
   emit SIG_zMovementChanged(int(180*distance/distance0));
   updateGL();
}

QVector2D MainViewer::normalizeMouse(QPoint qp)
{
   QVector2D res = QVector2D(arcb->invWidth * (double(qp.x()) - 0.5*screenW),
                             arcb->invHeight * (0.5*screenH - double(qp.y())));

   return res;
}

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

void MainViewer::resizeGL(int w, int h)
{
// qWarning("resizeGL");
  if ( w==0 ) h = 1;  // prevent divide by 0 error when minimised
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov, (float)w/h, 0.1, 50000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  this->screenW = double(w);
  this->screenH = double(h);

  arcb->set_bounds (w, h);
//  qWarning("arcb->set_bounds:  resizeGL");
}
//  gluPerspective(VS->fov, float(w)/h, 0.01, 3000);
//  glOrtho(zoom*xl, zoom*xr, zoom*yd, zoom*yu, zoom*zn, zoom*zf);
//  glOrtho(-siz, siz, -siz*f, siz*f, 0.0, 10.*siz);
//-----------------------------------------------------------------------------------

void MainViewer::paintGL()
{
// qWarning("paintGL");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(bg_red, bg_green, bg_blue, 1.0);

  if ( ActualData->atoms_loaded=="none" && ActualData->img_loaded=="none") return;
//  glPushMatrix();
  glLoadIdentity();
  double ddx = 1.75 * (max_.x() - min_.x()) * d_x;
  double ddy = 1.75 * (max_.y() - min_.y()) * d_y;
  

  glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  glGetIntegerv(GL_VIEWPORT, viewport);
// get 3D coordinates based on window coordinates
//gluUnProject(int(viewport[2]*0.999), int(viewport[3]*0.999), 0.01,
//             model_view, projection, viewport, &ru_x, &ru_y, &ru_z);

//     glPushMatrix();
  glTranslated(ddx, ddy, distance);
//  glTranslated(-cent_.x, -cent_.y, -cent_.z);
//  glTranslated(0., 0., 10.);
  glMultMatrixd(transformM);
//   getAxisAngle(arcb->quaternion, axis, angle);
//   glRotated(constant::rad2deg*angle, axis.x, axis.y, axis.z);
//  glTranslated(0., 0., -10.);
//      glPopMatrix();
// qWarning("paintGL -- axes");
  if ( ActualData->img_loaded!="none" ) {
     glPushMatrix();
     glDisable(GL_LIGHTING);
     glEnable(GL_TEXTURE_2D);
     drawTexture(background, QVector3D(xl, yd, 0.0), QVector3D(xl, yu, 0.0), 
                            QVector3D(xr, yu, 0.0), QVector3D(xr, yd, 0.0));
//     glDisable(GL_COLOR_MATERIAL);
     glDisable(GL_TEXTURE_2D);
//     glDisable (GL_BLEND);
//    glEnable(GL_DEPTH_TEST);
     glClear(GL_DEPTH_BUFFER_BIT);
     glEnable(GL_LIGHTING);

     glPopMatrix();
  }


  if ( ActualData->atoms_loaded!="none" ) {
     if ( ActualData->atoms->n_atoms>0 && ActualData->visible[0] ) {
        glPushMatrix();
        draw_atoms();
        glPopMatrix();
     }
     if ( ActualData->atoms->n_bonds>0 && ActualData->visible[1] ) {
        glPushMatrix();
        draw_bonds();
        glPopMatrix();
     }
// qWarning("paintGL -- atoms");
// qWarning("distance=%g, rad_scene=%g, n_atoms=%d", distance, rad_scene, ActualData->atoms->n_atoms);
  }

  if ( ActualData->visible[5] ) {
     glPushMatrix();
     draw_axis();
     glPopMatrix();
  }

  if ( ActualData->ndisl>0 ){
     glPushMatrix();
     doGLdisloc();
     glPopMatrix();
  qWarning("VIEWER - ndisl=%d", ActualData->ndisl);
  }
/*
  if ( ActualData->Mode==2 ){
     glPushMatrix();
     glEnable(GL_BLEND);
     glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, invis);
     drawInvisBox();
     glDisable(GL_BLEND);
     glPopMatrix();
  }

  if ( ActualData->visible[6] && ActualData->res_loaded!="none" ) {
     glDisable(GL_LIGHTING);
     QVector3D ld = QVector3D(ld_x, ld_y, ld_z);
     QVector3D rd = QVector3D(rd_x, rd_y, rd_z);
     QVector3D lu = QVector3D(lu_x, lu_y, lu_z);
     QVector3D ru = QVector3D(ru_x, ru_y, ru_z);
     QVector3D ld0 = ld;
     QVector3D rd0 = rd;
     const float f = 1./255.;
     for (int i=0; i<12; i++) {
        Int3 col = ActualData->set0->col_spectr[i];
        glColor4f(f*col.i1, f*col.i2, f*col.i3, 1.0);

        QVector3D l = ld + (lu - ld) * (double(i+1)/12.);
        QVector3D r = rd + (ru - rd) * (double(i+1)/12.);
        glBegin(GL_QUADS);
         glVertex3d(ld0.x, ld0.y, ld0.z);
         glVertex3d(l.x, l.y, l.z);
         glVertex3d(r.x, r.y, r.z);
         glVertex3d(rd0.x, rd0.y, rd0.z);
        glEnd();
        str1.sprintf("%d", i);
        QVector3D ll = (ld0+l)*0.5;
        glColor4f(0.0, 0.0, 0.0, 1.0);
        glDisable(GL_DEPTH_TEST);
        renderText( ll.x, ll.y, ll.z, str1);
        glEnable(GL_DEPTH_TEST);
        ld0 = l;
        rd0 = r;
     }
     glEnable(GL_LIGHTING);
  }
     if ( ActualData->visible[0] ) {
        glPushMatrix();
        draw_faces1(0.);
        glPopMatrix();
     }

     if ( ActualData->visible[1] ) {
        glPushMatrix();
        draw_faces1(ActualData->mfactor);
        glPopMatrix();
     }

// qWarning("paintGL -- fems");

//     glPushMatrix();
//     draw_numbers();
//     glPopMatrix();

     if ( ActualData->visible[2] ) {
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cube_mat1);
        draw_nod_disp(0.);
        glPopMatrix();
     }

     if ( ActualData->visible[3] ) {
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cube_mat2);
        draw_nod_disp(ActualData->mfactor);
        glPopMatrix();
     }

  }

  if ( ActualData->visible[7] ) {
     glPushMatrix();
     draw_arrows(ActualData->mfactor);
     glPopMatrix();
  }

*/
//  glPopMatrix();
}


//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

void MainViewer::draw_atoms()
{
  for (unsigned int i=0; i<ActualData->atoms->n_atoms; i++) { 
     unsigned int aki = ActualData->atoms->type[i];
//        qWarning("i, aki=%d, %d ", i, aki);
     QVector3D trans = ActualData->atoms->coord[i] - cent_;
     glPushMatrix();
     glTranslated(trans.x(), trans.y(), trans.z());
     a_color[0] = ActualData->ap->atom_red[aki];
     a_color[1] = ActualData->ap->atom_green[aki];
     a_color[2] = ActualData->ap->atom_blue[aki];
     a_color[3] = 1.0;
//     glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, a_color);
     glMaterialfv(GL_FRONT, GL_AMBIENT, a_color);
     glMaterialfv(GL_FRONT, GL_DIFFUSE, a_color);

     glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
//     glColor3f(a_color[0], a_color[1], a_color[2]);
     glCallList (sphereList[aki]);
     glPopMatrix();
  }
}

void MainViewer::draw_bonds()
{
 glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
 glMaterialfv(GL_FRONT, GL_DIFFUSE, grey);

 for (unsigned int i=0; i<ActualData->atoms->n_bonds; i++) {
     int a1 = ActualData->atoms->bonds[i].i1 - 1;
     int a2 = ActualData->atoms->bonds[i].i2 - 1;

     glPushMatrix();

     QVector3D temp1 = ActualData->atoms->coord[a2] - ActualData->atoms->coord[a1];
     double dist = temp1.length();
     double invnorm = 1.0 / dist;
     QVector3D temp2 = QVector3D(-temp1.y()*invnorm, temp1.x()*invnorm, 0.0);
     QVector3D trans = ActualData->atoms->coord[a1] - cent_;
     glTranslated(trans.x(), trans.y(), trans.z());
     glRotated(constant::rad2deg*acos(temp1.z()*invnorm), temp2.x(), temp2.y(), temp2.z());

     GLUquadricObj *qobj = gluNewQuadric();
     gluCylinder(qobj, 0.05, 0.05, dist, 16, 8);
     gluDeleteQuadric(qobj);

     glPopMatrix();
  }
}


void MainViewer::draw_numbers()
{
/*
  if ( n_nodes<=0 ) return;

  double delta = 0.01*rad_scene;
  glColor4f(0.9, 0.01, 0.1, 0.95);
  for (int i=0; i<n_nodes; i++) {
     glPushMatrix();

     QVector3D vc = nodes->coords[i] - cent_;
//  qWarning("B %d  -  (%g, %g, %g)", i, vc.x, vc.y, vc.z);
     glTranslated(vc.x, vc.y, vc.z);
     string.sprintf("%2d", i);
     renderText( vc.x+delta, vc.y+delta, vc.z+delta, string);
//     renderText( vc.x, vc.y, vc.z, string);

     glPopMatrix();
  }
*/
}

void MainViewer::draw_axis()
{
//  QVector3D orig = min_ - cent_;
  const double fact = 1.0666;
  QVector3D orig = QVector3D(fact*(min_.x()-cent_.x()), fact*(min_.y()-cent_.y()), fact*(max_.z()-cent_.z()));

  glPushMatrix();
  glMaterialfv(GL_FRONT, GL_AMBIENT, colorRed);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, colorRed);
  arrow(orig, ActualData->axeX, 0.3, small);
  glPopMatrix();

  glPushMatrix();
  glMaterialfv(GL_FRONT, GL_AMBIENT, colorGreen);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, colorGreen);
  arrow(orig, ActualData->axeY, 0.3, small);
  glPopMatrix();

  glPushMatrix();
  glMaterialfv(GL_FRONT, GL_AMBIENT, colorBlue);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlue);
  arrow(orig, ActualData->axeZ, 0.3, small);
  glPopMatrix();

}

void MainViewer::arrow(QVector3D orig, QVector3D vect, double fact, double sm)
{
   vect *= (1.0 - fact);
   double dist1 = vect.length();
   double dist2 = dist1 * ( fact/(1.0-fact) );
   double invnorm = 1.0 / dist1;
   QVector3D temp1 = QVector3D(-vect.y()*invnorm, vect.x()*invnorm, 0.0);
   double ang = constant::rad2deg*acos(vect.z()*invnorm);
//   glPushMatrix();
   glTranslated(orig.x(), orig.y(), orig.z());
   glRotated(ang, temp1.x(), temp1.y(), temp1.z());
   GLUquadricObj *qobj = gluNewQuadric();
   gluCylinder(qobj, sm, sm, dist1, 16, 8);
   gluDeleteQuadric(qobj);
   glTranslated(0.0, 0.0, dist1);
   qobj = gluNewQuadric();
   gluCylinder(qobj, 1.5*sm, 0.0, dist2, 16, 8);
   gluDeleteQuadric(qobj);
//   glPopMatrix();
}

void MainViewer::SL_dothetaRotation()
{
/*
  double angl;
  if ( ActualData->sliderMove ) {
     angl = ActualData->sliderValue;
     if ( angl<-180. ) angl += 360.;
     if ( angl>180. ) angl -= 360.;
     thetaRot = double(angl);
     ActualData->sliderMove = false;
     euler2matr();
     arcb->quaternion = quatfromEuler();
     updateGL();
  }
// qWarning("...Y ROTATION - %g", VS->y_rot);
*/
  double angl;
  if ( ActualData->sliderMove ) {
//     angl = ActualData->sliderValue;
     angl = double(ActualData->sliderValue);
     if ( angl<-180. ) angl += 360.;
     if ( angl>180. ) angl -= 360.;
     thetaRot = double(angl);
     ActualData->sliderMove = false;
     euler2matr();
     arcb->quaternion = quatfromEuler();
     updateGL();
  }
}

void MainViewer::SL_dophiRotation()
{
  double angl;
  if ( ActualData->sliderMove ) {
     angl = ActualData->sliderValue;
     if ( angl<-90. ) angl += 180.;
     if ( angl>90. ) angl -= 180.;
     phiRot = angl;
     ActualData->sliderMove = false;
     euler2matr();
     arcb->quaternion = quatfromEuler();
     updateGL();
  }
// qWarning("...X ROTATION - %g", VS->x_rot);
}

void MainViewer::SL_dopsiRotation()
{
  double angl;
  if ( ActualData->sliderMove ) {
     angl = ActualData->sliderValue;
     if ( angl<-180. ) angl += 360.;
     if ( angl>180. ) angl -= 360.;
     psiRot = double(angl);
     ActualData->sliderMove = false;
     euler2matr();
     arcb->quaternion = quatfromEuler();
     updateGL();
  }
}
// qWarning("...Z ROTATION - %g", VS->z_rot);
void MainViewer::SL_doXMovement()
{
  if ( ActualData->sliderMove ) {
     d_x = 0.01*double(ActualData->sliderValue);
     ActualData->sliderMove = false;
     updateGL();
  }
 qWarning("...X move - %g", d_x);
}

void MainViewer::SL_doYMovement()
{
  if ( ActualData->sliderMove ) {
     d_y = 0.01*double(ActualData->sliderValue);
     ActualData->sliderMove = false;
     updateGL();
  }
// qWarning("...Y move - %g", d_y);
}

void MainViewer::SL_doZMovement()
{
  if ( ActualData->sliderMove ) {
     d_0 = double(ActualData->sliderValue)/180.;
     ActualData->sliderMove = false;
     distance = d_0 * distance0;
     updateGL();
  }
}

/*
void MainViewer::SL_doXMovement()
{
  if ( ActualData->sliderMove ) {
     d_x = ActualData->sliderValue;
     ActualData->sliderMove = false;
     updateGL();
  }
// qWarning("...X move - %g", d_x);
}

void MainViewer::SL_doYMovement()
{
  if ( ActualData->sliderMove ) {
     d_y = ActualData->sliderValue;
     ActualData->sliderMove = false;
     updateGL();
  }
// qWarning("...Y move - %g", d_y);
}

void MainViewer::SL_doZMovement()
{
  if ( ActualData->sliderMove ) {
     d_0 = ActualData->sliderValue;
     ActualData->sliderMove = false;
     distance = d_0 * distance0;
     updateGL();
  }
}
*/
void MainViewer::SL_needDraw()
{
  qWarning("slot needDraw");
   prepare_scene();
   paintGL();
}

void MainViewer::SL_repaint()
{
   paintGL();
}

void MainViewer::SL_keypress(int key)
{
  qWarning("slot: key = %d", key);
}

void MainViewer::drawTexture(GLuint tex, QVector3D pld, QVector3D plu, QVector3D pru, 
                                                                 QVector3D prd)
{
// qWarning("drawTexture");
  glBindTexture(GL_TEXTURE_2D, tex);
//  glNormal3f(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0);          glVertex3d(pld.x(), pld.y(), pld.z());
   glTexCoord2f(0.0, 1.0);          glVertex3f(plu.x(), plu.y(), plu.z());
   glTexCoord2f(1.0, 1.0);          glVertex3f(pru.x(), pru.y(), pru.z());
   glTexCoord2f(1.0, 0.0);          glVertex3f(prd.x(), prd.y(), prd.z());
  glEnd();
}

void MainViewer::SL_loadImage()
{
  background = Pixmap2texture(&ActualData->img);
}

GLuint MainViewer::Pixmap2texture(QPixmap *pixmap)
{
  GLuint tex;
// 1E-3 needed. Just try with width=128 and see !
  int width0 = pixmap->width();
  int height0 = pixmap->height();
  int newWidth  = 1<<(int)(1+log(width0 -1+1E-3) / log(2.0));
  int newHeight = 1<<(int)(1+log(height0-1+1E-3) / log(2.0));
  if ( ( width0!=newWidth ) || ( height0!=newHeight ) ) {
     qWarning("Image size set to %dx%d pixels", newWidth, newHeight);
     pixmap->copy(0, 0, newWidth, newHeight);
  }
  tex = bindTexture(*pixmap, GL_TEXTURE_2D);
//  delete *pixmap;
  return tex;
}


GLuint MainViewer::image2texture(QImage* bmp)
{
  GLuint tex;
  // 1E-3 needed. Just try with width=128 and see !
  int width0 = bmp->width();
  int height0 = bmp->height();
  int newWidth  = 1<<(int)(1+log(width0 -1+1E-3) / log(2.0));
  int newHeight = 1<<(int)(1+log(height0-1+1E-3) / log(2.0));
  if ( ( width0!=newWidth ) || ( height0!=newHeight ) ) {
     qWarning("Image size set to %dx%d pixels", newWidth, newHeight);
//     qWarning("loadImage:  u_max, >v_max %g %g", tex.u_max, tex.v_max);
     bmp->copy(0, 0, newWidth, newHeight);
  }
// Bind the img texture...
  tex = bindTexture(*bmp, GL_TEXTURE_2D);
//  delete *pixmap;
  return tex;
}

//-----------------------------------------------------------------------------------

void MainViewer::initializeGL()
{
  qWarning ("initializeGL");
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
/*
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
*/
   glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse);
   glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
   glLightfv(GL_LIGHT1, GL_AMBIENT, mat_ambient);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, mat_diffuse);
   glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);

//   glEnable(GL_TEXTURE_2D);
//   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

   glEnable(GL_DEPTH_TEST);
//   glEnable(GL_CULL_FACE);
//   glDisable(GL_DEPTH_TEST);
//   glDisable(GL_CULL_FACE);

   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

}
// qWarning ("Fems - vertex");

QVector3D MainViewer::getOGLPos(int x, int y)
{
  GLfloat winX, winY, winZ;
  GLdouble posX, posY, posZ;
  winX = float(x);
  winY = float(viewport[3]) - float(y);
  glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
  gluUnProject( winX, winY, winZ, model_view, projection, viewport,
                                                     &posX, &posY, &posZ);
 qWarning(" +++ GetOGLPos  disloc. position:  (%g, %g, %g) (%g, %g, %g)", 
                                            winX, winY, winZ, posX, posY, posZ);
  return QVector3D(posX, posY, posZ);
}

QVector3D MainViewer::getMousePos(int x, int y)
{
  GLfloat winX, winY, winZ;
  GLdouble posX, posY, posZ;
  winX = float(x);
  winY = float(viewport[3]) - float(y);
//  winZ = float(min_.z);
  winZ = 1.;
//  glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
  gluUnProject( winX, winY, winZ, model_view, projection, viewport,
                                                     &posX, &posY, &posZ);
//  winZ = 1.;
 qWarning(" +++ GetMousePos  mouse position:  (%g, %g, %g) (%g, %g, %g)",
                                            winX, winY, winZ, posX, posY, posZ);
  return QVector3D(posX, posY, posZ);
}

void MainViewer::doGLdisloc()
{
  if ( ActualData->ndisl<=0 ) return;
  for (int i=0; i<ActualData->ndisl; i++) { 
  qWarning("doGLdisloc  --   rrr=%g, %g, %g", ActualData->disl[i].rrr.x(), ActualData->disl[i].rrr.y(),
                                                                         ActualData->disl[i].rrr.z());
     glPushMatrix();
     QVector3D p1 = ActualData->disl[i].p1 - cent_;
     QVector3D p2 = ActualData->disl[i].p2 - cent_;
     QVector3D temp1 = p2 - p1;
     double dist = temp1.length();
     double invnorm = 1.0 / dist;

     QVector3D temp2 = QVector3D(-temp1.y()*invnorm, temp1.x()*invnorm, 0.0);
     glTranslated(p1.x(), p1.y(), p1.z());
     glRotated(constant::rad2deg*acos(temp1.z()*invnorm), temp2.x(), temp2.y(), temp2.z());
//     glColor4f(0.5, 0.5, 0.5, 1.0);
     glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cube_mat11);
     GLUquadricObj *qobj = gluNewQuadric();
     gluCylinder(qobj, 0.15, 0.15, dist, 16, 8);
     gluDeleteQuadric(qobj);
     glPopMatrix();
  }
}

// ---------------------------------------

void MainViewer::drawBox(double v[8][3], double norm_v[8][3])
{
  static GLint _faces[6][4] = {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  for (int i=5; i>=0; i--) {
     glBegin(GL_QUADS);
//	glNormal3dv(&norm_v[i][0]);
     glVertex3dv(&v[_faces[i][0]][0]);
     glVertex3dv(&v[_faces[i][1]][0]);
     glVertex3dv(&v[_faces[i][2]][0]);
     glVertex3dv(&v[_faces[i][3]][0]);
     glEnd();
  }
}

void MainViewer::drawBox0(float size)
{
   static GLfloat n[6][3] = {
                {-1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{1.0, 0.0, 0.0},
		{0.0, -1.0, 0.0},
		{0.0, 0.0, 1.0},
		{0.0, 0.0, -1.0}
   };
   static GLint _faces[6][4] = {
		{0, 1, 2, 3},
		{3, 2, 6, 7},
		{7, 6, 5, 4},
		{4, 5, 1, 0},
		{5, 6, 2, 1},
		{7, 4, 0, 3}
   };
   GLfloat v[8][3];
   GLint i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

   for (i = 5; i >= 0; i--) {
	glBegin(GL_QUADS);
	glNormal3fv(&n[i][0]);
	glVertex3fv(&v[_faces[i][0]][0]);
	glVertex3fv(&v[_faces[i][1]][0]);
	glVertex3fv(&v[_faces[i][2]][0]);
	glVertex3fv(&v[_faces[i][3]][0]);
	glEnd();
   }
}

void MainViewer::euler2matr()
{
    // Assuming the angles are in radians.
    double ch = cos(thetaRot*constant::deg2rad); // heading
    double sh = sin(thetaRot*constant::deg2rad);
    double ca = cos(psiRot*constant::deg2rad);   // attitude
    double sa = sin(psiRot*constant::deg2rad);
    double cb = cos(phiRot*constant::deg2rad);   // bank
    double sb = sin(phiRot*constant::deg2rad);

    transformM[0] = ch*ca;
    transformM[1] = sh*sb - ch*sa*cb;
    transformM[2] = ch*sa*sb + sh*cb;
    transformM[4] = sa;
    transformM[5] = ca*cb;
    transformM[6] = -ca*sb;
    transformM[8] = -sh*ca;
    transformM[9] = sh*sa*cb + ch*sb;
    transformM[10] = -sh*sa*sb + ch*cb;
    transformM[3] = transformM[7] = transformM[11] = 
    transformM[12] = transformM[13] = transformM[14] = 0.;
    transformM[15] = 1.;
}

void MainViewer::quat2matr(QQuaternion q)
{
    double sqw = q.scalar()*q.scalar();
    double sqx = q.x()*q.x();
    double sqy = q.y()*q.y();
    double sqz = q.z()*q.z();

    double invs = 1 / (sqx + sqy + sqz + sqw);
    transformM[0] = (sqx - sqy - sqz + sqw)*invs;
    transformM[5] = (-sqx + sqy - sqz + sqw)*invs;
    transformM[10] = (-sqx - sqy + sqz + sqw)*invs;
    
    double tmp1 = q.x()*q.y();
    double tmp2 = q.z()*q.scalar();
    transformM[4] = 2.*(tmp1 + tmp2)*invs;
    transformM[1] = 2.*(tmp1 - tmp2)*invs;
    tmp1 = q.x()*q.z();
    tmp2 = q.y()*q.scalar();
    transformM[8] = 2.*(tmp1 - tmp2)*invs;
    transformM[2] = 2.*(tmp1 + tmp2)*invs;
    tmp1 = q.y()*q.z();
    tmp2 = q.x()*q.scalar();
    transformM[9] = 2.*(tmp1 + tmp2)*invs;
    transformM[6] = 2.*(tmp1 - tmp2)*invs;
    transformM[3] = transformM[7] = transformM[11] = 
    transformM[12] = transformM[13] = transformM[14] = 0.;
    transformM[15] = 1.;
}
/*
*   Coordinate System: right hand
*   Positive angle: right hand
*   Order of euler angles: heading first, then attitude, then bank
*   matrix row column ordering:
*   [m00 m01 m02]
*   [m10 m11 m12]
*   [m20 m21 m22]
*/
QVector3D MainViewer::quat2euler(QQuaternion q)
{
   double head, att, b;
   double test = q.x()*q.y() + q.z()*q.scalar();
   if (test > 0.499) { // singularity at north pole
      head = 2.*atan2(q.x(), q.scalar());
      att = constant::pi_2;
      b = 0.;
      return QVector3D(head, att, b);
   }
   if (test < -0.499) { // singularity at south pole
     head = -2.*atan2(q.x(), q.scalar());
     att = - constant::pi_2;
     b = 0.;
     return QVector3D(head, att, b);
   }
   double sqx = q.x()*q.x();
   double sqy = q.y()*q.y();
   double sqz = q.z()*q.z();
   head = atan2(2.*q.y()*q.scalar()-2.*q.x()*q.z(), 1. - 2.*sqy - 2.*sqz);
   att = asin(2.*test);
   b = atan2(2.*q.x()*q.scalar()-2.*q.y()*q.z(), 1. - 2.*sqx - 2.*sqz);
   return QVector3D(head, att, b);
}

QQuaternion MainViewer::quatfromEuler()
{
//   QQuaternion res;
   double heading2  = 0.5 * constant::deg2rad * thetaRot;
   double attitude2 = 0.5 * constant::deg2rad * psiRot;
   double bank2     = 0.5 * constant::deg2rad * phiRot;
   double c1 = cos(heading2);
   double s1 = sin(heading2);
   double c2 = cos(attitude2);
   double s2 = sin(attitude2);
   double c3 = cos(bank2);
   double s3 = sin(bank2);
   double c1c2 = c1*c2;
   double s1s2 = s1*s2;
   return QQuaternion(c1c2*c3 - s1s2*s3, 
       c1c2*s3 + s1s2*c3, s1*c2*c3 + c1*s2*s3, c1*s2*c3 - s1*c2*s3);
}

void MainViewer::getAxisAngle(const QQuaternion q, QVector3D &v, double &ang)
{
   double temp_angle = acos(q.scalar());
   double scale = sqrt(q.x()*q.x() + q.y()*q.y() + q.z()*q.z());
   double epsilon = 1.e-10;
   if ( scale<epsilon )  {
       ang = 0.;
       v = QVector3D(0., 0., 1.);
   } else {
       ang = temp_angle + temp_angle;
       scale = 1./scale;
       v = QVector3D(q.x()*scale, q.y()*scale, q.z()*scale);
       v = v.normalized();
   }
   return;
}
