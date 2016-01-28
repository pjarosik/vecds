#ifndef LATTICE_H
#define LATTICE_H

#include "internal.h"

class Lattice
{
	
 public:	
  /**
   * Constructor.
   */
  Lattice();
  ~Lattice();
  
  friend class MainWindow;
  friend class OsgScene;
  friend class MiscFunc;
  friend class Gener;
  friend class Calc;
  
 private: 
  
  void init(QString atname, int numbAt);
  void clearL();
  //osg::ref_ptr<osg::Vec3Array> coords;
  std::vector<glm::dvec3> coords;
  //osg::ref_ptr<osg::Vec3Array> u;
  //osg::ref_ptr<osg::Vec3Array> du;
  std::vector<glm::dvec3> u;
  std::vector<glm::dvec3> du;
  std::vector<int> marked;
  osg::ref_ptr<osg::IntArray> nAt;
  osg::ref_ptr<osg::IntArray> bond1;
  osg::ref_ptr<osg::IntArray> bond2;
  osg::ref_ptr<osg::IntArray> nK;
  QStringList kName;
  //QList<int> marked;
  CrysCell *crC;
  double xMin, xMax, yMin, yMax, zMin, zMax;
  double scDim;
  int n_atoms;
  int n_bonds;
  int n_k;
  QString name;
  
};  
#endif //LATTICE_H
