#include <QtGui>
#include <QtOpenGL>
#include <GL/glut.h>
#include <vecds/gui/main_viewer.h>
#include <vecds/base/internal.h>
#include <vecds/base/constant.h>

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
  this->phiRot           = 0;
  this->thetaRot         = 0;
  this->psiRot           = 0.;
  this->d_x              = 0.;
  this->d_y              = 0.;
  this->d_0              = 180.;
  this->n_atoms          = 0;
  this->n_bonds          = 0;
  this->n_elements       = 0;
  this->bg_red           = 0.99;
  this->bg_green         = 0.99;
  this->bg_blue          = 1.0;
  this->VIEW_rad_fact    = 0.25;
  this->VIEW_whichRadius = 1;
  this->mousePos         = QVector3D(0., 0., 0.);

  init_spheres (8);

  // Then deal with the colour scheme we are using:
  colour_spectrum = new QColor[12];
  for (int i=0; i<12; i++) 
    colour_spectrum[i] = ActualData->set0->colour_spectrum[i];

  set_defaults ();
  prepare_scene ();
  setFont (QFont ("Times", 12));
  arcb       = new ArcBall ();
//  transformM = new double[16];
/*
  for (unsigned int i=1; i<15; i++) 
    transformM[i] = 0.;

  for (unsigned int i=0; i<16; i+=5) 
    transformM[i] = 1.;
*/
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
  this->dist0 = -1.8;
  this->fov = ActualData->set0->fov;
}

void MainViewer::init_faces()
{
  QString name;
  QImage *ActSpectr;
  int n_res = 3;
  int n_steps = 12;
  double min_val = -100.;
  double step = 5.6;
  ActualData->calc_faces();
  for (int i=0; i<ActualData->numbFaces; i++) 
    {
      ActSpectr = new QImage(64, 64, QImage::Format_ARGB32_Premultiplied);
      QPainter imgpainter(ActSpectr);
      imgpainter.initFrom(this);
      imgpainter.setRenderHint(QPainter::Antialiasing, true);
      imgpainter.eraseRect(rect());
      Int4 fq = ActualData->faces[i].f_quad;
      int nrel = ActualData->faces[i].nr_elem;
      int p1 = ActualData->elems->el_nodes[nrel][fq.i1];
      int p2 = ActualData->elems->el_nodes[nrel][fq.i2];
      int p3 = ActualData->elems->el_nodes[nrel][fq.i3];
      int p4 = ActualData->elems->el_nodes[nrel][fq.i4];
      draw(&imgpainter, Int4(p1, p2, p3, p4), n_res, min_val, step, n_steps);
      imgpainter.end();
      ActualData->faces[i].tex = image2texture(ActSpectr);
      //     name.sprintf("image%d.png", i);
      //     (*ActSpectr).save(name);
    }
  // qWarning ("init_faces -- end");
}

void MainViewer::calculate_size ()
{
  max_.setX(qMax(ActualData->a_max_.x(), ActualData->e_max_.x()));
  min_.setX(qMin(ActualData->a_min_.x(), ActualData->e_min_.x()));
  max_.setY(qMax(ActualData->a_max_.y(), ActualData->e_max_.y()));
  min_.setY(qMin(ActualData->a_min_.y(), ActualData->e_min_.y()));
  max_.setZ(qMax(ActualData->a_max_.z(), ActualData->e_max_.z()));
  min_.setZ(qMin(ActualData->a_min_.z(), ActualData->e_min_.z()));
  cent_ = (max_ + min_) * 0.5;
  rad_scene = qMax(max_.x()-min_.x(), qMax(max_.y()-min_.y(), max_.z()-min_.z()));
  qWarning("calc_size: rad_scene=%g, distance=%g", rad_scene, distance);
//  qWarning("min-mx: x:%g-%g, y:%g-%g, z:%g-%g", 
//             min_.x(), max_.x(), min_.y(), max_.y(), min_.z(), max_.z());

  if (ActualData->atoms_loaded=="none" && ActualData->fems_loaded =="none") {
      qWarning("******** none atoms, none fems *********");
      min_ = QVector3D(-5., -5., -5);
      max_ = QVector3D(5., 5., 5.);
      rad_scene = 10.;
    }
}


void MainViewer::prepare_scene()
{
  calculate_size ();
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
//  prepare_invbox(min_ , max_);
  //  prepare_invbox(min_ - cent_, max_ - cent_);
  prepare_axis();
  qWarning(" ---------- prepare_scene end");
//  updateGL();

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

  ActualData->invbox[0] = xmin; // QVector3D (x1, y1, z1);
  ActualData->invbox[1] = QVector3D(x1, y1, z2);
  ActualData->invbox[2] = QVector3D(x1, y2, z2);
  ActualData->invbox[3] = QVector3D(x1, y2, z1);
  ActualData->invbox[4] = QVector3D(x2, y1, z1);
  ActualData->invbox[5] = QVector3D(x2, y1, z2);
  ActualData->invbox[6] = xmax;//QVector3D(x2, y2, z2);
  ActualData->invbox[7] = QVector3D(x2, y2, z1);
}

void MainViewer::prepare_axis ()
{
  double ll = 0.1 * rad_scene;
  ActualData->axeX = QVector3D(ll, 0.0, 0.0);
  ActualData->axeY = QVector3D(0.0, ll, 0.0);
  ActualData->axeZ = QVector3D(0.0, 0.0, ll);
}

void MainViewer::init_spheres ( int numbOfSubdiv )
{ 
  // qWarning ("init_spheres");
  for (int i=0; i<125; i++ ) {
      GLUquadricObj *obj = gluNewQuadric();
      gluQuadricNormals(obj, GL_SMOOTH);
      gluQuadricDrawStyle(obj, GLU_FILL);
      sphereList[i] = glGenLists(1);

      glNewList(sphereList[i], GL_COMPILE);
      if (VIEW_whichRadius==1)
        gluSphere (obj, 
		   ActualData->ap->a_rad1[i]*VIEW_rad_fact, 
		   numbOfSubdiv*2, 
		   numbOfSubdiv);
      else
        gluSphere (obj, 
		   ActualData->ap->a_rad2[i]*VIEW_rad_fact, 
		   numbOfSubdiv*2, 
		   numbOfSubdiv);
      
     glEndList ();
     gluDeleteQuadric(obj);
  }

  cubeList = glGenLists(1);
  
  glNewList(cubeList, GL_COMPILE);
  
  draw_quad(smaller);
  
  glEndList();
}

//-----------------------------------------------------------------------------------

void MainViewer::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    mousePt = arcb->normalizeMouse(event->pos());

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
       if ( event->buttons()&Qt::LeftButton ) {
          mousePos = getMousePos(event->x(), event->y());
          arcb->click(mousePt);//Update Start Vector And Prepare For Dragging
// qWarning("<--------  startVec = (%g %g %g)", arcb->startVec.x, arcb->startVec.y, arcb->startVec.z);
//  qWarning(" Mouse = (%g %g %g)", mousePos.x, mousePos.y, mousePos.z);
       }// else {
//          arcb->quaternion = Quat(0., 0., 0., 1.);
//          quat2matr(arcb->quaternion);
//       }
    }
    event->accept();
    updateGL();
//    thisRot = lastRot;

}

void MainViewer::mouseMoveEvent (QMouseEvent *event)
{
  lastPos = event->pos();
//  mousePt = normalizeMouse(lastPos);
  mousePt = arcb->normalizeMouse(event->pos());
  if ( event->buttons()&Qt::LeftButton && arcb->is_used ) {
      arcb->drag(mousePt);
      // qWarning("--------> endVec = (%g %g %g)",
      //                 arcb->startVec.x, arcb->startVec.y, arcb->startVec.z,
      //                           arcb->endVec.x, arcb->endVec.y, arcb->endVec.z);
      arcb->get_matrix();
      QVector3D result = quat2euler(arcb->quaternion)*constant::rad2deg;
      // qWarning("Angles = %g %g %g",
      //          constant::rad2deg*result.x, rad2deg*result.y, rad2deg*result.z);
      if ( thetaRot!=result.x() ) {
	  thetaRot = result.x();
	  if ( thetaRot<-180. ) thetaRot += 360.;
	  if ( thetaRot>180. ) thetaRot -= 360.;
          emit SIG_thetaRotationChanged(thetaRot);
	}
      if ( psiRot!=result.y() ) {
	  psiRot = result.y();
	  if ( psiRot<-180. ) psiRot += 180.;
          if ( psiRot>180. ) psiRot -= 180.;
          emit SIG_psiRotationChanged(psiRot);
      }
      if ( phiRot!=result.z() ) {
	  phiRot = result.z();
	  if ( phiRot<-90. ) phiRot += 180.;
	  if ( phiRot>90. ) phiRot -= 180.;
	  emit SIG_phiRotationChanged(phiRot);
      }
      // qWarning("Angles = %g %g %g", thetaRot, phiRot, psiRot);
//      updateGL();
    } else {
        arcb->is_used = false;
        if ( event->buttons() & Qt::RightButton && ActualData->Mode!=2 ) {
           int mmx = event->x() - lastPos.x();
           int mmy = event->y() - lastPos.y();
           if ( mmx!=0 ) {
              d_x += 0.002*mmx;
              emit SIG_xMovementChanged(d_x);
              updateGL();
           }
        }
        if (mmy!=0) {
           d_y -= 0.002*mmy;
           emit SIG_yMovementChanged (d_y);
//           updateGL();
        }
    }
    event->accept();
    updateGL();
}

void MainViewer::mouseReleaseEvent(QMouseEvent *event)
{
//  lastPos = event->pos();
//  mousePt = normalizeMouse(lastPos);
   if ( event->buttons() & Qt::LeftButton ) arcb->is_used = false;
}

void MainViewer::wheelEvent(QWheelEvent *event)
{
   if ( event->delta()<0 )  distance *= 0.99;
   else                     distance *= 1.01;
   if ( distance<distance0 ) {
      distance = distance0;
      return;
   }
   emit SIG_zMovementChanged(int(180.*(distance/distance0)));
   updateGL();
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
  gluPerspective(fov, float(w/h), 0.1, 50000);
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
// qWarning("paintGL begin");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(bg_red, bg_green, bg_blue, 1.0);

  if ( ActualData->atoms_loaded=="none" && ActualData->fems_loaded=="none" 
                                    && ActualData->img_loaded=="none") return;
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
  glMultMatrixd(arcb->rotM);
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

  if ( ActualData->showU ) {
     glPushMatrix();
     draw_arrows(1.);
     glPopMatrix();
  }


  if ( ActualData->atoms_loaded!="none" ) {
     if ( ActualData->atoms->n_atoms>0 && ActualData->visible[0] ) {
//        glPushMatrix();
        draw_atoms();
//        glPopMatrix();
     }
     if ( ActualData->atoms->n_bonds>0 && ActualData->visible[1] ) {
        glPushMatrix();
        draw_bonds();
        glPopMatrix();
     }
// qWarning("paintGL -- atoms");
// qWarning("distance=%g, rad_scene=%g, n_atoms=%d", distance, rad_scene, ActualData->atoms->n_atoms);
  }

  if ( ActualData->fems_loaded!="none" ) {
     glPushMatrix();
     draw_faces1(0.);
     glPopMatrix();
// qWarning("paintGL -- fems");

//     glPushMatrix();
//     draw_numbers();
//     glPopMatrix();
     glPushMatrix();
     draw_nod_disp(0.);
     glPopMatrix();
  }

  if ( ActualData->visible[5] ) {
     glPushMatrix();
     draw_axis();
     glPopMatrix();
  }

  if ( ActualData->n_dislocations>0 )
    {
      glPushMatrix();
      doGLdisloc();
      glPopMatrix();
      qWarning("VIEWER - n_dislocations=%d", ActualData->n_dislocations);
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
     if ( !ActualData->atoms->marked[i] ) continue;
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
 int *d = ActualData->atoms->bonds.data();
 for (unsigned int i=0; i<ActualData->atoms->n_bonds; i++) {
     int a1 = ActualData->atoms->bonds.at(2*i) - 1;
     int a2 = ActualData->atoms->bonds.at(2*i+1) - 1;

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

void MainViewer::draw_line (QVector<int> line, double f)
{
  Q_ASSERT (line.size() == 2); 
  
  QVector3D p1 = ActualData->nodes->coords[line[0]]-cent_;
  QVector3D p2 = ActualData->nodes->coords[line[1]]-cent_;
  
  if (f!=0.) 
    {
      p1 += QVector3D(ActualData->results->res[line[1]][0]*f,
		      ActualData->results->res[line[1]][1]*f,
		      ActualData->results->res[line[1]][2]*f);
      p2 += QVector3D(ActualData->results->res[line[1]][0]*f,
		      ActualData->results->res[line[1]][1]*f,
		      ActualData->results->res[line[1]][2]*f);
    }

  glBegin(GL_LINES);
  glVertex3d(p1.x(), p1.y(), p1.z());
  glVertex3d(p2.x(), p2.y(), p2.z());
  glEnd();

}


void MainViewer::draw_faces1(double f)
{
  if (ActualData->numbFaces<=0) 
    return;


  glPushMatrix();
  //  glDisable(GL_LIGHTING);
  //  glEnable(GL_BLEND);
  for (int i=0; i<ActualData->numbFaces; i++) 
    {
      int nrel = ActualData->faces[i].nr_elem;
      int nl_int = ActualData->faces[i].n_intlines;
      //  qWarning("== draw faces ==  nrel, nl_int - %d %d", nrel, nl_int);
      if (ActualData->visible[4]) 
	{
	  glColor4f(0.05, 0.93, 0.1, 0.3);
	  
	  for (int j=0; j<nl_int; j++) 
	    {
	      int b1 = ActualData->elems->el_nodes[nrel][ActualData->faces[i].f_intlines[j][1]];
	      int b2 = ActualData->elems->el_nodes[nrel][ActualData->faces[i].f_intlines[j][2]];
	      
	      QVector<int> line = QVector<int> (b1, b2);
	      
	      draw_line (line, f);
	    }
	}

     if (ActualData->visible[5]) 
       {
	 glColor4f(0.65, 0.73, 0.1, 0.3);
	 
	 int nl_ext = ActualData->faces[i].n_extlines;
	 for (int j=0; j<nl_ext; j++) 
	   {
	     int b1 = ActualData->elems->el_nodes[nrel][ActualData->faces[i].f_extlines[j][1]];
	     int b2 = ActualData->elems->el_nodes[nrel][ActualData->faces[i].f_extlines[j][2]];
	     
	     QVector<int> line = QVector<int> (b1, b2);
	     
	     draw_line (line, f);
	   }
       }
    }

  glPopMatrix();

  if ( ActualData->res_loaded!="none" && 
       ActualData->visible[6]) 
    {
      
      glPushMatrix();
      
      for (int i=0; i<ActualData->numbFaces; i++) 
	{
	  GLuint tex = GLuint(ActualData->faces[i].tex);

	  Int4 fq  = ActualData->faces[i].f_quad;

	  int nrel = ActualData->faces[i].nr_elem;
	  int p1   = ActualData->elems->el_nodes[nrel][fq.i1];
	  int p2   = ActualData->elems->el_nodes[nrel][fq.i2];
	  int p3   = ActualData->elems->el_nodes[nrel][fq.i3];
	  int p4   = ActualData->elems->el_nodes[nrel][fq.i4];
	  //      GLuint tex = image2coordinate(Data->image[i]);
	  QVector3D pld = ActualData->nodes->coords[p1] - cent_;
	  QVector3D plu = ActualData->nodes->coords[p2] - cent_;
	  QVector3D pru = ActualData->nodes->coords[p3] - cent_;
	  QVector3D prd = ActualData->nodes->coords[p4] - cent_;
	  if ( f!=0. ) 
	    {
	      pld += QVector3D(ActualData->results->res[p1][0]*f,
			       ActualData->results->res[p1][1]*f,
			       ActualData->results->res[p1][2]*f);
	      plu += QVector3D(ActualData->results->res[p2][0]*f,
			       ActualData->results->res[p2][1]*f,
			       ActualData->results->res[p2][2]*f);
	      pru += QVector3D(ActualData->results->res[p3][0]*f,
			       ActualData->results->res[p3][1]*f,
			       ActualData->results->res[p3][2]*f);
	      prd += QVector3D(ActualData->results->res[p4][0]*f,
			       ActualData->results->res[p4][1]*f,
			       ActualData->results->res[p4][2]*f);
	    }
	  
	  glDisable(GL_LIGHTING);
	  glEnable (GL_BLEND);
	  glEnable(GL_TEXTURE_2D);
	  drawTexture(tex, pld, plu, pru, prd);
	  glDisable(GL_TEXTURE_2D);
	  glDisable (GL_BLEND);
	  glEnable(GL_LIGHTING);
	}

      glPopMatrix();
    }
}

void MainViewer::draw_nod_disp(double factor)
{
  if ( ActualData->INT_nn<=0 ) return;
//  if ( !ActualData->visible[3] ) return;
//  glColor4f(cube_mat1[0], cube_mat1[1], cube_mat1[2], cube_mat1[3]); 
//  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cube_mat2);
  for (unsigned int i=0; i<ActualData->INT_nn; i++) {
     glPushMatrix();
 
     QVector3D vc = ActualData->nodes->coords[i] - cent_;
     if ( factor!=0. )   vc += QVector3D(ActualData->results->res[i][0]*factor,
                                     ActualData->results->res[i][1]*factor,
                                     ActualData->results->res[i][2]*factor);
//  qWarning("A %d  -  (%g, %g, %g)", i, vc.x, vc.y, vc.z);
     glTranslated(vc.x(), vc.y(), vc.z());

     glCallList(cubeList);

     glPopMatrix();
  }
//  draw_numbers();
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

void MainViewer::draw_arrows(double f)
{
//  if ( ActualData->INT_nn<=0 ) return;
//  if ( !ActualData->visible[7] ) return;
//  glMaterialfv(GL_FRONT, GL_AMBIENT, colorRed);
//  glMaterialfv(GL_FRONT, GL_DIFFUSE, colorRed);
//  double delta = 0.01*rad_scene;
//  glColor4f(cube_mat1[0], cube_mat1[1], cube_mat1[2], cube_mat1[3]); 
//  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cube_mat2);
  for (unsigned int i=0; i<ActualData->atoms->n_atoms; i++) { 

     glPushMatrix();

  glMaterialfv(GL_FRONT, GL_AMBIENT, colorRed);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlue);
 
     QVector3D orig = ActualData->atoms->coord[i] - cent_ - ActualData->atoms->u[i];
     QVector3D size = ActualData->atoms->u[i]*f;
     arrow(orig, size, 0.3, smaller);

     glPopMatrix();
  }
}

void MainViewer::arrow(QVector3D orig, QVector3D vect, double fact, double sm)
{
   vect *= (1.0 - fact);
   double dist1 = vect.length();
   double dist2 = dist1 * ( fact/(1.0-fact) );
   double invnorm = 1.0 / dist1;
   QVector3D temp1 = QVector3D(-vect.y()*invnorm, vect.x()*invnorm, 0.0);
   double ang = constant::rad2deg*acos(vect.z()*invnorm);
   glPushMatrix();
   glTranslated(orig.x(), orig.y(), orig.z());
   glRotated(ang, temp1.x(), temp1.y(), temp1.z());
   GLUquadricObj *qobj = gluNewQuadric();
   gluCylinder(qobj, sm, sm, dist1, 16, 8);
   gluDeleteQuadric(qobj);
   glTranslated(0.0, 0.0, dist1);
   qobj = gluNewQuadric();
   gluCylinder(qobj, 1.5*sm, 0.0, dist2, 16, 8);
   gluDeleteQuadric(qobj);
   glPopMatrix();
}

void MainViewer::SL_dothetaRotation()
{
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
// qWarning("...Y ROTATION - %g", VS->y_rot);
}

void MainViewer::SL_dophiRotation()
{
  double angl;
  if ( ActualData->sliderMove ) {
     angl = double(ActualData->sliderValue);
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
     angl = double(ActualData->sliderValue);
     if ( angl<-180. ) angl += 360.;
     if ( angl>180. ) angl -= 360.;
     psiRot = double(angl);
     ActualData->sliderMove = false;
     euler2matr();
     arcb->quaternion = quatfromEuler();
     updateGL();
  }
// qWarning("...Z ROTATION - %g", VS->z_rot);
}

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
  int new_width  = 1<<(int)(1+log(width0 -1+1E-3) / log(2.0));
  int new_height = 1<<(int)(1+log(height0-1+1E-3) / log(2.0));
  if ( ( width0!=new_width ) || ( height0!=new_height ) ) {
     qWarning("Image size set to %dx%d pixels", new_width, new_height);
     pixmap->copy(0, 0, new_width, new_height);
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
  int new_width  = 1<<(int)(1+log(width0 -1+1E-3) / log(2.0));
  int new_height = 1<<(int)(1+log(height0-1+1E-3) / log(2.0));
  if ( ( width0!=new_width ) || ( height0!=new_height ) ) 
    {
      qWarning("Image size set to %dx%d pixels", 
	       new_width, new_height);
      //     qWarning("loadImage:  u_max, >v_max %g %g", tex.u_max, tex.v_max);
      bmp->copy(0, 0, new_width, new_height);
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
  if ( ActualData->n_dislocations<=0 ) 
    return;

  for (int i=0; i<ActualData->n_dislocations; ++i) 
    { 
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

void MainViewer::draw_quad (double v[8][3], double norm_v[8][3])
{
  // A quad has six faces and four corners on each face:
  static GLint quad[6][4] = 
    {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
    };
  
  for (unsigned int i=5; i>=0; --i) 
    {
      glBegin (GL_QUADS);
      
      glVertex3dv (&v[quad[i][0]][0]);
      glVertex3dv (&v[quad[i][1]][0]);
      glVertex3dv (&v[quad[i][2]][0]);
      glVertex3dv (&v[quad[i][3]][0]);
      
      glEnd ();
    }
}

void MainViewer::draw_quad (float size)
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

  static GLint quad[6][4] = 
    {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
    };
  GLfloat v[8][3];
  
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size/2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] =  size/2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size/2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] =  size/2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size/2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] =  size/2;

  for (GLint i = 5; i >= 0; --i) 
    {
      glBegin (GL_QUADS);
      glNormal3fv (&n[i][0]);
      glVertex3fv (&v[quad[i][0]][0]);
      glVertex3fv (&v[quad[i][1]][0]);
      glVertex3fv (&v[quad[i][2]][0]);
      glVertex3fv (&v[quad[i][3]][0]);
      glEnd ();
    }
}

void MainViewer::draw (QPainter* painter, Int4 pq, int n_res,
		       double min_val, double step, int n_steps)
{
  QPen pen(Qt::black, 0.5);
  QColor *colour_spectrum = new QColor[12];

  painter->setPen(pen);
  painter->fillRect(QRect (0, 0, 64, 64), 
		    QColor (255, 255, 255, 255));

  double rld = ActualData->results->res[pq.i1][n_res];
  double rlu = ActualData->results->res[pq.i2][n_res];
  double rru = ActualData->results->res[pq.i3][n_res];
  double rrd = ActualData->results->res[pq.i4][n_res];

  for (int i=0; i<n_steps; i++)
    colour_spectrum[i] = ActualData->set0->colour_spectrum[i];
  
  const double fact = 1./(63.*63.);

  for (int i=0; i<64; ++i) 
    for (int j=0; j<64; ++j) 
      {
        double ddd = 
	  rld*fact*double ((63-i)*j)      +
	  rrd*fact*double (i*j)           +
	  rlu*fact*double ((63-i)*(63-j)) +
	  rru*fact*double (i*(63-j));

        int k = int (ddd + min_val)/step;
        painter->setPen (colour_spectrum[k]);
        painter->drawPoint (i, j);
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

    arcb->rotM[0] = ch*ca;
    arcb->rotM[1] = sh*sb - ch*sa*cb;
    arcb->rotM[2] = ch*sa*sb + sh*cb;
    arcb->rotM[4] = sa;
    arcb->rotM[5] = ca*cb;
    arcb->rotM[6] = -ca*sb;
    arcb->rotM[8] = -sh*ca;
    arcb->rotM[9] = sh*sa*cb + ch*sb;
    arcb->rotM[10] = -sh*sa*sb + ch*cb;
    arcb->rotM[3] = arcb->rotM[7] = arcb->rotM[11] = 
    arcb->rotM[12] = arcb->rotM[13] = arcb->rotM[14] = 0.;
    arcb->rotM[15] = 1.;
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
QVector3D MainViewer::quat2euler (QQuaternion quaternion)
{
   double head = 0.;
   double att  = 0.;
   double b    = 0.;

   const double test = 
     quaternion.x()*quaternion.y()     + 
     quaternion.z()*quaternion.scalar();
   
   // singularity at north pole
   if (test > 0.499) 
     {
       head = 2.*atan2 (quaternion.x(), quaternion.scalar());
       att  = constant::pi_2;
       b    = 0.;
     }
   
   // singularity at south pole
   else if (test < -0.499) 
     {
       head = -2.*atan2(quaternion.x(), quaternion.scalar());
       att  = -constant::pi_2;
       b    = 0.;
     }
   
   // if there are no singularities....
   else
     {
       double sqx = quaternion.x()*quaternion.x();
       double sqy = quaternion.y()*quaternion.y();
       double sqz = quaternion.z()*quaternion.z();
       
       head = atan2 (2.*quaternion.y()*quaternion.scalar() -
		     2.*quaternion.x()*quaternion.z(), 
		     1. - 2.* (sqy - sqz));
       
       att  = asin (2.*test);
       
       b    = atan2 (2.*quaternion.x()*quaternion.scalar() -
		     2.*quaternion.y()*quaternion.z(), 
		     1. - 2.* (sqx - sqz));
     }

   return QVector3D(head, att, b);
}

QQuaternion MainViewer::quatfromEuler ()
{

  const double heading2  = 0.5 * constant::deg2rad * thetaRot;
  const double altitude2 = 0.5 * constant::deg2rad * psiRot;
  const double bank2     = 0.5 * constant::deg2rad * phiRot;
  const double c1        = cos (heading2);
  const double s1        = sin (heading2);
  const double c2        = cos (altitude2);
  const double s2        = sin (altitude2);
  const double c3        = cos (bank2);
  const double s3        = sin (bank2);
  const double c1c2      = c1*c2;
  const double s1s2      = s1*s2;

  return QQuaternion(c1c2*c3  - s1s2*s3, 
		     c1c2*s3  + s1s2*c3, 
		     s1*c2*c3 + c1*s2*s3, 
		     c1*s2*c3 - s1*c2*s3);
}

void MainViewer::get_axis_angle (const QQuaternion quaternion, 
				 QVector3D        &v, 
				 double           &angle)
{
   double tmp_angle = acos (quaternion.scalar());
   double scale     = sqrt (quaternion.x()*quaternion.x() + 
			    quaternion.y()*quaternion.y() + 
			    quaternion.z()*quaternion.z());
   
   // Don't do anything if scale is really small:
   if (scale < 1.e-12)  
     {
       angle = 0.;
       v     = QVector3D (0., 0., 1.);
     }
   
   // otherwise do something:
   else 
     {
       angle = tmp_angle + tmp_angle;

       v = QVector3D (quaternion.x()/scale, 
		      quaternion.y()/scale, 
		      quaternion.z()/scale);

       v = v.normalized ();
     }
}

void MainViewer::SL_rotate()
{
// qWarning("...Y ROTATION - %g", VS->y_rot);
//  double matr[16];
  for (int i=0; i<16; i++) arcb->rotM[i] = 0.0;
  for (int i=0; i<3; i++) {
     arcb->rotM[i] = ActualData->rot_tensor[3*i];
//  for (int i=4; i<7; i++)
     arcb->rotM[i+4] = ActualData->rot_tensor[3*i + 1];
//  for (int i=8; i<11; i++)
     arcb->rotM[i+8] = ActualData->rot_tensor[3*i + 2];
  }
  arcb->rotM[15] = 1.0;
//  glMultMatrixd(matr);
//  updateGL();
}
