#include "miscfunc.h"
#include "atoms.h"
#include "lattice.h"

extern Atoms *AT;
extern Internal *INT;
extern Lattice *LATT;

void MiscFunc::getEulerFromQuat(const osg::Quat q, double& heading, double& elevation, double& bank)
{
//  std::cout << " ===== 0 =========" << std::endl;		
    double sqx = q.x()*q.x();
    double sqy = q.y()*q.y();
    double sqz = q.z()*q.z();
    double t = q.x()*q.y() + q.z()*q.w();

    if ( t>0.49999 )  { // singularity at north pole
        heading = 2. * atan2(q.x(), q.w());
        elevation = osg::PI_2;
        bank = 0.;
    } else if ( t<-0.49999) {  // singularity at south pole
        heading = -2. * atan2(q.x(), q.w());
        elevation = -osg::PI_2;
        bank = 0.;
    } else {
        heading = atan2(2.*(q.y()*q.w() - q.x()*q.z()), 1. - 2.*sqy - 2.*sqz);
        elevation = asin(2.*t);
        bank = atan2(2.*(q.x()*q.w() - q.y()*q.z()), 1. - 2.*sqx - 2.*sqz);
    } 
}
    
osg::Quat MiscFunc::quatFromEuler(const double h, const double e, const double b)
{
    const double h2  = 0.5 * h;
    const double e2 = 0.5 * e;
    const double b2 = 0.5 * b;
    const double c1 = cos(h2);
    const double s1 = sin(h2);
    const double c2 = cos(e2);
    const double s2 = sin(e2);
    const double c3 = cos(b2);
    const double s3 = sin(b2);
    const double c1c2 = c1*c2;
    const double s1s2 = s1*s2;
    return osg::Quat( c1c2*s3  + s1s2*c3, 
	               s1*c2*c3 + c1*s2*s3, 
		       c1*s2*c3 - s1*c2*s3,
		       c1c2*c3  - s1s2*s3 );
}    

osg::Vec3d MiscFunc::convert(const QVector3D &vecIn)
{
    osg::Vec3d vecOut( vecIn.x(), vecIn.y(), vecIn.z() );
    return vecOut;
}

osg::Vec3d MiscFunc::convert(const double &x, const double &y, const double &z)
{
    osg::Vec3d vecOut( x, y, z );
    return vecOut;
}

osg::Vec3d MiscFunc::convert(const glm::dvec3 &vecIn)
{
    return osg::Vec3d( vecIn.x, vecIn.y, vecIn.z );
}

glm::dvec3 MiscFunc::convert(const osg::Vec3d &vecIn)
{
  return glm::dvec3( vecIn.x(), vecIn.y(), vecIn.z() );
}
/*
osg::Quat MiscFunc::convert(const glm::dquat)
{

}
*/
int MiscFunc::whichAtom(const QString atom)
{ 
  QString res;
  res.clear();
  for (int i=0; i<atom.size(); i++) {
      QChar ch = atom.at(i);
      if ( ch.isLetter() ) {
          if ( i==0 )  ch.toUpper();
          else         ch.toLower();
          res.append(ch);
      }
  }
  return whichString(res, AT->namea);
}

int MiscFunc::whichString(const QString str, const QStringList lst)
{
  for (int i=0; i<lst.size(); i++) if ( str.compare(lst.at(i), Qt::CaseSensitive)==0 ) return i;
  return -666;
}  

double MiscFunc::readFraction(const QString line)
{
  int i_fr;
  if ( (i_fr=line.indexOf("/", 0))<0 ) return line.toDouble();
  else  return double(line.left(i_fr).toInt())/double(line.mid(i_fr+1).toInt());
}

double MiscFunc::readFraction(const std::string line) // czyta ułamek np. w zapisie wektora Burgersa, przelicza na liczbę dziesiętną
{
  size_t i_fr = line.find('/');
  std::istringstream ins;
  ins.clear();
  ins.str(line);
  double result, temp; char c;
  if ( i_fr==std::string::npos ) ins >> result;
  else                         { ins >> result >> c >> temp; result /= temp; }
  return result;
}

osg::Vec4 MiscFunc::osgColor(const QColor col)
{
  return osg::Vec4(col.redF(), col.greenF(), col.blueF(), col.alphaF());
}

QColor MiscFunc::qtColor(const osg::Vec4 col)
{
  return QColor(int(255*col.x()), int(255*col.y()), int(255*col.z()), int(255*col.w())); 
}

int MiscFunc::toInt(const std::string word)
{
  int i;
  std::istringstream ins;
  ins.clear();
  ins.str(word);
  ins >> i;
  return i;
}

double MiscFunc::toDouble(const std::string word)
{
  double x;
  std::istringstream ins;
  ins.clear();
  ins.str(word);
  ins >> x;
  return x;
}

QString MiscFunc::fileName(QString path)
{
  return path.section('/', -1);
}

std::vector<std::string> MiscFunc::tokenize(const std::string& str, std::string del)
{
  std::vector<std::string> tokens;
  std::string delimiters = del;
  size_t lastPos = str.find_first_not_of(delimiters, 0);            // Skip delimiters at beginning.
  size_t pos = str.find_first_of(delimiters, lastPos);              // Find first "non-delimiter".
  tokens.clear();
  while ( pos!=std::string::npos || lastPos!=std::string::npos ) {
     tokens.push_back(str.substr(lastPos, pos-lastPos));            // Found a token, add it to the vector.
     lastPos = str.find_first_not_of(delimiters, pos);              // Skip delimiters. Note the "not_of"
     pos = str.find_first_of(delimiters, lastPos);                  // Find next "non-delimiter"
  }
  return tokens;
} 

std::string MiscFunc::stripBlanks(const std::string StringToModify)
{
   if ( StringToModify.empty() ) return "";
   size_t startIndex = StringToModify.find_first_not_of(" ");
   size_t endIndex = StringToModify.find_last_not_of(" ");
   std::string tempString = StringToModify;
   tempString = tempString.substr(startIndex, endIndex-startIndex+1);
   return tempString;
} 

void MiscFunc::printVec(const std::string title, const glm::dvec3 v)
{
   std::cout << title << "= ( " << v.x << ", " << v.y << ", " << v.z << " )" << std::endl;   
}

void MiscFunc::printVec(const std::string title, const osg::Vec3d v)
{
   std::cout << title << "= ( " << v.x() << ", " << v.y() << ", " << v.z() << " )" << std::endl;   
}

void MiscFunc::printVecPrec(const std::string str, const osg::Vec3d vec)
{
  std::cout << str << " =  ( " << std::setw(15) << vec.x() << std::setw(15) << vec.y() << std::setw(15) << vec.z() << " )" << std::endl;
}

void MiscFunc::printMat(const std::string str, const glm::dmat3 m)
{
  std::cout << str << "    det = " << glm::determinant(m) << std::endl;
  std::cout << " row 0 = " << std::setw(15) << m[0][0] << std::setw(15) << m[1][0] << std::setw(15) << m[2][0] << std::endl;
  std::cout << " row 1 = " << std::setw(15) << m[0][1] << std::setw(15) << m[1][1] << std::setw(15) << m[2][1] << std::endl;
  std::cout << " row 2 = " << std::setw(15) << m[0][2] << std::setw(15) << m[1][2] << std::setw(15) << m[2][2] << std::endl;
}   

//void MiscFunc::ComputeSizes(const osg::ref_ptr<osg::Vec3Array>nodes, double& minX, double& maxX, double& minY, double& maxY, double& minZ, double& maxZ)
void MiscFunc::ComputeSizes(const std::vector <glm::dvec3> nodes, double& minX, double& maxX, double& minY, double& maxY, double& minZ, double& maxZ)
{
    minX = 1e300; maxX = -1e300;
    minY = 1e300; maxY = -1e300;
    minZ = 1e300; maxZ = -1e300;
    for (unsigned k=0; k<nodes.size(); k++ ) {
        double valx = double(nodes[k].x); double valy = double(nodes[k].y); double valz = double(nodes[k].z);
        if ( valx < minX ) minX = valx;  if ( valx > maxX ) maxX = valx;
        if ( valy < minY ) minY = valy;  if ( valy > maxY ) maxY = valy;
        if ( valz < minZ ) minZ = valz;  if ( valz > maxZ ) maxZ = valz;
    }
}  

//void MiscFunc::ComputePlane(const osg::ref_ptr<osg::Vec3Array>nodes, const glm::dmat3 rotTens, double& minX, double& maxX, double& minY, double& maxY, double& minZ, int nA)
//void MiscFunc::ComputeBottom(const osg::ref_ptr<osg::Vec3Array>nodes, const glm::dmat3 rotTens, int& imnx, int& imxx, int& imny, int& imxy, int& iz)
void MiscFunc::ComputePlane(const std::vector <glm::dvec3> nodes, const glm::dmat3 rotTens, double& minX, double& maxX, double& minY, double& maxY, double& minZ, int nA)
{
/*
    double minX = 1e300; double maxX = -1e300;
    double minY = 1e300; double maxY = -1e300;
    double minZ = 1e300;
*/  
    minX = 1e300; maxX = -1e300;
    minY = 1e300; maxY = -1e300;
    //minZ = 1e300;
    int imnx, imxx, imny, imxy;// iz,
    imnx = imxx = imny = imxy = -666;// iz = 
    for (unsigned k=0; k<nodes.size(); k++ ) {
        glm::dvec3 pos = LATT->coords[k];//MiscFunc::convert(LATT->coords.get()->at(k));
        osg::Vec3d v = MiscFunc::convert(rotTens * pos);
        double valx = v.x(); double valy = v.y();// double valz = v.z();
        if ( valx < minX ) { minX = valx;   imnx = int(k); } 
	if ( valx > maxX ) { maxX = valx;   imxx = int(k); }
        if ( valy < minY ) { minY = valy;   imny = int(k); }
	if ( valy > maxY ) { maxY = valy;   imxy = int(k); }
	if ( int(k)==nA ) minZ = v.z();
        //if ( valz < minZ ) { minZ = valz;   iz = int(k); }
    }
    std::cout << "   imnx=" << imnx << "   imxx=" << imxx <<  "   imny=" << imny <<  "   imxy=" << imxy << std::endl; // "   iz=" << iz << 
    //minZ = (LATT->coords.get()->at(nA)).z();
}

//void MiscFunc::ComputeRect(const osg::ref_ptr<osg::Vec3Array>nodes, const glm::dmat3 rotTens, const int nA,  glm::dvec3& p1, glm::dvec3& p2, glm::dvec3& p3, glm::dvec3& p4)
void MiscFunc::ComputeRect(const std::vector <glm::dvec3> nodes, const glm::dmat3 rotTens, const int nA,  glm::dvec3& p1, glm::dvec3& p2, glm::dvec3& p3, glm::dvec3& p4)
{
    double minX = 1e300; double maxX = -1e300;
    double minY = 1e300; double maxY = -1e300;
    double minZ = 1e300;
    for (unsigned k=0; k<nodes.size(); k++ ) {
        glm::dvec3 pos = LATT->coords[k];//MiscFunc::convert(LATT->coords.get()->at(k));
        osg::Vec3d v = MiscFunc::convert(rotTens * pos);
        double valx = v.x(); double valy = v.y();// double valz = v.z();
        if ( valx < minX )  minX = valx;
	if ( valx > maxX )  maxX = valx;
        if ( valy < minY )  minY = valy;
	if ( valy > maxY )  maxY = valy;
	if ( int(k)==nA ) minZ = v.z();
    }
    p1 = glm::dvec3(minX, minY, minZ);
    p2 = glm::dvec3(maxX, minY, minZ);
    p3 = glm::dvec3(maxX, maxY, minZ);
    p4 = glm::dvec3(minX, maxY, minZ);
}

glm::dmat3 MiscFunc::quat2rot(const osg::Quat qq)
{
   double a = qq.w();
   double b = qq.x();
   double c = qq.y();
   double d = qq.z();
   double a2 = a*a;
   double b2 = b*b;
   double c2 = c*c;
   double d2 = d*d;
   return glm::dmat3( a2+b2-c2-d2,  2.*(b*c+a*d),  2.*(b*d-a*c),
	       	     2.*(b*c-a*d), a2-b2+c2-d2,   2.*(c*d+a*b),
		     2.*(b*d+a*c), 2.*(c*d-a*b),  a2-b2-c2+d2 );
}

osg::ref_ptr<osg::Geometry> MiscFunc::makeSphere(double radius, int slices, int stacks, osg::Vec4 col)
{
    float rho, drho, dtheta;
    float nsign = 1.0;

    drho = osg::PI / (float) stacks;
    dtheta = 2.*osg::PI / (float) slices;
    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
    osg::Vec3Array* vl = new osg::Vec3Array;
    osg::Vec3Array* nl = new osg::Vec3Array;
    osg::Vec4Array* cl = new osg::Vec4Array;
    cl->push_back(col);
	
    /* build slices as quad strips */
    for (int i = 0; i < stacks; i++ ) {
      	rho = i * drho;
	  for (int j = 0; j <= slices; j++ ) {
  	     double theta = (j == slices) ? 0.0 : j * dtheta;
	     double x = -sin(theta) * sin(rho);
	     double y = cos(theta) * sin(rho);
	     double z = nsign * cos(rho);
             osg::Vec3 normal(x*nsign, y*nsign, z*nsign);
	     normal.normalize();
	     nl->push_back(normal);
	     vl->push_back(osg::Vec3(x*radius, y*radius, z*radius));
	     x = -sin(theta) * sin(rho+drho);
	     y = cos(theta) * sin(rho+drho);
	     z = nsign * cos(rho+drho);
             normal = osg::Vec3(x*nsign, y*nsign, z*nsign);
	     normal.normalize();
	     nl->push_back(normal);
	     vl->push_back(osg::Vec3(x*radius, y*radius, z*radius));
	  }
    }
    //geometry->setUseDisplayList(false);
    //geometry->setUseVertexBufferObjects(true);
    geometry->setVertexArray(vl);
    geometry->setNormalArray(nl);
    geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
        //geometry->setTexCoordArray(0, tl);
    geometry->setColorArray(cl);
    geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
    geometry->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_STRIP, 0, vl->size()));
    return geometry.get();
}
