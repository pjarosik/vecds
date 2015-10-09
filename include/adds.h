#ifndef ADDs_H
#define ADDS_H

#include "internal.h"
#include "miscfunc.h"

class Adds
{
	
 public:	
  /**
   * Constructor.
   */
  Adds();
  
  friend class MainWindow;
  friend class OsgScene;
  //friend class MiscFunc;
  //friend class Gener;
  //friend class Calc;
  
 private: 

  osg::ref_ptr<osg::Vec3Array> pos;
  osg::ref_ptr<osg::IntArray> nKind;

  std::vector<glm::dvec3> millerVs;
  std::vector<glm::dvec3> millerPs;
  std::vector<double> fracts;
  std::vector<glm::dmat3> rotTens;
  std::vector<QString> strName;
  std::vector<QString> strBV;
  std::vector<int> crCNum;
  int n_adds;
  int n_markers;
  //void initAdds();   
  osg::ref_ptr<osg::Geometry> markers[6];
};  
#endif //ADDS_H