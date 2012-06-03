
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010)
// Affiliation: Polish Academy of Sciences
//
// Copyright (C) 2010 The vecds authors
//
// This  program is  free  software: you  can  redistribute it  and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software  Foundation, either version 3 of the
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


#ifndef MAIN_VIEWER_H
#define MAIN_VIEWER_H


                                 // Qt includes
#include <QGLWidget>
#include <QColor>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>

                                 // vecds base includes
#include <vecds/base/integer.h>
#include <vecds/base/matrix.h>
#include <vecds/base/additional.h>
#include <vecds/base/constant.h>

#include <vecds/arc_ball.h>


namespace vecds
{

                                 /* This is the class
				    <code>MainViewer</code>. Since we
				    use QGL widgets here this must be
				    a derived class. */

  class MainViewer 
    : 
  public QGLWidget
  {
    Q_OBJECT
      
      public:
                                 /* constructor */

    MainViewer (QWidget *parent = 0);

                                 /* destructor */
    ~MainViewer();
  
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    
    public slots:
    
    void SL_dophiRotation();
    void SL_dothetaRotation();
    void SL_dopsiRotation();
    void SL_doXMovement();
    void SL_doYMovement();
    void SL_doZMovement();
    
    void SL_needDraw();
    void SL_repaint();
    void SL_keypress(int key);
    void SL_loadImage();
    
  signals:
    
    void SIG_phiRotationChanged(int);
    void SIG_thetaRotationChanged(int);
    void SIG_psiRotationChanged(int);
    void SIG_xMovementChanged(int);
    void SIG_yMovementChanged(int);
    void SIG_zMovementChanged(int);
/*
    void SIG_phiRotationChanged(int);
    void SIG_thetaRotationChanged(double);
    void SIG_psiRotationChanged(double);
    void SIG_xMovementChanged(double);
    void SIG_yMovementChanged(double);
    void SIG_zMovementChanged(double);
*/
    void SIG_actPoint(QVector3D res);
    void SIG_actPosition(QVector3D res);
    
  protected:
 
                                 /* These functions are reimplemented
				    from Qt's virtual functin
				    block. In general, they asre
				    called in this order: */
    void initializeGL ();
    void resizeGL (int width, int height);
    void paintGL ();

                                 /* These functions are reimplemented
				    from Qt's virtual function
				    library. They deal with mouse
				    clicks and other movements. */
    void mousePressEvent (QMouseEvent *event);
    void mouseMoveEvent (QMouseEvent *event);

                                 /* vecds functions. */
    void draw_atoms();
    void draw_bonds();
    void draw_axis();
 

    
  private:
    
                                 // TODO: document
    void set_defaults ();

                                 // TODO: document
    void prepare_scene ();

                                 // TODO: document
    void prepare_invbox (const glm::dvec3, 
			 const glm::dvec3);

                                 // TODO: document
    void prepare_axis ();
  
    void init_spheres(int numbOfSubdiv);
    void normalizeAngle(double *angle);
    void wheelEvent(QWheelEvent *event);
    

    
    void arrow(glm::dvec3 orig, glm::dvec3 vect, double fact, double sm);
    void drawBox0(float size);
    void draw_arrows(double f);
    void drawBackground();
    void drawTexture(GLuint tex, glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3, glm::dvec3 p4);
    
    QVector3D getOGLPos(int, int);
    QVector3D getMousePos(int, int);
    
    void drawBox(double v[8][3]/*, double norm_v[8][3]*/);
    void drawInvisBox();
    void set_v(double v[8][3], double norm_v[8][3], glm::dvec3 p1, glm::dvec3 p2);
    void draw_numbers();
    
    void doGLdisloc();
    void paintEvent();
    void getAxisAngle(const QQuaternion q, glm::dvec3 &v, double &ang);
    void euler2matr();
    
    QVector2D normalizeMouse(QPoint qp);
    void quat2matr(QQuaternion q);
    glm::dvec3 quat2euler(QQuaternion q);
    QQuaternion quatfromEuler();

                                 /* number of atoms in this
				    context. */
    unsigned int n_atoms;
    
                                 /* number of bonds in this
				    context. */ 
    unsigned int n_bonds;
  

    
    glm::dvec3* coord;
    int* a_kind;
    vecds::Int2* bonds;
    glm::dvec3 min_, max_, a_min_, a_max__;
    
    GLuint textures[100];
    GLuint background;
    
  public:
    
    double screenW, screenH;
    QPoint lastPos;
    
  private:
    
    GLdouble model_view[16];
    GLdouble projection[16];
    GLint viewport[4];
    
                                 // create an arc-ball for viewing
    vecds::ArcBall *arcball;

                                 /* rotation angles (rangle) related
				    to: banking */
    double rangle_phi;

                                 /* to: heading */
    double rangle_theta;

                                 /* to: altitude */
    double rangle_psi; 

    QVector2D mousePt;
    QVector3D mousePos;
    double* transformM;
    double d_x, d_y, d_0;

    glm::dvec3 axis;
    double xl, xr, yd, yu;
    GLuint Pixmap2texture(QPixmap *pix);
    GLuint image2texture(QImage *bmp);
    
    GLuint sphereList[125], cubeList;
    
    double distance, dist0, distance0;
    
    int mmx, mmy;
    glm::dvec3 cent_;
    float fov;
    
    
    double bg_red;
    double bg_green;
    double bg_blue;
    GLfloat a_color[4];
    float VIEW_rad_fact;
    int VIEW_whichRadius;
    
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
    double small;
    double smaller;
    double rad_scene;

  };                             // class MainViewer

}                                // namespace vecds
#endif

                 
