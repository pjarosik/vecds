#ifndef MISCFUNC_H
#define MISCFUNC_H

#include <QColor>
/*
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multiroots.h>
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

#include "internal.h"

class MiscFunc
{ 
 public:
  static constexpr double deg2rad = 0.01745329251994329576;
  static QString dateTime();
  static void getEulerFromQuat(const osg::Quat q, double& heading, double& elevation, double& bank);
  static osg::Quat quatFromEuler(const double h, const double e, const double b);
  static osg::Vec3d convert(const QVector3D &);
  static osg::Vec3d convert(const double &x, const double &y, const double &z);
  static osg::Vec3d convert(const glm::dvec3 &);
  static glm::dvec3 convert(const osg::Vec3d &);
  static void printVec(const std::string title, const glm::dvec3 v);
  static void printVec(const std::string title, const QVector3D v);
  static void printVec(const std::string title, const osg::Vec3d v);
  static int whichAtom(const QString atom);
  static int whichString(const QString str, const QStringList lst);
  static double readFraction(const QString line);
  static double readFraction(std::string line);
  static osg::Vec4 osgColor(const QColor col);
  static QColor qtColor(const osg::Vec4 col);
  static int toInt(const std::string);
  static double toDouble(const std::string);
  static std::vector<std::string> tokenize(const std::string& str, std::string del);
  static std::string stripBlanks(const std::string StringToModify);
  static QString fileName(QString path);
  static void ComputeSizes(const std::vector <glm::dvec3> nodes, double& minX, double& maxX, double& minY, double& maxY, double& minZ, double& maxZ);
  static void ComputePlane(const std::vector <glm::dvec3> nodes, const glm::dmat3 rotTens, double& minX, double& maxX, double& minY, double& maxY, double& minZ, int nA);
  static void ComputeRect(const std::vector <glm::dvec3> nodes, const glm::dmat3 rotTens, const int nA, glm::dvec3& p1, glm::dvec3& p2, glm::dvec3& p3, glm::dvec3& p4);  
  static void printVecPrec(const std::string str, const osg::Vec3d vec);
  static void printMat(const std::string str, const glm::dmat3 m);
  static glm::dmat3 quat2rot(const osg::Quat qq);  
  static osg::ref_ptr<osg::Geometry> makeSphere(double radius, int slices, int stacks, osg::Vec4 col); 
};
#endif // MISCFUNC_H
