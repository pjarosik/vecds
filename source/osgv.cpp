#include "osgscene.h"
#include "osgv.h"
#include "mainwindow.h"

const unsigned int defaultRefreshPeriod = 50;//30;
const unsigned int idleRefreshPeriod = 150;
const double camFact = 0.15;
extern Internal *INT;
extern MainWindow mainwindow;

OsgViewerQt::OsgViewerQt(OsgScene *scene1, double fovy) : QGLWidget(), 
                                                m_refreshPeriod(defaultRefreshPeriod)
{
        setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
	setAttribute(Qt::WA_DeleteOnClose);
        //osgViewer::ViewerBase::CullDrawThreadPerContext
        //osgViewer::ViewerBase::SingleThreaded
        //osgViewer::ViewerBase::DrawThreadPerContext
        //osgViewer::ViewerBase::CullThreadPerCameraDrawThreadPerContext
        // disable the default setting of viewer.done() by pressing Escape.
        //setKeyEventSetsDone(0);
        m_gw = createGraphicsWindow(0, 0, 800, 600);
	QGLWidget::makeCurrent();
	unsigned int contextID = m_gw->getState()->getContextID();
	bool goodGraph = osg::isGLExtensionSupported(contextID, "GL_EXT_draw_instanced");
	//if ( goodGraph ) std::cout << "graph OK" << std::endl;
        //else {
	  if ( !goodGraph && !INT->msgGraph ) {
             QMessageBox::information(0, "For your information", "Because of missing OpenGL extensions 'GL_EXT_draw_instanced'\n"  "the program will work poorly on your system.");
	     INT->msgGraph = true;
	} //}     
	
        QWidget* widget1 = addViewWidget( scene1, fovy );
        addView( m_view );
        
        QGridLayout* grid = new QGridLayout;
        grid->addWidget( widget1, 0, 0 );
        setLayout( grid );

        m_zNear = 1.0;
        m_zFar = 10000.0;
        i_h = 0;
        i_e = 0;
        i_b = 0;
	i_dist = 0.;
	i_cx0 = 0.;
	i_cy0 = 0.;
	dist0 = m_trackballManipulator->getDistance();
	INT->dist0 = dist0;
	_centr0 = m_trackballManipulator->getCenter();
	INT->centr0 = _centr0;
	std::cout << "VIEWER    dist0=" << dist0 << std::endl;
	std::cout << "VIEWER    _centr0=" << _centr0.x() << ",  "  << _centr0.y() << ",  "  << _centr0.z() << std::endl;
	_timer.start( m_refreshPeriod );
        connect( &_timer, SIGNAL(timeout()), this, SLOT(my_update()) ); //SLOT(update()));
	//setFocus();
	//viewer.
	setKeyEventSetsDone(0);
	this->setStyleSheet("background-color: white");
        std::cout << "OsgViewerQt OK" << std::endl;
}

OsgViewerQt::~OsgViewerQt()
{
}

    QWidget* OsgViewerQt::addViewWidget(OsgScene* scene1, double fovy)
    {
        m_view = new osgViewer::View;
//        addView( m_view );
        m_camera = m_view->getCamera();
        m_camera->setGraphicsContext( m_gw );
        //m_camera->getOrCreateStateSet()->setGlobalDefaults(); // viewer.getCamera()->getOrCreateStateSet()->setGlobalDefaults()
        const osg::GraphicsContext::Traits* traits = m_gw->getTraits();

        m_camera->setClearColor(INT->bgColor); //( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
        m_camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
        m_width = static_cast<double>(traits->width);
        m_height = static_cast<double>(traits->height);
        m_ratio = m_width/m_height;
        if ( fovy==0.0 )   m_camera->setProjectionMatrixAsOrtho(-camFact*m_width, camFact*m_width, -camFact*m_height, camFact*m_height, 1.0, 1000000.0 ); //m_zNear, m_zFar );
        else               m_camera->setProjectionMatrixAsPerspective(fovy, m_ratio, 1.0, 10000000.0 ); // m_zNear, m_zFar );

        m_view->setSceneData( scene1->getScene() );
        m_view->addEventHandler( new osgViewer::StatsHandler );
        m_view->addEventHandler( new PickHandler );
	//m_view->setKeyEventSetsDone(0);
        m_trackballManipulator = new osgGA::TrackballManipulator;
	m_trackballManipulator->setAutoComputeHomePosition(true); // nowa wstawka * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * + + +
        m_view->setCameraManipulator( m_trackballManipulator );

        return m_gw->getGLWidget();
    }
    
    void OsgViewerQt::removeViewWidget()
    {
	removeView(m_view);
    }

    osg::ref_ptr<osgQt::GraphicsWindowQt> OsgViewerQt::createGraphicsWindow(int x, int y, int w, int h, const std::string& name, bool windowDecoration)
    {
        osg::ref_ptr<osg::DisplaySettings> ds = osg::DisplaySettings::instance().get();
        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
        traits->windowName = name;
        traits->windowDecoration = windowDecoration;
        traits->x = x;
        traits->y = y;
        traits->width = w;
        traits->height = h;
        traits->doubleBuffer = true;
        traits->alpha = ds->getMinimumNumAlphaBits();
//        traits->alpha =1;
        traits->stencil = ds->getMinimumNumStencilBits();
        traits->sampleBuffers = ds->getMultiSamples();
        traits->samples = ds->getNumMultiSamples();

        return new osgQt::GraphicsWindowQt(traits.get());
    }
    
    void OsgViewerQt::setRefreshPeriod(unsigned int period)
    {
        m_refreshPeriod = period;
    }
    
    void OsgViewerQt::setIdle(bool val)
    {
       if ( val )  _timer.start(idleRefreshPeriod);
       else        _timer.start(defaultRefreshPeriod);
    }

    void OsgViewerQt::my_update()
    {
      if ( !INT->atName.empty() || !INT->fem.empty() || !INT->compFem.empty() ) {
       //if ( !INT->block ) {
        _quat = m_trackballManipulator->getRotation();
	_centr = m_trackballManipulator->getCenter();
	_dist = m_trackballManipulator->getDistance();
        MiscFunc::getEulerFromQuat(_quat, _heading, _elevation, _bank);
        i_h1 = static_cast<int>(57.295779514 * _heading);// yaw
        i_e1 = static_cast<int>(57.295779514 * _elevation);// pitch
        i_b1 = static_cast<int>(57.295779514 * _bank);// roll
        if ( i_h1!=i_h ) {
		   i_h = i_h1;
		   QString str;
		   emit SIG_thetaRotationChanged(str.setNum(static_cast<double>(i_h), 'f', 3));   //std::cout << "+-+my_update-+  h0=" << i_h << std::endl;		
  //std::cout << "------------------------------" << std::endl << "SIG_thetaRotationChanged  ..... str=" << str.toStdString() << std::endl;		   
        }		
        if ( i_e1!=i_e ) {
		   i_e = i_e1;
		   QString str;
		   emit SIG_phiRotationChanged(str.setNum(static_cast<double>(i_e), 'f', 3));
  //std::cout << "------------------------------" << std::endl << "SIG_phiRotationChanged  ..... str=" << str.toStdString() << std::endl;		   		     
		   //emit SIG_phiRotationChanged(i_e);
        }		
        if ( i_b1!=i_b ) {
		   i_b = i_b1;
		   QString str;
		   emit SIG_psiRotationChanged(str.setNum(static_cast<double>(i_b - 90), 'f', 3));  
   //std::cout << "------------------------------" << std::endl << "SIG_psiRotationChanged  ..... str=" << str.toStdString() << std::endl;		   
		   //emit SIG_psiRotationChanged(i_b - 90);
        }	
	if ( i_dist!=_dist ) {
		   i_dist = _dist;
		   QString str;
		   emit SIG_distChanged(str.setNum(static_cast<double>(i_dist/dist0), 'f', 3));
        }		      
	if ( i_cx0!=_centr.x() ) {    
	       i_cx0 = _centr.x();
	       QString str;
	       double dd = (INT->centr0.x()-i_cx0)/INT->scDim;
	       emit SIG_cxChanged(str.setNum(dd, 'f', 3));
//   std::cout << "------------------------------" << std::endl << "X  ..SIG emited..... str=" << str.toStdString() << std::endl;
//   std::cout << "X  SIG... centr0=" << _centr.x() << ",  "  << _centr.y() << ",  "  << _centr0.z() << std::endl;		   
//   std::cout << "X  SIG... test  " << INT->centr0.x()+dd*INT->scDim << std::endl;
	}
	if ( i_cy0!=_centr.z() ) {    
	       i_cy0 = _centr.z();
		   QString str;
		   double dd = (INT->centr0.z()-i_cy0)/INT->scDim;
		   emit SIG_cyChanged(str.setNum(dd, 'f', 3));
//   std::cout << "------------------------------" << std::endl << "Y  ..SIG emited..... str=" << str.toStdString() << std::endl;
//   std::cout << "X  SIG... centr0=" << _centr.x() << ",  "  << _centr.y() << ",  "  << _centr0.z() << std::endl;		   
//   std::cout << "X  SIG... test  " << INT->centr0.z()+dd*INT->scDim << std::endl;
	}

        if ( INT->pressed ) {
            INT->pressed = false;
          std::cout << " ++==++==++   my_Update    X:" << INT->actPoint.x << ",   Y:" << INT->actPoint.y << ",   Z:" << INT->actPoint.z << std::endl;
            emit SIG_actPoint();//INT->actPoint);
        }
        update();
      } //    ---- first "if"
    }
    
    osg::Matrixd OsgViewerQt::getMatrix()
    {
        return m_trackballManipulator->getMatrix();
    }

    void OsgViewerQt::setMatrix(osg::Matrixd m)
    {
        m_trackballManipulator->setByMatrix(m);
	//m_trackballManipulator->setAutoComputeHomePosition(true);
        //my_update();
    }
    
    void OsgViewerQt::setQuat(osg::Quat q)
    {
        m_trackballManipulator->setRotation(q);
	m_trackballManipulator->setAutoComputeHomePosition(true);
        //my_update();
    }

    void OsgViewerQt::setCentr(osg::Vec3d v)
    {
        m_trackballManipulator->setCenter(v);
	//m_trackballManipulator->setAutoComputeHomePosition(true);
 //       my_update();
    }
    
    void OsgViewerQt::setDist(double d)
    {
        m_trackballManipulator->setDistance(d);
	//m_trackballManipulator->setAutoComputeHomePosition(true);
//        my_update();
    } 


    void OsgViewerQt::keyPressEvent( QKeyEvent *event )
    {
	if ( m_gw.valid() )  {
	  std::cout << "EVENT  " << event << "  text: " << event->text().toStdString() << std::endl;
		//int value = STATIC_KEY_MAP.remapKey(event);
		//window->getEventQueue()->keyPress( value );
	}
    }
  
       
