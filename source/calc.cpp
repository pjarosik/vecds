#include "calc.h"
#include <iostream>

extern Internal *INT;
extern Lattice *LATT;
extern Atoms *AT;
    
    
  glm::dmat3 Calc::rotation_tensor(glm::dvec3 mV, glm::dvec3 mP) 
  { // obliczanie tensora obrotu sztywnego na podstawie wskaźników Millera
//    if ( INT->hklDefined )  {
       glm::dvec3 s = glm::normalize(INT->crC->c2o * mV); // std::cout << "s   " << s.x << "   " << s.y << "   " << s.z << std::endl;       
       glm::dvec3 m = glm::normalize(glm::transpose(INT->crC->o2c) * mP);//std::cout << "m   " << m.x << "   " << m.y << "   " << m.z << std::endl;              
       glm::dvec3 mxs = glm::normalize(glm::cross(m, s));// std::cout << mxs.x << " -1-  " << mxs.y << "   " << mxs.z << std::endl;
       //mxs = glm::normalize(mxs); std::cout << mxs.x << " -2-  " << mxs.y << "   " << mxs.z << std::endl;                            
       glm::dvec3 mxsxm = glm::cross(mxs, m);  std::cout << mxsxm.x << " -3-  " << mxsxm.y << "   " << mxsxm.z << std::endl;              
       mxsxm = glm::normalize(mxsxm);  std::cout << mxsxm.x << " -4-  " << mxsxm.y << "   " << mxsxm.z << std::endl;              
       return glm::dmat3(mxsxm.x, mxs.x, m.x,
                         mxsxm.y, mxs.y, m.y,
                         mxsxm.z, mxs.z, m.z);
/*    } else {
       return glm::dmat3(1.0, 0.0, 0.0,
                          0.0, 1.0, 0.0,
                          0.0, 0.0, 1.0); //rotTens = MiscFunc::quat2rot((mview1->getQuat()*ZtoY).conj())
       std::cout << "Rotation not defined" << std::endl;
    }  
*/    
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
       INT->hklDefined = glm::length2(glm::cross(INT->millerP, INT->millerV))>0.1;   
       return true;
  }
  
glm::dvec3 Calc::plane1(const std::string line)//, glm::dvec3 point)
{
   int mill[4];
   std::string line1 = MiscFunc::stripBlanks(line);//  cout << "line1=" <<  line1 << endl;
   size_t left1 = line1.find('[');
   size_t right1 = line1.find(']');
   size_t left2 = line1.find('(');
   size_t right2 = line1.find(')');   
   int sw = (left1>line1.size())? 2 : 1;
   size_t left = (sw==1)? left2 : left1;
   size_t right = (sw==1)? right2 : right1;
   std::string line2 = MiscFunc::stripBlanks(line1.substr(left+1, right-left-1));//  cout << "line2 (2)= " <<  line2 << endl;
   if ( internal_miller(line2, sw, mill) )  return glm::dvec3(mill[0], mill[1], mill[2]);
   else { std::cout << "ERROR Calc::plane1" << std::endl;   return glm::dvec3(0., 0., 0.); }
}
    
int Calc::identAtom(glm::dvec3 point)
{
   double px = point.x;       
   double py = point.y;
   double pz = point.z;
   for (int i=0; i<LATT->n_atoms; i++) {
      int ak = LATT->nAt.get()->at(i);
      double r = INT->radFactor * AT->a_rad1[ak];
      glm::dvec3 p2 = LATT->coords[i];
      if ( px>(p2.x - r) && px<=(p2.x + r) &&
           py>(p2.y - r) && py<=(p2.y + r) &&
           pz>(p2.z - r) && pz<=(p2.z + r) )   return i;
   }
   return -666;
}

glm::dvec3 Calc::mixed_u(glm::dvec3 rotdist, double be, double bz)
{  
   if ( be==0. ) return glm::dvec3(0., 0., bz/(2.*osg::PI)*atan2(rotdist.y, rotdist.x));
   double r2 = rotdist.x*rotdist.x + rotdist.y*rotdist.y;//if ( r2<1e-15 )
   double r = sqrt(r2);
   double x = rotdist.x/r;
   double y = rotdist.y/r;
   double r02 = INT->r0_fact*be*be; // radius of inmobile ring relative to which the atoms in the core move up
   double ux =  be /(2.*osg::PI)*(atan2(y,x)+x*y/(2.*(1.-INT->nu)));// - 0.5*be;
   double uy = -be /(8.*osg::PI*(1.-INT->nu)) * ((1.-2.*INT->nu)*log(r2/r02) + (x+y)*(x-y));
   return glm::dvec3(ux, uy, bz/(2.*osg::PI)*atan2(y, x));
} // mixed_u
  
bool Calc::rect_box(glm::dvec3 pos, glm::dvec3 sizeB)
{
  pos -= INT->actPoint;
  return ( pos.x>-0.5*sizeB.x && pos.x<0.5*sizeB.x &&
           pos.y>-0.5*sizeB.y && pos.y<0.5*sizeB.y &&
           pos.z>-0.5*sizeB.z && pos.z<0.5*sizeB.z );
}

bool Calc::hex_box(glm::dvec3 pos, glm::dvec3 size)
{
  double cos30 = 0.5*sqrt(3.);
  double hr = size.x*cos30;
  pos -= INT->actPoint;
  if ( pos.z>0.5*size.z || pos.z<-0.5*size.z ) return false;
  double rad = sqrt(pos.x*pos.x+pos.y*pos.y);
  if ( rad>size.x ) return false;
  if ( rad<hr ) return true;
  if ( pos.x<=0.5*size.x && pos.x>=-0.5*size.x ) return ( pos.y<=hr && pos.y>=-hr );
  double ca = ((pos.y>=0. && pos.x>0.) || (pos.y<0. && pos.x<0.))?  -0.5 : 0.5;
  double  cx = pos.y*ca - pos.x*cos30;
  return ( cx<=hr && cx>=-hr );
}

bool Calc::romb_box(glm::dvec3 pos, glm::dvec3 size)
{
  double cos30 = 0.5*sqrt(3.);
  pos -= INT->actPoint;
  if ( pos.z>0.5*size.z || pos.z<-0.5*size.z ) return false;
  if ( pos.y<0. || pos.y>cos30*size.x ) return false;
  double tg = -sqrt(3.);
  if ( pos.x<0. && pos.y<tg*pos.x ) return false;
  if ( pos.x>0.5*size.x && pos.y>tg*(pos.x-size.x) ) return false;
  return true;
}

bool Calc::cylinder(glm::dvec3 pos, glm::dvec3 sizeB)
{
  pos -= INT->actPoint;
  if ( pos.z>0.5*sizeB.z || pos.z<-0.5*sizeB.z ) return false;
  return ( pos.x*pos.x + pos.y*pos.y<sizeB.x*sizeB.x );
}

int Calc::Love_function(const gsl_vector *x, void *par, gsl_vector *result_funct)
{
  double rad_fact = 1.0;
  double nu          = 0.35;
  
  double be = ((struct params *) par)->be;
  double bz = ((struct params *) par)->bz;
  double u0x = ((struct params *) par)->u0x;
  double u0y = ((struct params *) par)->u0y;
  double u0z = ((struct params *) par)->u0z;
  const double distx = gsl_vector_get(x, 0);
  const double disty = gsl_vector_get(x, 1);
//const double distz = gsl_vector_get(x, 2);
  double r2 = distx*distx+disty*disty;
  double r = sqrt(r2);
  double xx = distx/r;
  double yy = disty/r;
  double r02 = rad_fact*be*be; // radius of inmobile ring relative to which the atoms in the core move up
  const double ux =  u0x - be /(2.*osg::PI)*(atan2(yy,xx)+xx*yy/(2.*(1.-nu)));// - 0.5*be;
  const double uy = u0y + be /(8.*osg::PI*(1.-nu)) * ((1.-nu-nu)*log(r2/r02) + (xx+yy)*(xx-yy));
  const double uz = u0z - bz/(2.*osg::PI)*atan2(yy, xx);
  gsl_vector_set(result_funct, 0, ux);
  gsl_vector_set(result_funct, 1, uy);
  gsl_vector_set(result_funct, 2, uz);
  return GSL_SUCCESS;
}

int Calc::Beta_function(const gsl_vector *x, void *par, gsl_matrix *jac) {
 
  double nu          = 0.35;
  
  double be = ((struct params *) par)->be;
  double bz = ((struct params *) par)->bz;
  const double xx = gsl_vector_get(x, 0);
  const double yy = gsl_vector_get(x, 1);
//const double zz = gsl_vector_get(x, 2);
  const double y2 = yy*yy;
  const double x2 = xx*xx;
  const double r2 = x2+y2;
//  cout << "  beta " << xx << "      " << yy << endl;
  if ( r2<1.e-15 ) {
    cout << " Atom  in the center of dislocation core" << endl;
    gsl_matrix_set(jac, 0, 0, 1.);
    gsl_matrix_set(jac, 0, 1, 0.);
    gsl_matrix_set(jac, 0, 2, 0.);
    gsl_matrix_set(jac, 1, 0, 0.);
    gsl_matrix_set(jac, 1, 1, 1.);
    gsl_matrix_set(jac, 1, 2, 0.);
    gsl_matrix_set(jac, 2, 0, 0.);
    gsl_matrix_set(jac, 2, 1, 0.);
    gsl_matrix_set(jac, 2, 2, 1.);
  } else {
    const double a = be/(4. * osg::PI * (1.-nu) * r2*r2);                  // a = bx/(4. * osg::PI * (1.-n) * r2*r2)
    const double bxx = 1. + a * yy * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2); // u(4) = -a * y * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xx 
    const double byx = a * xx * ((1.-2.*nu)*x2 + (3.-2.*nu)*y2);      // u(5) = -a * x * ((1.-2.*n)*x*x + (3.-2.*n)*y*y) !yx
    const double bzx = bz/(2.*osg::PI) * yy/r2;                            // u(6) = -bz/(2.*osg::PI) * y/r2                       !zx
    const double bxy = -a * xx * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2);     // u(7) =  a * x * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xy
    const double byy = 1. - a * yy * ((1.+2.*nu)*x2 - (1.-2.*nu)*y2); // u(8) =  a * y * ((1.+2.*n)*x*x - (1.-2.*n)*y*y) !yy
    const double bzy = bz/(2.*osg::PI) * xx/r2;                            // u(9) = -bz/(2.*osg::PI) * x/r2                       !zy
    gsl_matrix_set(jac, 0, 0, bxx);
    gsl_matrix_set(jac, 0, 1, bxy);
    gsl_matrix_set(jac, 0, 2, 0.);
    gsl_matrix_set(jac, 1, 0, byx);
    gsl_matrix_set(jac, 1, 1, byy);
    gsl_matrix_set(jac, 1, 2, 0.);
    gsl_matrix_set(jac, 2, 0, bzx);
    gsl_matrix_set(jac, 2, 1, bzy);
    gsl_matrix_set(jac, 2, 2, 1.); }
  return GSL_SUCCESS;
}

int Calc::Love_fdf(const gsl_vector *x, void *par, gsl_vector *result_funct, gsl_matrix *jac) {
  Love_function(x, par, result_funct);
  Beta_function(x, par, jac);
  return GSL_SUCCESS;
}




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