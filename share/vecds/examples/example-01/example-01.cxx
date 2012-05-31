#include "example-01.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multiroots.h>

using namespace std;

static const double pi          = 3.14159265358979323846;
static const double inv_pi      = 0.31830988618379067154;
static const double deg2rad     = 0.01745329251994329576;
static const double rad2deg     = 57.29577951308232087721;
static const double nu          = 0.35;

Atoms* atoms;
CrysCell *crysCell;
string act_disl, current_dir;
int n_dislocations, n_addAtoms, n_Atoms;
miller mil;
glm::dmat3 rot_tensor, rot_inv, rev_tens, unitMatrix;
double rad_fact = 1.0;
double crit_stop = 1e-7;
size_t countN_R = 120;
glm::dvec3 *coord1;
string words[] = {"latt_const", "numb_addAtoms", "single_disl", "mult_disl", "end", "sum_u", "update"};
int n_errors = 0;
int n_Errors = 0;
int NumLine;
bool sw_iter;

struct params 
{
  double be;
  double bz;
  double u0x;
  double u0y;
  double u0z;
};

int main (int argc, char *argv[])
{ 
  current_dir = get_working_path();
  crysCell    = new CrysCell();
  atoms       = new Atoms();
  unitMatrix  = glm::dmat3(1., 0., 0.,
			   0., 1., 0.,
			   0., 0., 1.);
  rot_tensor = unitMatrix;
  rev_tens   = glm::dmat3(-1., 0., 0.,
			  0., 1., 0.,
			  0., 0.,-1.);
  sw_iter = true;
  n_dislocations = n_addAtoms = 0;

  string cd0 = current_dir;
  string cd1 = cd0.append("/input/atoms.xyz");

  if ( !read_xyz(cd1) ) 
    {
      cout << "Error - program can't find file " << cd1 << endl;
      return -10;
    }

  atoms->du     = new glm::dvec3[atoms->n_atoms];
  atoms->u      = new glm::dvec3[atoms->n_atoms];
  atoms->beta   = new glm::dmat3[atoms->n_atoms];

  for (int i=0; i<atoms->n_atoms; ++i) 
    atoms->u[i] = atoms->du[i] = glm::dvec3(0., 0., 0.);

  for (int i=0; i<atoms->n_atoms; ++i) 
    atoms->beta[i] = glm::dmat3(0.,0.,0., 0.,0.,0., 0.,0.,0.);

  coord1 = new glm::dvec3[atoms->n_atoms];

  if ( !readProgData("./input/new.data") ) 
    {
      cout << "Error - program can't read file  new.data" << endl;
      return -11;
    }

  cout << " $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $  $" << endl;
  cout << "Number of errors = " << n_errors << endl << "Number of Errors = " << n_Errors << endl;

  return 0;
}

bool read_xyz (string s0) // czyta zbiór początkowy xyz
{ 
  ifstream infile;
  infile.open(s0.c_str());
  if ( !infile ) 
    {
      cout << " !infile\n";
      return false;
    }

  int count = -1;
  string str1;
  istringstream instream;    // Declare an input string stream
  while ( !infile.eof() ) 
    {
      getline(infile, str1);
      ++count;
      instream.clear();       // Reset from possible previous errors.
      instream.str(str1);     // Use str1 as source of input.

      if ( count==0 ) 
	{
	  instream >> atoms->n_atoms;
	  atoms->a_name = new string[atoms->n_atoms];
	  atoms->coord  = new glm::dvec3[atoms->n_atoms];
	  continue;
	}

     if ( count==1 ) 
       continue;

     string a_name1;
     double xx, yy, zz;

     if (instream >> a_name1 >> xx >> yy >> zz) 
       {
	 instream >> ws;        // Skip white space, if any.
	 if (!instream.eof ())  
	   cout << "Warning. Too much items on the line nr " << count+1 << endl;
       } 
     else 
       {
	 if ( (count-2)>=atoms->n_atoms ) 
	   return true;

	 cout << "Error: Didn't find 4 items in line nr " << count+1 << endl;
	 return false;
       }
     atoms->a_name[count-2] = a_name1;
     atoms->coord[count-2] = glm::dvec3(xx, yy, zz);
    }

  return true;
}

int Love_function(const gsl_vector *x, void *par, gsl_vector *result_funct) 
{
  double be = ((struct params *) par)->be;
  double bz = ((struct params *) par)->bz;
  double u0x = ((struct params *) par)->u0x;
  double u0y = ((struct params *) par)->u0y;
  double u0z = ((struct params *) par)->u0z;
  const double distx = gsl_vector_get(x, 0);
  const double disty = gsl_vector_get(x, 1);
  const double distz = gsl_vector_get(x, 2);
  double r2 = distx*distx+disty*disty;
  double r = sqrt(r2);
  double xx = distx/r;
  double yy = disty/r;
  double r02 = rad_fact*be*be; // radius of inmobile ring relative to which the atoms in the core move up
  const double ux =  u0x - be /(2.*pi)*(atan2(yy,xx)+xx*yy/(2.*(1.-nu)));// - 0.5*be;
  const double uy = u0y + be /(8.*pi*(1.-nu)) * ((1.-nu-nu)*log(r2/r02) + (xx+yy)*(xx-yy));
  const double uz = u0z - bz/(2.*pi)*atan2(yy, xx);
  gsl_vector_set(result_funct, 0, ux);
  gsl_vector_set(result_funct, 1, uy);
  gsl_vector_set(result_funct, 2, uz);
  return GSL_SUCCESS;
}

int Beta_function(const gsl_vector *x, void *par, gsl_matrix *jac) 
{
  double be = ((struct params *) par)->be;
  double bz = ((struct params *) par)->bz;
  const double xx = gsl_vector_get(x, 0);
  const double yy = gsl_vector_get(x, 1);
  const double zz = gsl_vector_get(x, 2);
  const double y2 = yy*yy;
  const double x2 = xx*xx;
  const double r2 = x2+y2;

  if ( r2<1.e-15 ) 
    {
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
    } 
  else 
    {
      const double a = be/(4. * pi * (1.-nu) * r2*r2);                  // a = bx/(4. * pi * (1.-n) * r2*r2)
      const double bxx = 1. + a * yy * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2); // u(4) = -a * y * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xx 
      const double byx = a * xx * ((1.-2.*nu)*x2 + (3.-2.*nu)*y2);      // u(5) = -a * x * ((1.-2.*n)*x*x + (3.-2.*n)*y*y) !yx
      const double bzx = bz/(2.*pi) * yy/r2;                            // u(6) = -bz/(2.*pi) * y/r2                       !zx
      const double bxy = -a * xx * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2);     // u(7) =  a * x * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xy
      const double byy = 1. - a * yy * ((1.+2.*nu)*x2 - (1.-2.*nu)*y2); // u(8) =  a * y * ((1.+2.*n)*x*x - (1.-2.*n)*y*y) !yy
      const double bzy = bz/(2.*pi) * xx/r2;                            // u(9) = -bz/(2.*pi) * x/r2                       !zy
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

int Love_fdf(const gsl_vector *x, void *par, gsl_vector *result_funct, gsl_matrix *jac) 
{
  Love_function (x, par, result_funct);
  Beta_function (x, par, jac);

  return GSL_SUCCESS;
}

void singledisl(int n_a) 
{ // Liczy pole przemieszczeń od dyslokacji ze rdzeniem przechodzącym przez punk n_a  
  
  printMat("rotation tensor", rot_tensor);
  glm::dvec3 burgers_vector = rot_tensor * (mil.fraction * crysCell->c2o * glm::dvec3(mil.indices[0], mil.indices[1], mil.indices[2]));

  glm::dvec3 cd = rot_tensor * atoms->coord[n_a];
  double be=sqrt(burgers_vector.x*burgers_vector.x+burgers_vector.y*burgers_vector.y);
  double bz = burgers_vector.z;
  cout << " be=" << be << "     bz=" << bz << endl;

  for (int i=0; i<atoms->n_atoms; i++) 
    coord1[i] = rot_tensor * atoms->coord[i];
  
  if ( sw_iter ) 
    {
      
      const gsl_multiroot_fdfsolver_type *T;
      T = gsl_multiroot_fdfsolver_hybridsj;
      gsl_multiroot_fdfsolver *s; // = gsl_multiroot_fdfsolver_alloc(T, 3);
      gsl_vector *x; // = gsl_vector_alloc(3);
      size_t count;

      struct params p;
      size_t status;
      glm::dvec3 diff;
      double crit = 0.;
      
      p.be = be;
      p.bz = bz;
      
      for (int i=0; i<n_Atoms; i++) 
	{
	  //    if ( i>=atoms->n_atoms-n_addAtoms ) continue;
	  if ( i==n_a ) 
	    { 
	      cout<< "Error for n_a=" << n_a << "   i=" << i << endl;  continue; 
	    }
	  count = 0;
	  
	  do 
	    {
	      count++;
	      p.u0x = atoms->du[i].x;
	      p.u0y = atoms->du[i].y;
	      p.u0z = atoms->du[i].z;
	      glm::dvec3 temp = coord1[i]+atoms->du[i] - cd;

	      if ( (temp.x*temp.x+temp.y*temp.y)<1.e-10 ) 
		{
		  atoms->du[i] = glm::dvec3(0., 0., 0.);
		  cout << " Atom " << i << " in the center of dislocation core" << endl;
		  n_errors++;
		  goto _END;
		}

	      gsl_multiroot_function_fdf f = {&Love_function, &Beta_function, &Love_fdf, 3, &p};
	      x = gsl_vector_alloc(3);
	      gsl_vector_set(x, 0, temp.x);
	      gsl_vector_set(x, 1, temp.y);
	      gsl_vector_set(x, 2, temp.z);
	      
	      s = gsl_multiroot_fdfsolver_alloc(T, 3);
	      gsl_multiroot_fdfsolver_set(s, &f, x);
	      
	      status = gsl_multiroot_fdfsolver_iterate(s);
	      
	      if ( status ) 
		break;

	      diff.x = gsl_vector_get(s->f, 0);
	      diff.y = gsl_vector_get(s->f, 1);
	      diff.z = gsl_vector_get(s->f, 2);
	      atoms->du[i] -= diff;
	      status = gsl_multiroot_test_residual(s->f, crit_stop);

	    } while ( status== GSL_CONTINUE && count<countN_R );

	  crit += diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;

	  gsl_multiroot_fdfsolver_free(s);
	  gsl_vector_free(x);
	  continue;
	_END:
	  cout << "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << endl;
	  n_Errors++;
	}

      cout << " --- n_a = " << n_a << "     crit = " << crit << endl;
    } 
  else 
    {  // !sw_iter
      for (int i=0; i<n_Atoms; i++) 
	{
	  //    if ( i>=atoms->n_atoms-n_addAtoms ) continue;
	  if ( i==n_a ) 
	    { 
	      cout<< "Error for n_a=" << n_a << "   i=" << i << endl;  
	      continue; 
	    }
	  
	  glm::dvec3 dist1 = coord1[i] - cd;
	  atoms->du[i] = mixed_u(i, dist1, be, bz);
	}
    }
  
  for (int i=0; i<n_Atoms; i++) 
    atoms->du[i] = rot_inv*atoms->du[i];
  
}

void multdisl (int n_a, int num_rep, double distance) // Liczy pole przemieszczeń od dyslokacji ze rdzeniem przechodzącym przez punk n_a
{ 
  multdisl0(n_a, num_rep, distance);
}

void multdisl0 (int n_a, int num_rep, double distance) // Liczy pole przemieszczeń od dyslokacji ze rdzeniem przechodzącym przez punk n_a
{ 
  cout << "multdisl for atom nr. " << n_a << ", num_rep=" << num_rep << ", distance=" << distance << endl;
  for (int i=0; i<n_Atoms; i++) 
    atoms->du[i] = glm::dvec3(0., 0., 0.);

  glm::dvec3 burgers_vector = mil.fraction * crysCell->c2o * glm::dvec3(mil.indices[0], mil.indices[1], mil.indices[2]);
  glm::dvec3 cd = atoms->coord[n_a];
  printVec("cd", cd);
  burgers_vector = rot_tensor * burgers_vector;
  printVec("burgers 2", burgers_vector);
  double be=sqrt(burgers_vector.x*burgers_vector.x+burgers_vector.y*burgers_vector.y);
  double bz = burgers_vector.z;
  ostringstream nazwa;
  nazwa << "./output/Um-" << setw(2) << setfill('0') << n_dislocations+1;
  glm::dvec3 vector = rot_inv * glm::dvec3(distance, 0., 0.); // matvecmult(rot_inv, glm::dvec3(distance, 0., 0.));
  glm::dmat3 rt1 = rev_tens * rot_tensor;
  glm::dmat3 rt2 = glm::transpose(rt1);
  printMat("rt1", rt1);
  glm::dvec3 res1, res2;
  for (int count=0; count<num_rep; count++) 
    {
      glm::dvec3 cd1 = cd - double(count)*vector;
      glm::dvec3 cd2 = cd + double(count+1)*vector;
      for (int i=0; i<n_Atoms; i++) 
	{
	  if ( i==n_a ) continue;
	  glm::dvec3 dist1 = atoms->coord[i] - cd1;
	  glm::dvec3 rotdist = rot_tensor * dist1;
	  res1 = rot_inv*mixed_u(i, rotdist, be, bz);
	  glm::dvec3 dist2 = atoms->coord[i] - cd2;
	  rotdist = rt1 * dist2;
	  res2 = rt2*mixed_u(i, rotdist, be, bz);
	  atoms->du[i] += res1 + res2;}
    }

  return;
}

glm::dvec3 mixed_u (int i, glm::dvec3 rotdist, double be, double bz)
{ 
  double r2 = rotdist.x*rotdist.x + rotdist.y*rotdist.y;
  if ( r2<1e-15 ) 
    {
      cout << " Atom " << i << " in the center of dislocation core" << endl;
      n_errors++;
      
      return glm::dvec3(0., 0., 0.);} 
  else 
    {
      double r = sqrt(r2);
      double x = rotdist.x/r;
      double y = rotdist.y/r;
      double r02 = rad_fact*be*be; // radius of inmobile ring relative to which the atoms in the core move up
      double ux =  be /(2.*pi)*(atan2(y,x)+x*y/(2.*(1.-nu)));// - 0.5*be;
      double uy = -be /(8.*pi*(1.-nu)) * ((1.-nu-nu)*log(r2/r02) + (x+y)*(x-y));
      
      return glm::dvec3(ux, uy, bz/(2.*pi)*atan2(y, x));} 
} // mixed_u

glm::dmat3 mixed_beta (int i, glm::dvec3 rotdist, double be, double bz)
{   
  glm::dmat3 b(0.,0.,0.,  0.,0.,0.,  0.,0.,0.);
  double x=rotdist.x;
  double x2=x*x;
  double y = rotdist.y;
  double y2=y*y;
  double r2=x2+y2;
  
  if ( r2<1.e-15 ) 
    cout << " Atom " << i << " in the center of dislocation core" << endl;

  else 
    {
      double a = be/(4. * pi * (1.-nu) * r2*r2);             // a = bx/(4. * pi * (1.-n) * r2*r2)
      double bxx = -a * y * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2); // u(4) = -a * y * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xx 
      double byx = -a * x * ((1.-2.*nu)*x2 + (3.-2.*nu)*y2); // u(5) = -a * x * ((1.-2.*n)*x*x + (3.-2.*n)*y*y) !yx
      double bzx = -bz/(2.*pi) * y/r2;                       // u(6) = -bz/(2.*pi) * y/r2                       !zx
      double bxy =  a * x * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2); // u(7) =  a * x * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xy
      double byy =  a * y * ((1.+2.*nu)*x2 - (1.-2.*nu)*y2); // u(8) =  a * y * ((1.+2.*n)*x*x - (1.-2.*n)*y*y) !yy
      double bzy = -bz/(2.*pi) * x/r2;                       // u(9) = -bz/(2.*pi) * x/r2                       !zy

      b = glm::dmat3( bxx, bxy, 0.,   byx, byy, 0.,    bzx, bzy, 0. );
    }

  return b;
}

void processMiller (string result_text) // liczy tensory obrotu na podstawie wskaźników Millera
{ 
  act_disl = result_text;
  mil = parse_miller(result_text);
  if ( mil.fraction!=0. ) 
    {
      rot_tensor = rotation_tensor();         // nowy kolejny obrót
      rot_inv = glm::transpose(rot_tensor);   //  jego odwrotność
    }
}

glm::dmat3 rotation_tensor () // obliczanie tensora obrotu sztywnego na podstawie wskaźników Millera 
{ 
  glm::dvec3 s = crysCell->c2o * glm::dvec3(mil.indices[0], mil.indices[1], mil.indices[2]);
  glm::dvec3 m = glm::transpose(crysCell->o2c) * glm::dvec3(mil.indices[3], mil.indices[4], mil.indices[5]);
  s = glm::normalize(s);
  m = glm::normalize(m);
  glm::dvec3 mxs = glm::normalize(glm::cross(m, s));
  glm::dvec3 mxsxm = glm::normalize(glm::cross(mxs, m));
  return glm::dmat3(mxsxm.x, mxs.x, m.x,
                    mxsxm.y, mxs.y, m.y,
                    mxsxm.z, mxs.z, m.z);
}

void extPrint (string file_name, int nvec, glm::dvec3 *vec)
{ 
  ofstream fout(file_name.c_str());
  if ( fout.bad() ) 
    {
      cout << "Error opening file " << file_name << endl;
      return;
    }
  fout << nvec << endl << endl;
  for (int i=0; i<nvec; i++)
    fout << setw(5) << i+1 << setw(15) << vec[i].x << setw(15) << vec[i].y << setw(15) << vec[i].z << endl;
  fout.close();
}

void extPrintM (string file_name, int nvec, glm::dmat3 *mat)
{ 
  ofstream fout(file_name.c_str());
  if ( fout.bad() ) 
    {
      cout << "Error opening file " << file_name << endl;
      return;
    }
  fout << nvec << endl << endl;
  for (int i=0; i<nvec; i++)
    fout << setw(15) << mat[i][0][0] << setw(15) << mat[i][1][0] << setw(15) << mat[i][2][0]
	 << setw(15) << mat[i][0][1] << setw(15) << mat[i][1][1] << setw(15) << mat[i][2][1]
	 << setw(15) << mat[i][0][2] << setw(15) << mat[i][1][2] << setw(15) << mat[i][2][2] << endl;
  fout.close();
}

void atoms1Print(string file_name, glm::dvec3 *vec)
{ 
  ofstream fout(file_name.c_str());
  fout << n_Atoms << endl << endl;
  for (int i=0; i<n_Atoms; i++) 
    fout << setw(5) << atoms->a_name[i] << setw(15) << vec[i].x << setw(15) << vec[i].y << setw(15) << vec[i].z << endl;
  
  fout.close();
}


void atoms2Print(string file_name, glm::dvec3 *vec1, glm::dvec3 *vec2)
{ 
  ofstream fout(file_name.c_str());
  fout << n_Atoms << endl << endl;
  for (int i=0; i<n_Atoms; i++) 
    {
      glm::dvec3 vec = vec1[i]+vec2[i];
      fout << setw(5) << atoms->a_name[i] << setw(15) << vec.x << setw(15) << vec.y << setw(15) << vec.z << endl;
    }
  fout.close();
}

bool readProgData(string filename)
{ 
  int numbFields;
  ifstream infile;
  infile.open(filename.c_str());
  if ( !infile ) 
    {
      cout << "Error - program can't find file " << filename << endl;
      return false;
    }

  string str1;
  ostringstream nazwa;
  int numMark;
  int what = -1;
  int count = 0;
  glm::dvec3 sumU;
  while ( !infile.eof() ) 
    {
      getline(infile, str1);
      ++count;
      NumLine = count;

      if ( str1.empty() ) 
	continue;

      vector<string> fields = tokenize(str1, " ");
      numbFields = fields.size();
      cout << str1 << "   numFields=" << numbFields << endl;

    if ( fields[0].compare(string("//"))==0 ) 
      continue;

    if ( numbFields>1 && fields[0].compare(string("!!"))==0 ) 
      {
	what = identify(fields[1], 7, words);// cout << "what=" << what << endl; cout << fields[1] << endl;
	if ( what<0 ) 
	  {
	    cout << "Error - unrecognized line nr. " << count << endl;
	    return false;
	  }
	if ( what==4 ) 
	  {                      // end
	  cout << "End of data" << endl;
	  return true;
	  }
      if ( what==5 ) {                      // sum_u
        for (int i=0; i<n_Atoms; i++) {
          atoms->u[i] += atoms->du[i];
          atoms->du[i] = glm::dvec3(0., 0., 0.); }

        nazwa.str("");
        nazwa << "./output/coord-" << setw(4) << setfill('0') << count << ".xyz";
        atoms2Print(nazwa.str().c_str(), atoms->coord, atoms->u);
      }
      if ( what==6 ) {                      // update
        for (int i=0; i<n_Atoms; i++) atoms->coord[i] += atoms->u[i];
        nazwa.str("");
        nazwa << "./output/Coordinates-" << setw(3) << setfill('0') << count << ".xyz";
        atoms1Print(nazwa.str().c_str(), atoms->coord);
      }
      continue;}
    double cosa6, sina6, cosa5, cosa4;
    int num_rep;
    double distance;
    ostringstream nazwa;


    switch (what) 
      {
      case 0 : 
	if ( numbFields!=6 ) 
	  {       // latt_const
	    cout << "error 0" << endl;
	    return false;
	  }
	crysCell->a = toDouble(fields[0]);
	crysCell->b = toDouble(fields[1]);
	crysCell->c = toDouble(fields[2]);
	crysCell->alpha = toDouble(fields[3]);
	crysCell->beta = toDouble(fields[4]);
	crysCell->gamma = toDouble(fields[5]);
	cosa6 = cos(deg2rad*crysCell->gamma);
	sina6 = sin(deg2rad*crysCell->gamma);
	if ( crysCell->gamma==90. ) 
	  { 
	    cosa6 = 0.;  sina6 = 1.; 
	  }
	cosa5 = cos(deg2rad*crysCell->beta);
	cosa4 = cos(deg2rad*crysCell->alpha);
	if ( crysCell->alpha==90. ) 
	  cosa4 = 0.;
	if ( crysCell->beta==90. ) 
	  cosa5 = 0.;
	crysCell->c2o = glm::dmat3(crysCell->a, 0., 0.,
				   crysCell->b*cosa6, crysCell->b*sina6, 0.,
				   crysCell->c*cosa5, crysCell->c*(cosa4-cosa5*cosa6)/sina6, 
				   crysCell->c*sqrt(1.+2.*cosa4*cosa5*cosa6-cosa4*cosa4-cosa5*cosa5-cosa6*cosa6)/sina6);

	crysCell->o2c = glm::inverse(crysCell->c2o);
	
	what = -1;
	break;
	
      case 1 : 
	if ( numbFields!=1 ) 
	  {         // numb_addAtoms
	    cout << "error 1" << endl;
	    return false;
	  }
	n_addAtoms = toInt(fields[0]);
	n_Atoms = atoms->n_atoms - n_addAtoms;
	what = -1;
	break;
	
      case 2 : if ( numbFields!=2 ) 
	  {         // single_disl
	    cout << "error 2" << endl;
	    return false;
	  }
	numMark = toInt(fields[0]);
	if ( numMark<0 ) 
	  { 
	    numMark = -numMark; 
	    sw_iter = false; 
	  } 
	else 
	  sw_iter = true;
	act_disl = fields[1];
	
	processMiller(act_disl);
	for (int i=0; i<n_Atoms; i++) 
	  {
	    atoms->du[i] = glm::dvec3(0.,0.,0.); 
	    atoms->beta[i] = glm::dmat3(0.,0.,0., 0.,0.,0., 0.,0.,0.);
	  }
	singledisl(numMark);
	++n_dislocations;
	sumU = glm::dvec3(0.0, 0.0, 0.0);;
	
	for (int i=0; i<n_Atoms; i++) 
	  sumU += atoms->du[i];
	
	sumU /= double(n_Atoms);
	cout << "Dislocation nr. " << n_dislocations << "   sumU = ( " << sumU.x << ", " << sumU.y << ", " << sumU.z << " )" << endl;
	for (int i=0; i<n_Atoms; i++) 
	  atoms->du[i] -= sumU;
	
	nazwa.str("");
	nazwa << "./output/du-disl" << setw(4) << setfill('0') << n_dislocations;;
	extPrint(nazwa.str().c_str(), n_Atoms, atoms->du);
	what = -1;
	break;
	
      case 3 : 
	if ( numbFields!=4 ) 
	  {          // mult_disl
	    cout << "error 3" << endl;
	    return false; 
	  }
	numMark = toInt(fields[0]);
	act_disl = fields[1];
	num_rep = toInt(fields[2]);
	distance = toDouble(fields[3]);
	processMiller(act_disl);
	multdisl(numMark, num_rep, distance);
	
	for (int i=0; i<n_Atoms; i++)  
	  {
	    atoms->u[i]  += atoms->du[i];
	    atoms->du[i]  = glm::dvec3(0., 0., 0.);
	  }
	
	++n_dislocations;
	what = -1;
	break;
	
      default: 
	cout << "Warning - unrecognized line nr. " << count << endl;
	break; 
      }
    }
  cout << "End of data not found" << endl;
  return true;
}
