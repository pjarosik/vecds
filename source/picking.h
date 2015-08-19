#ifndef PICKING_H
#define PICKING_H

#include <osgViewer/View>
#include <osg/Camera>
#include <osgGA/GUIEventHandler>
#include <osgUtil/LineSegmentIntersector>
#include <osg/ShapeDrawable>

#include "internal.h"
#include "calc.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>  
  
class PickHandler : public osgGA::GUIEventHandler
{
//   friend class MainWindow;

 public:

   virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
   virtual void doUserOperations( osgUtil::LineSegmentIntersector::Intersection& result );// = 0;

 private:
   glm::dvec3 pos;
   bool pressed;
   bool left;
   bool right;
   bool shift;
   bool ctrl;

};
#endif // PICKING_H    
