#ifndef OSGVIEWERQT_H
#define OSGVIEWERQT_H

#include <QGLWidget>
#include <QtCore/QTimer>
#include <QGridLayout>

#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgQt/GraphicsWindowQt>
#include <osgDB/WriteFile>
#include <osg/GLExtensions>
#include <osg/GL2Extensions>

#include "osgscene.h"
#include "miscfunc.h"
#include "picking.h"
#include "internal.h"

class OsgViewerQt : public QGLWidget, public osgViewer::CompositeViewer
{
    Q_OBJECT

public:
    OsgViewerQt(OsgScene *scene1, double fovy); //(OsgScene *scene);
    ~OsgViewerQt();
    
    virtual void paintEvent(QPaintEvent* event)  { frame(); }

    QWidget *addViewWidget( OsgScene *scene1, double fovy );
    void removeViewWidget();
    osg::ref_ptr<osgQt::GraphicsWindowQt> createGraphicsWindow( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false );
    void setRefreshPeriod(unsigned int period);
    void setIdle(bool val);
    osg::Quat getQuat() { return _quat; }
    const osg::Quat getQuat() const { return _quat; }
    osg::Matrixd getMatrix();
    void setMatrix(osg::Matrixd m);
    void setQuat(osg::Quat q);
    osg::Vec3d getCentr() { return _centr; }
    const osg::Vec3d getCentr() const { return _centr; }
    void setCentr(osg::Vec3d centr);
    double getDist() { return _dist; }
    const double getDist() const { return _dist; }
    void setDist(double dist);    
    
    //void saveScreen(QString name);
    void keyPressEvent( QKeyEvent *event );
    
private:
    
    osg::ref_ptr<osgViewer::View> m_view;
    osg::ref_ptr<osgQt::GraphicsWindowQt> m_gw;
    osg::ref_ptr<osg::Camera> m_camera;
    double m_width;
    double m_height;
    double m_ratio;
    double m_zNear;
    double m_zFar;
    unsigned int m_refreshPeriod;
    QTimer _timer;
    osg::ref_ptr<osgGA::TrackballManipulator> m_trackballManipulator;
    osg::Quat _quat;
    osg::Vec3d _centr, _centr0;
    double _dist, i_dist, dist0, i_cx0, i_cy0;
    double _heading, _elevation, _bank;// yaw, pitch, roll
    int i_h, i_e, i_b, i_h1, i_e1, i_b1;
     
signals:

    void SIG_phiRotationChanged(QString);
    void SIG_thetaRotationChanged(QString);//(int angle);
    void SIG_psiRotationChanged(QString); 
    void SIG_distChanged(QString);//(int);
    void SIG_cxChanged(QString);
    void SIG_cyChanged(QString);
    void SIG_actPoint(glm::dvec3);

    
public slots:

    void my_update(); 

};

#endif // OSGVIEWERQT_H
