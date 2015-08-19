#include "adds.h"
#include "atoms.h"
#include "internal.h"

extern Atoms *AT;
extern Internal *INT;


 Adds::Adds()
 {
    n_adds = 0;
    n_markers = 6;
    pos = new osg::Vec3Array;	
    nMark = new osg::IntArray;    
    for (int i=0; i<n_markers; i++) {
       osg::Vec4 color = AT->a_colors[i+117];
       float r = INT->radFactor * AT->a_rad1[i+117];
       markers[i] = MiscFunc::makeSphere(r, INT->sphSlices, INT->sphStacks, color);
    } 
  std::cout << "ADDS CREATED" << std::endl;   
 }