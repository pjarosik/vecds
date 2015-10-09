#include "../include/calc.h"
#include <iostream>

extern Internal *INT;
extern Lattice *LATT;
extern Atoms *AT;
    
    
  glm::dmat3 Calc::rotation_tensor() 
  { // obliczanie tensora obrotu sztywnego na podstawie wskaźników Millera
    if ( INT->hklDefined )  {
       glm::dvec3 s = glm::normalize(INT->crC->c2o * INT->millerV); // std::cout << "s   " << s.x << "   " << s.y << "   " << s.z << std::endl;       
       glm::dvec3 m = glm::normalize(glm::transpose(INT->crC->o2c) * INT->millerP);//std::cout << "m   " << m.x << "   " << m.y << "   " << m.z << std::endl;              
       glm::dvec3 mxs = glm::normalize(glm::cross(m, s));//std::cout << mxs.x << "   " << mxs.y << "   " << mxs.z << std::endl;              
       glm::dvec3 mxsxm = glm::normalize(glm::cross(mxs, m));//std::cout << mxsxm.x << "   " << mxsxm.y << "   " << mxsxm.z << std::endl;              
       return glm::dmat3(mxsxm.x, mxs.x, m.x,
                         mxsxm.y, mxs.y, m.y,
                         mxsxm.z, mxs.z, m.z);
    } else {
       return glm::dmat3(1.0, 0.0, 0.0,
                          0.0, 1.0, 0.0,
                          0.0, 0.0, 1.0); //rotTens = MiscFunc::quat2rot((mview1->getQuat()*ZtoY).conj())
       std::cout << "Rotation not defined" << std::endl;
    }  
  }  
    
  bool Calc::internal_miller(std::string line2, int which, int *mill)
  { //spradzenie poprawności danych i zamiana układu cztero na trójwsk.
       int numbmill;
       std::vector<std::string> fields;
       numbmill = fields.size();
       if ( line2.find_first_of(" ,")!=std::string::npos ) {
          std::vector<std::string> fields = MiscFunc::tokenize(line2, " ,");
          numbmill = fields.size();
          if ( numbmill<3 || numbmill>4 ) {  std::cout << "ERROR - BAD NUMBER OF MILLER INDICES 1" << std::endl;  return false;  }
          for (int i=0; i<numbmill; i++) mill[i] = MiscFunc::toInt(fields.at(i));
       } else {
          bool minus = false;
          numbmill = 0;
          size_t i = 0;
          while ( i<line2.size() ) {
             char ch = line2.at(i++);
	     if ( ch=='-' ) {
                 minus = true;
                 continue;}
             if ( isdigit(ch) ) {
		 int n = int(ch)-int('0'); //  std::cout << "numbmill=" << numbmill << "   n=" << n << std::endl;	         
                 if ( minus ) {  n = -n;  minus = false;  }
                 mill[numbmill++] = n;
	     }
             if ( ch=='.' && numbmill==2 ) mill[numbmill++] = -mill[0]-mill[1];
          }  // while
          if ( numbmill<3 || numbmill>4 ) {  std::cout << "ERROR - BAD NUMBER OF MILLER INDICES 2" << std::endl;  return false;  }
       } //if - else
       if ( numbmill==4 ) {
          if ( (mill[0]+mill[1])!=-mill[2] ) {  std::cout << "ERROR BAD SUM:  " << mill[0] << " + " << mill[1] << " != " << -mill[2] << std::endl; return false;  }
          if ( which==1 ) {  mill[0] -= mill[2];  mill[1] -= mill[2];  }
          mill[2] = mill[3];
       }
       return true;
  }

  bool Calc::parse_miller(const std::string line) 
  {
       int mill[4];
       std::string line1 = MiscFunc::stripBlanks(line);//  cout << "line1=" <<  line1 << endl;
       size_t i_left1 = line1.find('[');
       size_t i_right1 = line1.find(']');
       size_t i_left2 = line1.find('(');
       size_t i_right2 = line1.find(')');
       INT->fract = (i_left1==0)? 1. : MiscFunc::readFraction(line1.substr(0, i_left1));
       if ( i_left1>=i_right1 || i_left2>=i_right2 ) {  std::cout << "ERROR PARENTHESES" << std::endl;  return false;  }
       std::string line2 = MiscFunc::stripBlanks(line1.substr(i_left1+1, i_right1-i_left1-1));//  cout << "line2 (1)= " <<  line2 << endl;
       if ( internal_miller(line2, 1, mill) )  INT->millerV = glm::dvec3(mill[0], mill[1], mill[2]);
       else { std::cout << "ERROR - BAD MILLER INDICES 1" << std::endl;  return false;  }//  cout << "miller 1 " << mill[0] << "  " << mill[1] << "  " << mill[2] << std::endl;      
       INT->millerV = glm::dvec3( double(mill[0]), double(mill[1]), double(mill[2]) );    
       line2 = MiscFunc::stripBlanks(line1.substr(i_left2+1, i_right2-i_left2-1));//  cout << "line2 (2)= " <<  line2 << endl;
       if ( internal_miller(line2, 2, mill) )  INT->millerP = glm::dvec3(mill[0], mill[1], mill[2]);
       else {   std::cout << "ERROR - BAD MILLER INDICES 2" << std::endl;  return false;  }//  cout << "miller 2 " << mill[0] << "  " << mill[1] << "  " << mill[2] << std::endl; 
       INT->millerP = glm::dvec3( double(mill[0]), double(mill[1]), double(mill[2]) ); 
       INT->hklDefined = true;   
       return true;
  }
  
  glm::dvec3 Calc::plane1(const std::string line)//, glm::dvec3 point)
  {
       int mill[4];
       std::string line1 = MiscFunc::stripBlanks(line);//  cout << "line1=" <<  line1 << endl;
       size_t i_left2 = line1.find('(');
       size_t i_right2 = line1.find(')');
       std::string line2 = MiscFunc::stripBlanks(line1.substr(i_left2+1, i_right2-i_left2-1));//  cout << "line2 (2)= " <<  line2 << endl;
       if ( internal_miller(line2, 1, mill) )  return glm::dvec3(mill[0], mill[1], mill[2]);
       else { std::cout << "ERROR Plane" << std::endl;   return glm::dvec3(0., 0., 0.); }
  }
    
  int Calc::identAtom(glm::dvec3 point)
  {
       double px = point.x;       
       double py = point.y;
       double pz = point.z;
       for (int i=0; i<LATT->n_atoms; i++) {
           int ak = LATT->nAt.get()->at(i);
           double r = INT->radFactor * AT->a_rad1[ak];
           osg::Vec3 p2 = LATT->coords.get()->at(i);
           if ( px>(p2.x() - r) && px<=(p2.x() + r) &&
                py>(p2.y() - r) && py<=(p2.y() + r) &&
                pz>(p2.z() - r) && pz<=(p2.z() + r) )   return i;
       }
       return -666;
  }

  glm::dvec3 Calc::mixed_u(glm::dvec3 rotdist, double be, double bz)
  {  
     double r2 = rotdist.x*rotdist.x + rotdist.y*rotdist.y;//if ( r2<1e-15 )
     double r = sqrt(r2);
     double x = rotdist.x/r;
     double y = rotdist.y/r;
     double r02 = INT->r0_fact*be*be; // radius of inmobile ring relative to which the atoms in the core move up
     double ux =  be /(2.*osg::PI)*(atan2(y,x)+x*y/(2.*(1.-INT->nu)));// - 0.5*be;
     double uy = -be /(8.*osg::PI*(1.-INT->nu)) * ((1.-2.*INT->nu)*log(r2/r02) + (x+y)*(x-y));
     return glm::dvec3(ux, uy, bz/(2.*osg::PI)*atan2(y, x));
  } // mixed_u

/*
bool rect_box(glm::dvec3 pos)
{
  pos -= center;
  return (pos.x>-0.5*size_x && pos.x<0.5*size_x &&
          pos.y>-0.5*size_y && pos.y<0.5*size_x &&
          pos.z>-0.5*size_z && pos.z<0.5*size_z);
}

bool hex_box(glm::dvec3 pos)
{
  double hr = hexRad*cos30;
  pos -= center;
  if ( pos.z>0.5*size_z || pos.z<-0.5*size_z ) return false;
  double rad = sqrt(pos.x*pos.x+pos.y*pos.y);
  if ( rad>hexRad ) return false;
  if ( rad<hr ) return true;
  if ( pos.x<=0.5*hexRad && pos.x>=-0.5*hexRad ) return ( pos.y<=hr && pos.y>=-hr );
  double ca = ((pos.y>=0. && pos.x>0.) || (pos.y<0. && pos.x<0.))?  -0.5 : 0.5;
  double  cx = pos.y*ca - pos.x*cos30;
  return ( cx<=hr && cx>=-hr );
}

bool romb_box(glm::dvec3 pos)
{
  pos -= center;
  if ( pos.z>0.5*size_z || pos.z<-0.5*size_z ) return false;
  if ( pos.y<0. || pos.y>cos30*size_x ) return false;
  double tg = -sqrt(3.);
  if ( pos.x<0. && pos.y<tg*pos.x ) return false;
  if ( pos.x>0.5*size_x && pos.y>tg*(pos.x-size_x) ) return false;
  return true;
}
*/
