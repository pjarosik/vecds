#ifndef INTERNAL_H
#define INTERNAL_H

#include <vector>

#include <QVector>
#include <QStringList>
#include <QList>
#include <QtGui>
#include <QString>

#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/Matrixd>
#include <osg/Vec3d>
#include <osg/Math>
#include <osg/Vec4>

#include <osg/Geometry>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

//#include "../FEMApp/FEMAppLib/FEMAppInterface.h"

struct CrysCell
{
  QString crCName;
  double a, b, c, alpha, beta, gamma;
  glm::dmat3 c2o, o2c;
  QVector<glm::dvec3> cellCoord;
  QVector<int> cellAt;
  int numCellAt;
};

class Internal
{
	
 public:	
  /**
   * Constructor.
   */
  Internal();
  
  friend class MainWindow;
  friend class OsgScene;
  friend class MiscFunc;
  friend class OsgViewerQt;
  friend class QuestionForm2;
  //friend class Atoms;
  friend class Gener;
  friend class Calc;
  friend class SimplePad;
  friend class HelpBrowser;
  friend class PickHandler;
 //private:
/*
  osg::ref_ptr<osg::Camera> camera1;
  osg::ref_ptr<osg::MatrixTransform> m_worldAt;
  osg::ref_ptr<osg::MatrixTransform> m_image;
  osg::ref_ptr<osg::MatrixTransform> m_fem;
  osg::ref_ptr<osg::MatrixTransform> m_worldRes;
*/ 
  QStringList results;
 
  QString currDir;

  std::string image, atName, fem, compFem;
  std::string choosen_value;
  int whichFem;
    
  double xMin, xMax, yMin, yMax, zMin, zMax;
  double scDim;

  //QStringList calcLog;
  bool refrAtoms;
  bool refrAdds;
  bool addsDisplay;
  bool refrImage;
  bool refrFem;
  bool refrRes;
  bool newcalc;
  bool optim;
  //bool refresh;

  osg::ref_ptr<osg::MatrixTransform> m_worldAt;
  osg::ref_ptr<osg::MatrixTransform> m_worldAdds;
  osg::ref_ptr<osg::MatrixTransform> m_fem;
  osg::ref_ptr<osg::MatrixTransform> m_image;
  
  int mode;

  bool structureDefined;
  bool msgGraph;
  QString sett;
  //QString url;
  bool settDefined, crCDefined;

  CrysCell *crC;
  int crCNum;
  glm::dvec3 millerV, millerP;
  double fract;

  glm::dvec3 actPoint;
  glm::dmat3 rotTens;
  //int n_points;
  int nA;
  bool pressed;
  bool left, right, shift, ctrl;
//  bool planeDispl;
  bool hklDefined;
  bool atomDefined;
  bool coreDefined;
  
  QString points;
  double dist0;
  osg::Vec3d centr0;

  double nu;
  double r0_fact;
  //static const double deg2rad = 0.01745329251994329576;
  double cs4, cs5, cs6, sn6;
  int nr;
  QString name;
  QStringList words;
  
  osg::Vec4 colRed, colGreen, colBlue, colBlack, colWhite, colGray, colLGray, colDGray;

  //osg::ref_ptr<osg::MatrixTransform> m_worldReferenceFrame;
  osg::ref_ptr<osg::Vec4Array> mapColors;  
  
  //bool block;
  
  double fov;
  float radFactor;
  float alphaAt, alphaB, alphaIm;
  float alphaFemT, alphaFemL, alphaFemP;
  float axPr1, axPr2, axL, axRad, alphaAx;
  int axInd;
  osg::Vec4 bgColor;
  osg::Vec4 bondColor;
  float bondFactor;
  bool lightOn;
  bool materialOn;
  bool showNum;
  bool showAdds;
  float sizeTxt;
  osg::Vec4 matDiffuse, matSpecular, ls1Ambient, ls1Diffuse, ls1Specular, ls1Position, ls2Ambient, ls2Diffuse, ls2Specular, ls2Position;
  float matShiness;
  int sphSlices, sphStacks;
  float detRatio;
  
  QVector<CrysCell> structList;
  //QVector<int> structDesc;
  QStringList structNames;
    
  void initStructures();
  bool readDefaults(QString sett);
//  int nr;
//  QString name;
  osg::Vec4 ColorL, ColorP, ColorResL;
  float pointSize;
  int ndiv, ncol;
}; 
#endif //SETTINGS_H
/*
   lightSource->getLight()->setAmbient(osg::Vec4(0.4, 0.4, 0.4, 1.0));//(0.8, 0.8, 0.8, 1.0));
    lightSource->getLight()->setDiffuse(osg::Vec4(0.45, 0.45, 0.45, 1.0));//(0.6, 0.6, 0.6, 1.0));
    lightSource->getLight()->setSpecular(osg::Vec4(0.6, 0.6, 0.6, 1.0));//(1., 1., 1., 1.)); 
    lightSource->getLight()->setPosition(osg::Vec4(1., 0., 1., 0.));
    grp->addChild(lightSource); 
*/