#include "../include/picking.h"

#include  <iostream>

//extern osg::Vec4 colWhite;
extern Internal *INT;

bool PickHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
//    if ( (ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL)  && ea.getEventType()==osgGA::GUIEventAdapter::RELEASE ) {
    if ( ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN ) {
       if (ea.getKey()=='s') std::cout << " +++ === *** PRESSED ***   's'" << std::endl;
    }
    if ( (ea.getModKeyMask() & ( osgGA::GUIEventAdapter::MODKEY_CTRL | osgGA::GUIEventAdapter::MODKEY_SHIFT ) )  && ea.getEventType()==osgGA::GUIEventAdapter::RELEASE ) {
         left = ea.getButton() & osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON;
         right = ea.getButton() & osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON;
	 ctrl = ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL;
	 shift = ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_SHIFT;
         if ( !left && !right ) return false;
         osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
         if ( viewer ) {
            osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
            osgUtil::IntersectionVisitor iv( intersector.get() );
            viewer->getCamera()->accept( iv );
            if ( intersector->containsIntersections() ) {
                osgUtil::LineSegmentIntersector::Intersection result = *(intersector->getIntersections().begin());
                doUserOperations(result);//, left, right);
            }
         }
     }
     return false;
}

void PickHandler::doUserOperations(osgUtil::LineSegmentIntersector::Intersection& result)
{
   if ( result.nodePath.size()>0 )  {
		//osg::ref_ptr<osg::ShapeDrawable> cyl = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0., 0., 0.), 0.5, 1.0));
        //cyl->setColor(osg::Vec4(1., 1., 1., 1.));
         osg::Geode* geode = dynamic_cast<osg::Geode*>( result.nodePath.back() );
         if ( geode ) {//std::cout << "QQQQQQQQQQQQQ" << std::endl;//geode->removeDrawable( result.drawable.get() );
//			 osg::Vec3 r1 = result.getLocalIntersectPoint();
              //osgUtil::LineSegmentIntersector::Intersection::drawable
	      //osg::ref_ptr<osg::Drawable> drw = result.drawable();
	      /*
	      int ile = geode->getNumParents();
	      for (int i=0; i<ile; i++) {
	        std::string returnedString = geode->getParent(i)->getName();	      
	      //int returnedString;
	      //geode->getUserValue("id", returnedString);
	        std::cout << "string=" << returnedString << "   i=" << i << std::endl;
	      }
	      */
	      //osg::Vec3 r1 = result.getLocalIntersectPoint();
//	std::cout << " local= " << r1.x() << ",  " << r1.z() << ",  " << -r1.y() << std::endl;
	      osg::Vec3 r2 = result.getWorldIntersectPoint();
	      //std::cout << " local= " << r1.x() << ",  " << r1.y() << ",  " << r1.z() << std::endl;			 
	      //std::cout << " world= " << r2.x() << ",  " << r2.z() << ",  " << -r2.y() << std::endl;
	      INT->actPoint = glm::dvec3(r2.x(), r2.z(), -r2.y());
	      INT->pressed = true;
	      INT->right = right;
	      INT->left = left;
	      INT->shift = shift;
	      INT->ctrl = ctrl;
	      if ( right && ctrl ) {
                 std::cout << "*** PRESSED ***  RIGHT  CTRL  "  << r2.x() << ",  " << r2.z() << ",  " << -r2.y() << std::endl;
              }
              if ( right && shift ) {
                 //int nA = Calc::identAtom(glm::dvec3(r2.x(), r2.z(), -r2.y()));
		 std::cout << "*** PRESSED ***  RIGHT  SHIFT  "  << r2.x() << ",  " << r2.z() << ",  " << -r2.y() << std::endl; // "    nA=" << INT->nA <<
              }
	     /*
	      if ( left ) { //&& !ctrl && !shift) {
                 std::cout << "*** PRESSED ***  LEFT  "  << r2.x() << ",  " << r2.z() << ",  " << -r2.y() << std::endl;
              }
	      */
	      if ( left && ctrl ) {
                 std::cout << "*** PRESSED ***  LEFT  CTRL  "  << r2.x() << ",  " << r2.z() << ",  " << -r2.y() << std::endl;
              }
              if ( left && shift ) {
                 //int nA = Calc::identAtom(glm::dvec3(r2.x(), r2.z(), -r2.y()));
		 std::cout << "*** PRESSED ***  LEFT  SHIFT  "  << r2.x() << ",  " << r2.z() << ",  " << -r2.y() << std::endl; // "    nA=" << INT->nA <<
              }
         } // if ( geode )
   }
}
/*
		 //QString qstr("");
                //INT->calcLog.append(QString("aa ")).append(QString::number(r2.x())).append("   ").append(QString::number(r2.z())).append(QString("\n"));
		 //qstr.append(QString("bb ")).append(QString::number(r2.x())).append("   ").append(QString::number(r2.z())).append(":  nA= ").append(QString::number(INT->nA));//.append(QString("\n"));
		 //INT->calcLog << qstr;//QString("aa ") << QString::number(r2.x()) << "   " << QString::number(r2.z()) << QString("\n");


bool PickHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
    if ( ea.getEventType()!=osgGA::GUIEventAdapter::RELEASE ||
         ea.getButton()!=osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON ||
         !(ea.getModKeyMask()&osgGA::GUIEventAdapter::MODKEY_CTRL) )
                                    return false;
    osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
    if ( viewer ) {
         osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
                new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
         osgUtil::IntersectionVisitor iv( intersector.get() );
         viewer->getCamera()->accept( iv );
         if ( intersector->containsIntersections() ) {
             osgUtil::LineSegmentIntersector::Intersection result = *(intersector->getIntersections().begin());
             doUserOperations( result );
         }
     }
     return false;
}

void PickHandler::doUserOperations( osgUtil::LineSegmentIntersector::Intersection& result )
{
    if ( result.nodePath.size()>0 )  {
		//osg::ref_ptr<osg::ShapeDrawable> cyl = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0., 0., 0.), 0.5, 1.0));
        //cyl->setColor(osg::Vec4(1., 1., 1., 1.));
         osg::Geode* geode = dynamic_cast<osg::Geode*>( result.nodePath.back() );
         if ( geode ) {//std::cout << "QQQQQQQQQQQQQ" << std::endl;//geode->removeDrawable( result.drawable.get() );
//			 osg::Vec3 r1 = result.getLocalIntersectPoint();
              //osgUtil::LineSegmentIntersector::Intersection::drawable
	      //osg::ref_ptr<osg::Drawable> drw = result.drawable();
	      int ile = geode->getNumParents();
	      for (int i=0; i<ile; i++) {
	      std::string returnedString = geode->getParent(i)->getName();	      
	      //int returnedString;
	      //geode->getUserValue("id", returnedString);
	      std::cout << "string=" << returnedString << "   i=" << i << std::endl;
	      }
			 osg::Vec3 r2 = result.getWorldIntersectPoint();
//			 std::cout << "r1=" << r1.x() << ",  " << r1.y() << ",  " << r1.z() << std::endl;
			 std::cout << "r2=" << r2.x() << ",  " << r2.z() << ",  " << -r2.y() << std::endl;
			 //emit SIG_actPoint(glm::dvec3(r2.x(), r2.z(), -r2.y()));
			 INT->actPoint = glm::dvec3(r2.x(), r2.z(), -r2.y());
			 INT->pressed = true;
			 INT->points.push_back(glm::dvec3(r2.x(), r2.z(), -r2.y()));
			 //int ind = result.primitiveIndex;
			 //int nd = geode->getNumDrawables();
			 //std::cout << "    nd=" << nd << std::endl; //<< " ind=" << ind 
			 //geode->replaceDrawable(geode->getDrawable(0), cyl.get());
		 }	 
    }
}
*/
