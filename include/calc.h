#ifndef CALC_H
#define CALC_H

#include "internal.h"
#include "miscfunc.h"
#include "atoms.h"
#include "lattice.h"
#include <osg/Export>

#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multiroots.h>

class Calc
{ 
 public:
//  static const double deg2rad = 0.01745329251994329576;
  static glm::dmat3 rotation_tensor();
  static bool parse_miller(const std::string line);
  static glm::dvec3 plane1(const std::string line);
  static bool internal_miller(std::string line2, int which, int *mill);//spradzenie poprawności danych i zamiana układu cztero na trójwsk.
  static int identAtom(glm::dvec3 point);
  static glm::dvec3 mixed_u(glm::dvec3 rotdist, double be, double bz);
  static bool rect_box(glm::dvec3 pos, glm::dvec3 size);
  static bool hex_box(glm::dvec3 pos, glm::dvec3 size);
  static bool romb_box(glm::dvec3 pos, glm::dvec3 size);
  static int Love_function(const gsl_vector *x, void *par, gsl_vector *result_funct);
  static int Beta_function(const gsl_vector *x, void *par, gsl_matrix *jac);
  static int Love_fdf(const gsl_vector *x, void *par, gsl_vector *result_funct, gsl_matrix *jac);
  
};
#endif // CALC_H

/*
int Love_function(const gsl_vector *x, void *par, gsl_vector *result_funct) {
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
  const double ux =  u0x - be /(2.*pi)*(atan2(yy,xx)+xx*yy/(2.*(1.-nu)));// - 0.5*be;
  const double uy = u0y + be /(8.*pi*(1.-nu)) * ((1.-nu-nu)*log(r2/r02) + (xx+yy)*(xx-yy));
  const double uz = u0z - bz/(2.*pi)*atan2(yy, xx);
  gsl_vector_set(result_funct, 0, ux);
  gsl_vector_set(result_funct, 1, uy);
  gsl_vector_set(result_funct, 2, uz);
  return GSL_SUCCESS;
}

int Beta_function(const gsl_vector *x, void *par, gsl_matrix *jac) {
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

int Love_fdf(const gsl_vector *x, void *par, gsl_vector *result_funct, gsl_matrix *jac) {
  Love_function(x, par, result_funct);
  Beta_function(x, par, jac);
  return GSL_SUCCESS;
}

void singledisl(int n_a) { // Liczy pole przemieszczeń od dyslokacji ze rdzeniem przechodzącym przez punk n_a  
  printMat("rotation tensor", rot_tensor);
  glm::dvec3 burgers_vector = rot_tensor * (mil.fraction * crysCell->c2o * glm::dvec3(mil.indices[0], mil.indices[1], mil.indices[2]));
//  cout << " mil.indices   " << mil.indices[0] << "    " << mil.indices[1] << "    " << mil.indices[2] << endl;
//  cout << " burgers_vector   " << burgers_vector.x << "    " << burgers_vector.y << "    " << burgers_vector.z << endl;
  glm::dvec3 cd = rot_tensor * atoms->coord[n_a];
  double be=sqrt(burgers_vector.x*burgers_vector.x+burgers_vector.y*burgers_vector.y);
  double bz = burgers_vector.z;
  cout << " be=" << be << "     bz=" << bz << endl;
  for (int i=0; i<atoms->n_atoms; i++) coord1[i] = rot_tensor * atoms->coord[i];

  atoms1Print("rotatedCoord.xyz", coord1);

  if ( sw_iter ) {

    size_t count;
    struct params p;
    p.be = be;
    p.bz = bz;
    int status;
    glm::dvec3 diff;
    double crit = 0.;
//  stringstream sss;miller parse_miller(string line)
//  sss << "C1-" << NumLine << ".xyz";
//  extPrint(sss.str(), atoms->n_atoms, coord1);

    const gsl_multiroot_fdfsolver_type *T;
    if ( sw_powell ) T = gsl_multiroot_fdfsolver_hybridsj;
    else             T = gsl_multiroot_fdfsolver_newton;
    for (int i=0; i<n_Atoms; i++) {
//    if ( i>=atoms->n_atoms-n_addAtoms ) continue;
      if ( i==n_a ) { cout<< "Error for n_a=" << n_a << "   i=" << i << endl;  continue; }
      count = 0;
      do {
         gsl_multiroot_fdfsolver *s; // = gsl_multiroot_fdfsolver_alloc(T, 3);
         gsl_vector *x; // = gsl_vector_alloc(3);		  
         count++;
         p.u0x = atoms->du[i].x;
         p.u0y = atoms->du[i].y;
         p.u0z = atoms->du[i].z;
         glm::dvec3 temp = coord1[i]+atoms->du[i] - cd;
//       if ( count==1) fout << i+1 << "    " << temp.x << ", " << temp.y << ", " << temp.z << endl; 
         if ( (temp.x*temp.x+temp.y*temp.y)<1.e-10 ) {
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
//      if ( i<2 ) cout << " i=" << i << "   count=" << count << "   temp  " << temp.x << "   " << temp.y << "    " << temp.z << endl;
         s = gsl_multiroot_fdfsolver_alloc(T, 3);
         gsl_multiroot_fdfsolver_set(s, &f, x);

         status = gsl_multiroot_fdfsolver_iterate(s);

         if ( status ) break;
         diff.x = gsl_vector_get(s->f, 0);
         diff.y = gsl_vector_get(s->f, 1);
         diff.z = gsl_vector_get(s->f, 2);
       if ( i<2 ) cout << " i=" << i << "   count=" << count << "   diff  " << diff.x << "   " << diff.y << "    " << diff.z << endl;
//         if ( i==0 ) cout << diff.x << "   " << diff.y << "    " << diff.z << endl;
         atoms->du[i] -= diff;
//       if ( i<2 ) cout << " i=" << i << "   count=" << count << "   atoms->du[i]  " << atoms->du[i].x << "   " << atoms->du[i].y << "    " << atoms->du[i].z << endl;
         status = gsl_multiroot_test_residual(s->f, crit_stop);
         gsl_multiroot_fdfsolver_free(s);
         gsl_vector_free(x);
      } while ( status==GSL_CONTINUE && count<countN_R );
      crit += diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
//    cout << " --- i = " << i << "     count = " << count << endl;
      continue;
   _END:
      cout << "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << endl;
      n_Errors++;
    }
//  fout.close();
    cout << " --- n_a = " << n_a << "     crit = " << crit << endl;

  } else {  // !sw_iter
    for (int i=0; i<n_Atoms; i++) {
//    if ( i>=atoms->n_atoms-n_addAtoms ) continue;
      if ( i==n_a ) { cout<< "Error for n_a=" << n_a << "   i=" << i << endl;  continue; }
      glm::dvec3 dist1 = coord1[i] - cd;
      atoms->du[i] = mixed_u(i, dist1, be, bz);
    }
  }#ifndef GENER_H
#define GENER_H

#include "settings.h"
#include "atoms.h"
#include "lattice.h"
 
class Gener
{ 

  for (int i=0; i<n_Atoms; i++) //{
    atoms->du[i] = rot_inv*atoms->du[i];
//    atoms->beta[i] = rot_inv*mixed_beta(i, coord1[i]+atoms->du[i]-cd, be, bz)*rot_tensor;} //jeżeli jest potrzebne beta
}
*/ 
