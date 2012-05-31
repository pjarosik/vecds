// -------------------------------------------------------------------
//
// Author: 
//    Jan Cholewinski and Pawel Dluzewski 2010, 
//    Toby D. Young 2012.
//
// Copyright (C) 2010, 2012 The vecds authors
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------


                                 // Qt includes
#include <QtGui>
#include <QtOpenGL>
#include <GL/glut.h>

                                 // vecds gui includes
#include <vecds/gui/main_viewer.h>

#include <vecds/internal.h>

extern Internal *ActualData;
QString str1;
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 

GLfloat mat_ambient[] = { 0.4, 0.4, 0.4, 0.8 };
GLfloat colorA[] = { 0.8, 0.8, 0.2, 0.7 };

GLfloat light0_position[] = { 1000.0, 1000.0, 1000.0, 0.0 };
GLfloat light1_position[] = { 1000.0, -1000.0, -1000.0, 0.0 };
GLfloat cube_mat[]        = { 0.3, 0.3, 0.3, 0.2 };
GLfloat cube_mat0[]       = { 0.57, 0.57, 0.585, 0.22 };
GLfloat cube_mat1[]       = { 0.22, 0.75, 0.22, 0.43 };
GLfloat cube_mat11[]      = { 0.75, 0.22, 0.22, 0.43 };
GLfloat mat_diffuse[]     = { 0.9, 0.9, 0.9, 1.0 };
GLfloat mat_specular[]    = { 0.4, 0.4, 0.4, 1.0 };
GLfloat mat_emission[]    = {0.1, 0.1, 0.1, 0.0};
GLfloat specular_color[4] = { 0.4, 0.4, 0.4, 0.8 };
GLfloat colorRed[]        = {0.7, 0.1, 0.1, 1.0};
GLfloat colorGreen[]      = {0.1, 0.7, 0.1, 1.0};
GLfloat colorBlue[]       = {0.1, 0.1, 0.7, 1.0};

GLfloat mat_shininess[] = { 10.0 };
GLfloat signesAdd[] = {0.9, 0.9, 0.9, 0.7};
GLfloat disloc_sign[4];// = { 0.7, 0.1, 0.1, 0.4 };

GLfloat grey[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat invis[] = { 0.15, 0.38, 0.15, 0.1 };
QColor qcol;

//GLint viewport[4];

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 



vecds::MainViewer::MainViewer (QWidget *parent)
  : 
  QGLWidget (parent),
                                 // initial conditions (none)
  n_atoms   (0),
  n_bonds   (0),
                                 // background colours
  bg_red    (0.99),
  bg_green  (0.99),
  bg_blue   (0.99)
{
  makeCurrent ();

  this->rangle_phi       = 0.;
  this->rangle_theta     = 0.;
  this->rangle_psi       = 0.;
  this->d_x              = 0.;
  this->d_y              = 0.;
  this->d_0              = 180.;
  this->VIEW_rad_fact    = 0.25;
  this->VIEW_whichRadius = 1;
  this->mousePos         = QVector3D (0., 0., 0.);
  
  init_spheres (8);

  set_defaults (); 
  
  prepare_scene ();

  arcball    = new vecds::ArcBall ();
  transformM = new double[16];

  for (unsigned int i=1; i<15; ++i) 
    transformM[i] = 0.;

  for (unsigned int i=0; i<16; i+=5) 
    transformM[i] = 1.;

#ifdef DEBUG
  std::cout << "class MainViewer: successfully initialized." << std::endl;
#endif
}

vecds::MainViewer::~MainViewer ()
{
  makeCurrent ();

                                 // Delete lists to free up some
                                 // memory. TODO: Document the *magic*
                                 // number 125.
  for (unsigned int i=0; i<125; ++i) 
    glDeleteLists (sphereList[i], 1);
}

QSize vecds::MainViewer::minimumSizeHint () const
{
  return QSize (300, 225);
}

QSize vecds::MainViewer::sizeHint() const
{
  return QSize (800, 600);
}

// ----- START REIMPLEMENTED FROM QT -----

                                 // This function redefines a virtual
                                 // in Qt. This initialises the GL.
void vecds::MainViewer::initializeGL ()
{
  GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  
  glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv (GL_LIGHT0, GL_POSITION, light0_position);
  glEnable (GL_LIGHT1);
  glLightfv (GL_LIGHT1, GL_DIFFUSE, diffuse);
  glLightfv (GL_LIGHT1, GL_POSITION, light1_position);
  glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 30.0);
  glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,  specular_color);
  
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable (GL_TEXTURE_2D);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  qWarning ("class MainViewer: GL successfully initialized");
}

                                 // This function redefines a virtual
                                 // in Qt. It is generally alled
                                 // *after* initializeGL and simply
                                 // resizes the window.
void vecds::MainViewer::resizeGL (int width, int height)
{

  qWarning ("class MainViewer (resizeGL): Resizing GL");

                                 // prevent divide by 0 error when
                                 // window is minimised
  if (width==0)
    {
      height = 1;
    }
  
  glViewport (0, 0, width, height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (fov, (float (width/height)), 0.1, 50000);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

                                 // Set screen widths itd
  this->screenW = double (width);
  this->screenH = double (height);

                                 // Set arc ball rolling...
  qWarning ("class MainViewer (resizeGL): Setting up an arcball");
  arcball->set_bounds (width, height);

  qWarning ("class MainViewer (resizeGL): Resizing GL all done");
}

                                 // This function redefines a virtual
                                 // in Qt. It is generally alled
                                 // *after* initializeGL and after
                                 // resizeGL, and does some kind of
                                 // painting job.
void vecds::MainViewer::paintGL ()
{
  qWarning ("class MainViewer (paintGL): Starting to paint");

  glClear (GL_COLOR_BUFFER_BIT | 
	   GL_DEPTH_BUFFER_BIT);

  glClearColor (bg_red, bg_green, bg_blue, 1.0);

  if ((ActualData->atoms_loaded=="none") && (ActualData->img_loaded=="none")) 
    {
      qWarning ("No atoms loaded, no images loaded. What did you expect?");
      return;
    }

  glLoadIdentity();
  double ddx = 1.75 * (max_.x() - min_.x()) * d_x;
  double ddy = 1.75 * (max_.y() - min_.y()) * d_y;

  glGetDoublev (GL_MODELVIEW_MATRIX, model_view);
  glGetDoublev (GL_PROJECTION_MATRIX, projection);
  glGetIntegerv (GL_VIEWPORT, viewport);
  glTranslated (ddx, ddy, distance);

  glMultMatrixd (transformM);

  if (ActualData->img_loaded!="none") 
    {      
      glPushMatrix();
      glDisable(GL_LIGHTING);
      glEnable(GL_TEXTURE_2D);
      
      glBindTexture(GL_TEXTURE_2D, background);
      
      glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0);          glVertex3d(xl, yd, 0.0);   //(pld.x(), pld.y(), pld.z());
      glTexCoord2f(0.0, 1.0);          glVertex3f(xl, yu, 0.0);   // (plu.x(), plu.y(), plu.z());
      glTexCoord2f(1.0, 1.0);          glVertex3f(xr, yu, 0.0);   // (pru.x(), pru.y(), pru.z());
      glTexCoord2f(1.0, 0.0);          glVertex3f(xr, yd, 0.0);   // (prd.x(), prd.y(), prd.z());
      glEnd();
      
      glDisable(GL_TEXTURE_2D);
      
      glClear(GL_DEPTH_BUFFER_BIT);
      glEnable(GL_LIGHTING);
      
      glPopMatrix();
    }
  
  if ( ActualData->atoms_loaded!="none" ) 
    {
      if ( ActualData->atoms->n_atoms>0 ) 
	{
	  qWarning("VIEWER - atoms->n_atoms=%d", ActualData->atoms->n_atoms);
	  qWarning("xl, xr, yu, yd -- %g %g %g %g", xl, xr, yu, yd);
	  glPushMatrix();
	  draw_atoms();
	  glPopMatrix();
	}
      if ( ActualData->atoms->n_bonds>0 && ActualData->visible[1] ) 
	{
	  glPushMatrix();
	  draw_bonds();
	  glPopMatrix();
	}
    }
  if ( ActualData->visible[5] ) 
    {
      glPushMatrix();
      draw_axis();
      glPopMatrix();
    }
  
  if ( ActualData->ndisl>0 )
    {
      glPushMatrix();
      doGLdisloc();
      glPopMatrix();
      qWarning("VIEWER - ndisl=%d", ActualData->ndisl);
    }

  qWarning ("class MainViewer (paintGL): Successfully finished painting");
}

// ----- END REIMPLEMENTED FROM QT -----









//-----------------------------------------------------------------------------------

void vecds::MainViewer::set_defaults ()
{
  this->dist0 = -1.8;
  this->fov   = ActualData->set0->fov;
}

void vecds::MainViewer::prepare_scene ()
{
  if (ActualData->atoms_loaded=="none") 
    {
      qWarning("******** none atoms, none fems *********");
      min_ = QVector3D (-5., -5., -5);
      max_ = QVector3D (5., 5., 5.);
      rad_scene = 10.;
    } 
  else 
    {
      min_ = ActualData->a_min_;
      max_ = ActualData->a_max_;
      rad_scene = qMax (qMax ((max_.x ()-min_.x ()), (max_.y ()-min_.y ())), (max_.z ()-min_.z ()));
      qWarning("+++++++++++++++++ min_ = %g %g %g", min_.x(), min_.y(), min_.z());
    }

  distance0 = distance = (dist0/tan (fov*vecds::constant::deg2rad))*rad_scene;

  small     = 0.01 * rad_scene;
  smaller   = 0.22 * small;
  ActualData->rad_scene = rad_scene;
  ActualData->min_      = min_;
  ActualData->max_      = max_;
  
  xl = min_.x() - cent_.x();
  xr = max_.x() - cent_.x();
  yd = min_.y() - cent_.y();
  yu = max_.y() - cent_.y();
  ActualData->cent_ = cent_;

  makeCurrent();
  prepare_axis();

  std::cout << "class MainViewer (prepare_scene): successfully initialized the scene." << std::endl;
}

void vecds::MainViewer::prepare_invbox (const QVector3D xmin, 
					const QVector3D xmax)
{
  double x1 = xmin.x ();
  double x2 = xmax.x ();
  double y1 = xmin.y ();
  double y2 = xmax.y ();
  double z1 = xmin.z ();
  double z2 = xmax.z ();
  ActualData->invbox[0] = xmin;
  ActualData->invbox[1] = QVector3D (x1, y1, z2);
  ActualData->invbox[2] = QVector3D (x1, y2, z2);
  ActualData->invbox[3] = QVector3D (x1, y2, z1);
  ActualData->invbox[4] = QVector3D (x2, y1, z1);
  ActualData->invbox[5] = QVector3D (x2, y1, z2);
  ActualData->invbox[6] = xmax;
  ActualData->invbox[7] = QVector3D (x2, y2, z1);
}

void vecds::MainViewer::prepare_axis ()
{
  double length = 0.1 * rad_scene;

  ActualData->axeX = QVector3D (length, 0.0, 0.0);
  ActualData->axeY = QVector3D (0.0, length, 0.0);
  ActualData->axeZ = QVector3D (0.0, 0.0, length);
}

// ------------------------------------------------------------

void vecds::MainViewer::init_spheres( int numbOfSubdiv )
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

}

//-----------------------------------------------------------------------------------

void vecds::MainViewer::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    mousePt = normalizeMouse(lastPos);

    if ( ActualData->Mode==2 ) 
      {
	QVector3D res = getOGLPos(event->x(), event->y());
	if (event->buttons() & Qt::LeftButton) 
	  {
	    emit SIG_actPoint(res);
	    updateGL();
	  }
	if (event->buttons() & Qt::RightButton) 
	  {
	    emit SIG_actPosition(res);
	  } 
      } 
    else 
      {
	if (event->buttons() & Qt::LeftButton) 
	  {
	    mousePos = getMousePos(event->x(), event->y());

                                 // Update start vector and prepare
                                 // For dragging
	    arcball->click (mousePt);
	  }
      }

}

void vecds::MainViewer::mouseMoveEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    mousePt = normalizeMouse(lastPos);
    if (event->buttons() & Qt::LeftButton) 
      {
	arcball->drag(mousePt);
	
	quat2matr (arcball->quaternion);
	QVector3D result = quat2euler (arcball->quaternion)*vecds::constant::rad2deg;
	
	if (rangle_theta!=result.x ()) 
	  {
	    rangle_theta = result.x ();

	    if (rangle_theta<-180.) 
	      rangle_theta += 360.;

	    if (rangle_theta>180.) 
	      rangle_theta -= 360.;

	    emit SIG_thetaRotationChanged (int (rangle_theta));
	  }
	if (rangle_phi!=result.z ()) 
	  {
	    rangle_phi = result.z ();

	    if (rangle_phi<-90.) 
	      rangle_phi += 180.;

	    if (rangle_phi>90.) 
	      rangle_phi -= 180.;

	    emit SIG_phiRotationChanged (int (rangle_phi));
	  }
	if (rangle_psi!=result.y ()) 
	  {
	    rangle_psi = result.y ();

	    if (rangle_psi<-180.) 
	      rangle_psi += 180.;
	    if (rangle_psi>180.) 
	      rangle_psi -= 180.;

	    emit SIG_psiRotationChanged (int (rangle_psi));
	  }

       updateGL();
      } 
    else if (event->buttons() & Qt::RightButton && ActualData->Mode!=2) 
      {
	int mmx = event->x() - lastPos.x();
	int mmy = event->y() - lastPos.y();

	if ( mmx!=0 ) 
	  {
	    d_x += 0.002*mmx;
	    emit SIG_xMovementChanged(int(100*d_x));
	    updateGL();
	  }
	if ( mmy!=0 ) 
	  {
	    d_y -= 0.002*mmy;
	    emit SIG_yMovementChanged(int(100*d_y));
	    updateGL();
	  }
      }
}

void vecds::MainViewer::wheelEvent(QWheelEvent *event)
{
  if (event->delta()<0)  
    distance *= 0.99;
  else                     
    distance *= 1.01;

  if (distance<distance0) 
    {
      distance = distance0;
      return;
    }

  emit SIG_zMovementChanged(int(180*distance/distance0));
  updateGL();
}

QVector2D vecds::MainViewer::normalizeMouse(QPoint qp)
{
  QVector2D res = QVector2D (arcball->invWidth * (double(qp.x()) - 0.5*screenW),
			     arcball->invHeight * (0.5*screenH - double(qp.y())));
  
  return res;
}

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

void vecds::MainViewer::draw_atoms ()
{
  for (unsigned int i=0; i<ActualData->atoms->n_atoms; i++) 
    {
      unsigned int aki = ActualData->atoms->atom_type[i];
      QVector3D trans = ActualData->atoms->coordinates[i] - cent_;
      if ( i==0 ) 
	{
	  qWarning ("aki=%d, trans=(%g, %g, %g)", aki, trans.x(), trans.y(), trans.z());
	}
      glPushMatrix();
      glTranslated(trans.x(), trans.y(), trans.z());
      a_color[0] = ActualData->ap->atom_red[aki];
      a_color[1] = ActualData->ap->atom_green[aki];
      a_color[2] = ActualData->ap->atom_blue[aki];
      a_color[3] = 1.0;

      glMaterialfv(GL_FRONT, GL_AMBIENT, a_color);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, a_color);
      glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

      glCallList (sphereList[aki]);
      glPopMatrix();
    }
  makeCurrent();
}

void vecds::MainViewer::draw_bonds ()
{
  glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, grey);
 
  for (unsigned int i=0; i<ActualData->atoms->n_bonds; i++) 
    {
      int a1 = ActualData->atoms->atom_bond[i].i1 - 1;
      int a2 = ActualData->atoms->atom_bond[i].i2 - 1;
      
      glPushMatrix();
      
      QVector3D temp1 = ActualData->atoms->coordinates[a2] - ActualData->atoms->coordinates[a1];
      double dist = temp1.length();
      double invnorm = 1.0 / dist;
      QVector3D temp2 = QVector3D(-temp1.y()*invnorm, temp1.x()*invnorm, 0.0);
      QVector3D trans = ActualData->atoms->coordinates[a1] - cent_;
      
      glTranslated (trans.x(), trans.y(), trans.z());
      glRotated (vecds::constant::rad2deg*acos(temp1.z()*invnorm), temp2.x(), temp2.y(), temp2.z());
      
      GLUquadricObj *qobj = gluNewQuadric();
      gluCylinder(qobj, 0.05, 0.05, dist, 16, 8);
      gluDeleteQuadric(qobj);
      
      glPopMatrix();
    }
  makeCurrent();
}


void vecds::MainViewer::draw_axis ()
{
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
  
  makeCurrent();
}

void vecds::MainViewer::arrow (QVector3D orig, QVector3D vect, double fact, double sm)
{
  vect           *= (1.0 - fact);
  double dist1    = vect.length();
  double dist2    = dist1 * ( fact/(1.0-fact) );
  double invnorm  = 1.0 / dist1;
  QVector3D temp1 = QVector3D(-vect.y()*invnorm, vect.x()*invnorm, 0.0);
  double ang      = vecds::constant::rad2deg*acos(vect.z()*invnorm);

  glTranslated(orig.x(), orig.y(), orig.z());
  glRotated(ang, temp1.x(), temp1.y(), temp1.z());
  GLUquadricObj *qobj = gluNewQuadric();
  gluCylinder(qobj, sm, sm, dist1, 16, 8);
  gluDeleteQuadric(qobj);
  glTranslated(0.0, 0.0, dist1);
  qobj = gluNewQuadric();
  gluCylinder(qobj, 1.5*sm, 0.0, dist2, 16, 8);
  gluDeleteQuadric(qobj);
}

void vecds::MainViewer::SL_dothetaRotation ()
{
  if (ActualData->sliderMove) 
    {
      double angle = double (ActualData->sliderValue);
//      double angle = ActualData->sliderValue;

      if (angle<-180.) 
	angle += 360.;

      if (angle>180.) 
	angle -= 360.;

      rangle_theta           = double (angle);
      ActualData->sliderMove = false;
      euler2matr ();
      arcball->quaternion    = quatfromEuler ();
      updateGL ();
    }
}

void vecds::MainViewer::SL_dophiRotation ()
{
  if (ActualData->sliderMove) 
    {
      double angle = double (ActualData->sliderValue);
//      double angle = ActualData->sliderValue;
      if (angle<-90.) 
	angle += 180.;

      if (angle>90.) 
	angle -= 180.;

      rangle_phi             = angle;
      ActualData->sliderMove = false;
      euler2matr ();
      arcball->quaternion    = quatfromEuler ();
      updateGL ();
    }
}

void vecds::MainViewer::SL_dopsiRotation ()
{

  if (ActualData->sliderMove) 
    {
      double angle = double (ActualData->sliderValue);
//      double angle = ActualData->sliderValue;
      if (angle<-180.) 
	angle += 360.;

      if (angle>180.) 
	angle -= 360.;

      rangle_psi             = double (angle);
      ActualData->sliderMove = false;
      euler2matr ();
      arcball->quaternion    = quatfromEuler ();
      updateGL ();
    }
}

void vecds::MainViewer::SL_doXMovement ()
{
  if (ActualData->sliderMove) 
    {
      d_x = 0.01*double(ActualData->sliderValue);
      ActualData->sliderMove = false;
      updateGL();
    }
  qWarning("...X move - %g", d_x);
}

void vecds::MainViewer::SL_doYMovement ()
{
  if ( ActualData->sliderMove ) 
    {
      d_y = 0.01*double(ActualData->sliderValue);
      ActualData->sliderMove = false;
      updateGL();
    }
}

void vecds::MainViewer::SL_doZMovement ()
{
  if ( ActualData->sliderMove ) 
    {
      d_0 = double(ActualData->sliderValue)/180.;
      ActualData->sliderMove = false;
      distance = d_0 * distance0;
      updateGL();
    }
}

void vecds::MainViewer::SL_needDraw ()
{
  qWarning("slot needDraw");
  prepare_scene();

  paintGL();
}

void vecds::MainViewer::SL_repaint ()
{
  paintGL();
}

void vecds::MainViewer::SL_keypress (int key)
{
  qWarning("slot: key = %d", key);
}


void vecds::MainViewer::SL_loadImage ()
{
  background = Pixmap2texture(&ActualData->img);
}

GLuint vecds::MainViewer::Pixmap2texture (QPixmap *pixmap)
{
  GLuint tex;

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


GLuint vecds::MainViewer::image2texture (QImage* bmp)
{
  GLuint tex;
  // 1E-3 needed. Just try with width=128 and see !
  int width0    = bmp->width();
  int height0   = bmp->height();
  int newWidth  = 1<<(int)(1+log(width0 -1+1E-3) / log(2.0));
  int newHeight = 1<<(int)(1+log(height0-1+1E-3) / log(2.0));
  
  if ( ( width0!=newWidth ) || ( height0!=newHeight ) ) 
    {
      qWarning("Image size set to %dx%d pixels", newWidth, newHeight);
      bmp->copy(0, 0, newWidth, newHeight);
    }
  // Bind the img texture...
  tex = bindTexture(*bmp, GL_TEXTURE_2D);
  //  delete *pixmap;
  return tex;
}

//-----------------------------------------------------------------------------------



QVector3D vecds::MainViewer::getOGLPos (int x, int y)
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

QVector3D vecds::MainViewer::getMousePos (int x, int y)
{
  GLfloat winX, winY, winZ;
  GLdouble posX, posY, posZ;
  winX = float(x);
  winY = float(viewport[3]) - float(y);
  
  winZ = 1.;
  
  gluUnProject( winX, winY, winZ, model_view, projection, viewport,
		&posX, &posY, &posZ);
  
  qWarning(" +++ GetMousePos  mouse position:  (%g, %g, %g) (%g, %g, %g)",
	   winX, winY, winZ, posX, posY, posZ);
  return QVector3D(posX, posY, posZ);
}

void vecds::MainViewer::doGLdisloc ()
{
  if ( ActualData->ndisl<=0 ) return;
  for (int i=0; i<ActualData->ndisl; ++i) 
    { 
      qWarning("doGLdisloc  --   rrr=%g, %g, %g", ActualData->disl[i].rrr.x(), ActualData->disl[i].rrr.y(),
	       ActualData->disl[i].rrr.z());
      glPushMatrix();
      QVector3D p1    = ActualData->disl[i].p1 - cent_;
      QVector3D p2    = ActualData->disl[i].p2 - cent_;
      QVector3D temp1 = p2 - p1;
      double dist     = temp1.length();
      double invnorm  = 1.0 / dist;
      QVector3D temp2 = QVector3D(-temp1.y()*invnorm, temp1.x()*invnorm, 0.0);
      
      glTranslated (p1.x(), p1.y(), p1.z());
      glRotated (vecds::constant::rad2deg*acos(temp1.z()*invnorm), temp2.x(), temp2.y(), temp2.z());
      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cube_mat11);
      GLUquadricObj *qobj = gluNewQuadric();
      gluCylinder(qobj, 0.15, 0.15, dist, 16, 8);
      gluDeleteQuadric(qobj);
      glPopMatrix();
    }
}

// ---------------------------------------

void vecds::MainViewer::drawBox(double v[8][3])//, double norm_v[8][3])
{
  static GLint _faces[6][4] = 
    {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
    };
  
  for (int i=5; i>=0; --i) 
    {
      glBegin(GL_QUADS);
      glVertex3dv(&v[_faces[i][0]][0]);
      glVertex3dv(&v[_faces[i][1]][0]);
      glVertex3dv(&v[_faces[i][2]][0]);
      glVertex3dv(&v[_faces[i][3]][0]);
      glEnd();
    }
}

void vecds::MainViewer::drawBox0(float size)
{
   static GLfloat n[6][3] = 
     {
       {-1.0,  0.0,  0.0},
       { 0.0,  1.0,  0.0},
       { 1.0,  0.0,  0.0},
       { 0.0, -1.0,  0.0},
       { 0.0,  0.0,  1.0},
       { 0.0,  0.0, -1.0}
     };

   static GLint _faces[6][4] = 
     {
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
   v[4][0] = v[5][0] = v[6][0] = v[7][0] =  size / 2;
   v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
   v[2][1] = v[3][1] = v[6][1] = v[7][1] =  size / 2;
   v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
   v[1][2] = v[2][2] = v[5][2] = v[6][2] =  size / 2;
   
   for (i = 5; i >= 0; i--) 
     {
       glBegin(GL_QUADS);
       glNormal3fv(&n[i][0]);
       glVertex3fv(&v[_faces[i][0]][0]);
       glVertex3fv(&v[_faces[i][1]][0]);
       glVertex3fv(&v[_faces[i][2]][0]);
       glVertex3fv(&v[_faces[i][3]][0]);
       glEnd();
     }
}

void vecds::MainViewer::euler2matr ()
{
                                 // Assume the angles are in passed in
                                 // in radians.
  double ch = cos (vecds::constant::deg2rad * rangle_theta); // heading
  double sh = sin (vecds::constant::deg2rad * rangle_theta);
  double ca = cos (vecds::constant::deg2rad * rangle_psi);   // altitude
  double sa = sin (vecds::constant::deg2rad * rangle_psi);
  double cb = cos (vecds::constant::deg2rad * rangle_phi);   // bank
  double sb = sin (vecds::constant::deg2rad * rangle_phi);

    transformM[0]  = ch*ca;
    transformM[1]  = sh*sb - ch*sa*cb;
    transformM[2]  = ch*sa*sb + sh*cb;
    transformM[3]  = 0.;
    transformM[4]  = sa;
    transformM[5]  = ca*cb;
    transformM[6]  = -ca*sb;
    transformM[7]  = 0.;
    transformM[8]  = -sh*ca;
    transformM[9]  = sh*sa*cb + ch*sb;
    transformM[10] = -sh*sa*sb + ch*cb;
    transformM[11] = 0.;
    transformM[12] = 0.;
    transformM[13] = 0.;
    transformM[14] = 0.;
    transformM[15] = 1.;
}

void vecds::MainViewer::quat2matr (QQuaternion q)
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


QVector3D vecds::MainViewer::quat2euler (QQuaternion q)
{
  double head, att, b;
  double test = q.x()*q.y() + q.z()*q.scalar();
  if (test > 0.499) 
    { // singularity at north pole
      head = 2.*atan2(q.x(), q.scalar());
      att = vecds::constant::pi_2;
      b = 0.;
      return QVector3D(head, att, b);
    }
  if (test < -0.499) 
    { // singularity at south pole
      head = -2.*atan2(q.x(), q.scalar());
      att = - vecds::constant::pi_2;
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

QQuaternion vecds::MainViewer::quatfromEuler ()
{
  //   QQuaternion res;
  double heading2  = 0.5 * vecds::constant::deg2rad * rangle_theta;
  double attitude2 = 0.5 * vecds::constant::deg2rad * rangle_psi;
  double bank2     = 0.5 * vecds::constant::deg2rad * rangle_phi;
  
  double c1   = cos (heading2);
  double s1   = sin (heading2);
  double c2   = cos (attitude2);
  double s2   = sin (attitude2);
  double c3   = cos (bank2);
  double s3   = sin (bank2);
  double c1c2 = c1*c2;
  double s1s2 = s1*s2;
  return QQuaternion(c1c2*c3 - s1s2*s3, 
		     c1c2*s3 + s1s2*c3, s1*c2*c3 + c1*s2*s3, c1*s2*c3 - s1*c2*s3);
}

void vecds::MainViewer::getAxisAngle (const QQuaternion q, QVector3D &v, double &ang)
{
  double temp_angle = acos(q.scalar());
  double scale = sqrt(q.x()*q.x() + q.y()*q.y() + q.z()*q.z());
  double epsilon = 1.e-10;
  if ( scale<epsilon )  
    {
      ang = 0.;
      v = QVector3D(0., 0., 1.);
    } 
  else 
    {
      ang = temp_angle + temp_angle;
      scale = 1./scale;
      v = QVector3D(q.x()*scale, q.y()*scale, q.z()*scale);
      v = v.normalized();
    }
  return;
}
