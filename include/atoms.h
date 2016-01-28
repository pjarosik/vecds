#ifndef ATOMS_H
#define ATOMS_H

#include "internal.h"

class Atoms
{

 friend class Internal;
	
 public:	
  /**
   * Constructor.
   */
Atoms();
  
  friend class MainWindow;
  friend class Points;
  friend class OsgScene;
  friend class MiscFunc;
  friend class Gener;
  friend class Calc;
  
 private: 
  
  int numbOfAtoms;
  float a_rad1[125]; 
  //float a_rad2[125];
  osg::Vec4 a_colors[125];
  QStringList namea;

};
#endif //ATOMS_H
