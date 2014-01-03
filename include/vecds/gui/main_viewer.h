
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


#ifndef MAIN_VIEWER_H
#define MAIN_VIEWER_H

#include <QGLWidget>
#include <QColor>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>
#include <QVector>
               // vecds library includes
#include <vecds/gui/arc_ball.h>
                       
#include <vecds/numerics/integer_vector.h>
#include <vecds/numerics/algebra.h>
#include <vecds/numerics/mat9d.h>
#include <vecds/base/additional.h>
#include <vecds/base/constant.h>

                    /**
		     * This is the class <code>MainViewer</code>. Since
		     * we use QGL widgets here this must be a derived
		     * class.
		     */

class MainViewer : public QGLWidget
{
    Q_OBJECT

public:
                    /**
		     * Constructor
		     */

    MainViewer(QWidget *parent = 0);

                    /**
		     * Destructor
		     */
    ~MainViewer();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

//private slots:
public slots:
    void SL_rotate();
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
//    void SL_spectrograms();

signals:
    void SIG_phiRotationChanged(int angle);
    void SIG_thetaRotationChanged(int angle);
    void SIG_psiRotationChanged(int angle);
    void SIG_xMovementChanged(int range);
    void SIG_yMovementChanged(int range);
    void SIG_zMovementChanged(int range);
    void SIG_actPoint(QVector3D res);
    void SIG_actPosition(QVector3D res);

protected:

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void draw_atoms();
    void draw_bonds();
    void draw_axis();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void init_spheres(int numbOfSubdiv);
    void init_faces();
    void normalizeAngle(double *angle);
    void wheelEvent(QWheelEvent *event);
    void set_defaults();
    void calculate_size();
    void prepare_scene();
    void prepare_invbox(QVector3D, QVector3D);
    void prepare_axis();
    void arrow(QVector3D orig, QVector3D vect, double fact, double sm);
    void draw_quad(float size);
//    void draw_quad(double v[8][3], double n[8][3]);
//    void draw_nodes();
    void draw_nod_disp(double f);
    void draw_arrows(double f);
//    void draw_nodes();
    void drawBackground();
//    void colorQuad(QColor, QColor, QColor, QColor, QVector3D, QVector3D, QVector3D, QVector3D);
    void drawTexture(GLuint tex, QVector3D p1, QVector3D p2, QVector3D p3,
                                                         QVector3D p4);
    QVector3D getOGLPos(int, int);
    QVector3D getMousePos(int, int);
    void draw_quad (double v[8][3], double norm_v[8][3]);
    void drawInvisBox();


    void draw_faces1(double f);

    /**
     * No idea what this does...?
     */ 
    void draw_line (QVector<int> line, double f);

    void draw_numbers();
//    void draw_cube_fem();

    void doGLdisloc();

    /**
     * No idea what this does?
     */
    void draw(QPainter* painter, Int4 pq, int n_res,
	      double min_val, double step, int n_steps);
    void draw(QPainter* painter, QVector<int> pq, int n_res,
	      double min_val, double step, int n_steps);

    void paintEvent();

    /**
     * Get the angle of this axis.
     */
    void get_axis_angle (const QQuaternion q, QVector3D &v, double &ang);

    void euler2matr();
//    void quat2Matr9(QQuaternion q);
//    QVector2D normalizeMouse(QPoint qp);
//    void quat2matr(QQuaternion q);
    QVector3D quat2euler(QQuaternion q);
    QQuaternion quatfromEuler();

  /**
   * Number of bonds in this context.
   */ 
    unsigned int n_bonds;
  
  /**
   * Number of atoms in this context.
   */
    unsigned int n_atoms;

    QVector3D* coord;
    //    QVector3D* normals;
    int* a_kind;
    
    QVector3D min_, max_, a_min_, a_max_, e_min_, e_max_;
    
    GLuint textures[100];
    GLuint background;
    
 public:
    double screenW, screenH;
    QPoint lastPos;
    //    double m_x, m_y;
    
 private:
    GLdouble model_view[16];
    GLdouble projection[16];
    GLint viewport[4];
    ArcBall *arcb;
    QVector2D mousePt;
    QVector3D mousePos;
    //    Mat9d thisRot, lastRot;
    double* transformM;
    double phiRot, thetaRot, psiRot, d_x, d_y, d_0;
    //    QQuaternion cam_quat, mouse_quat, temp_quat;
    //    double change_x, change_y, change_z;
    double angle;
    QVector3D axis;
    double xl, xr, yd, yu;
    GLuint Pixmap2texture(QPixmap *pix);
    GLuint image2texture(QImage *bmp);
    
    GLuint sphereList[125], cubeList;
    
    //    QVector3D invbox[8];
    double distance, dist0, distance0;
    
    int mmx, mmy;
    QVector3D cent_;
    float fov;
    
    Nodes* nodes;
    Elements* elems;
    //    Face* faces;
    Face f0;
    
    /**
     * Number of elements in this mesh context.
     */
    unsigned int n_elements;
    
    /**
     * Number of faces in this element context.
     */
    unsigned int n_faces;
    
    /**
     * Number of nodes in this element context.
     */
    unsigned int n_nodes;
    
    //    int n_elements, numbFaces, n_nodes, numbRes;
    
    double bg_red;
    double bg_green;
    double bg_blue;
    GLfloat a_color[4];
    float VIEW_rad_fact;
    int VIEW_whichRadius;
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
    // QString string;
    double small;
    double smaller;
    
    /**
     * RGB spectrum container for the main viewer.
     */
    QColor *colour_spectrum;
    
    double rad_scene;
    // QVector3D cube_normals[12];
};
#endif

                 
