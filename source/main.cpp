#include "../include/mainwindow.h"

Atoms *AT;
Lattice *LATT;
Internal *INT;
Adds *ADDS;

int main(int argc, char *argv[])
{
  //Q_INIT_RESOURCE(stylesheet);

  QApplication app(argc, argv);
  if ( ( QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_5)==0 ) {
       QMessageBox::critical(0, "OpenGL features missing", "OpenGL version 1.5 or higher is required to run this program.\n",  "The program will now exit.");
       return -1;
  }
 
  QLocale::setDefault(QLocale::C);
  
  AT = new Atoms();
  INT = new Internal();
  LATT = new Lattice();
  ADDS = new Adds();  
  
  if ( argc>1 ) INT->atName = std::string(argv[1]);

  MainWindow mainwindow;
//  mainwindow.setStyleSheet("padding: 0px;");// Display that context on screen:
  mainwindow.show ();

  return app.exec ();	
}
/*
     if ( switch_hex || switch_rect || switch_romb ) {
        if ( switch_rect ) {
           cout << "switch_rect";
           for (int i=0; i<numbAtoms; i++ ) visible[i] = rect_box(Coords[i]);
           for (int i=0; i<numbAdds; i++ )  visible[i+numbAtoms] = rect_box(AddCoords[i]);
        }
        if ( switch_hex ) {
           cout << "switch_hex";
           for (int i=0; i<numbAtoms; i++ ) visible[i] = hex_box(Coords[i]);
           for (int i=0; i<numbAdds; i++ )  visible[i+numbAtoms] = hex_box(AddCoords[i]);
        }
        if ( switch_romb ) {
           cout << "switch_romb";
           for (int i=0; i<numbAtoms; i++ ) visible[i] = romb_box(Coords[i]);
           for (int i=0; i<numbAdds; i++ )  visible[i+numbAtoms] = romb_box(AddCoords[i]);
        }
============================================================================================
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

struct params {
  double be;
  double bz;
  double u0x;
  double u0y;
  double u0z;
};

  unitMatrix = glm::dmat3(1., 0., 1.,
                          0., 1., 0.,
                          0., 0., 1.);
			  
  atoms->du     = new glm::dvec3[atoms->n_atoms];
  atoms->u      = new glm::dvec3[atoms->n_atoms];
  atoms->beta   = new glm::dmat3[atoms->n_atoms];
  for (int i=0; i<atoms->n_atoms; i++) atoms->u[i] = atoms->du[i] = glm::dvec3(0., 0., 0.); //atoms->uu[i] = 
  for (int i=0; i<atoms->n_atoms; i++) atoms->beta[i] = glm::dmat3(0.,0.,0., 0.,0.,0., 0.,0.,0.);

  coord1 = new glm::dvec3[atoms->n_atoms];			  

void singledisl(int n_a) { // Liczy pole przemieszczeń od dyslokacji ze rdzeniem przechodzącym przez punk n_a  

  printMat("rotation tensor", rot_tensor);
  glm::dvec3 burgers_vector = rot_tensor * (mil.fraction * crysCell->c2o * glm::dvec3(mil.indices[0], mil.indices[1], mil.indices[2]));
//  cout << " mil.indices   " << mil.indices[0] << "    " << mil.indices[1] << "    " << mil.indices[2] << endl;
//  cout << " burgers_vector   " << burgers_vector.x << "    " << burgers_vector.y << "    " << burgers_vector.z << endl;
  glm::dvec3 cd = rot_tensor * atoms->coord[n_a];
  double be=sqrt(burgers_vector.x*burgers_vector.x+burgers_vector.y*burgers_vector.y);
  double bz = burgers_vector.z;
  if ( nowy_switch ) {
      be = vector_be; bz = vector_bz; }
  cout << " be=" << be << "     bz=" << bz << endl;
  for (int i=0; i<atoms->n_atoms; i++) coord1[i] = rot_tensor * atoms->coord[i];

//  atoms1Print("rotatedCoord.xyz", coord1);

  int count_moved = 0;
  if ( sw_iter ) {

    size_t count;
    struct params p;
    p.be = be;
    p.bz = bz;
    int status;
    glm::dvec3 diff;
    double crit = 0.;
    
    stringstream sss;
    sss << "numb-" << NumLine << ".txt";
//  extPrint(sss.str(), atoms->n_atoms, coord1);
    ofstream f_numb(sss.str().c_str());
    const gsl_multiroot_fdfsolver_type *T;
    if ( sw_powell ) T = gsl_multiroot_fdfsolver_hybridsj;
    else             T = gsl_multiroot_fdfsolver_newton;
    for (int i=0; i<n_Atoms; i++) {
//    if ( i>=atoms->n_atoms-n_addAtoms ) continue;
      if ( i==n_a ) { cout<< "Error for n_a=" << n_a << "   i=" << i << endl;  continue; }
      count = 0;
      glm::dvec3 dist1 = coord1[i] - cd;
      glm::dvec3 du0 = mixed_u(i, dist1, be, bz);
      double duNorm = 0.8*glm::length(du0);
      double ddd = 0.;
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
         atoms->du[i] -= diff;
         ddd = glm::distance(du0, atoms->du[i]); 

         status = gsl_multiroot_test_residual(s->f, crit_stop);
         gsl_multiroot_fdfsolver_free(s);
         gsl_vector_free(x);
      } while ( status==GSL_CONTINUE && count<countN_R );
      crit += diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
//    cout << " --- i = " << i << "     count = " << count << endl;
      if ( ddd>duNorm ) {
		 f_numb << i << endl;
         cout << " i=" << i << "   count=" << count << "   diff  " << diff.x << "   " << diff.y << "    " << diff.z << endl;
         cout << "  du0  = " << du0.x << ",  " << du0.y << ",  " << du0.z << endl;
         cout << "  du  = " << atoms->du[i].x << ",  " << atoms->du[i].y << ",  " << atoms->du[i].z << endl;
         cout << " duNorm = " << duNorm << "        ddd = " << ddd << "       coord X = " << atoms->coord[i].x << "       coord Y = " << atoms->coord[i].y << endl;
         double rrr = sqrt(atoms->coord[i].x*atoms->coord[i].x + atoms->coord[i].y*atoms->coord[i].y);
         cout << "       x = " << atoms->coord[i].x/rrr << "       y = " << atoms->coord[i].y/rrr << endl;   
         cout << "+++++++++   atom nr. " << i << " was moved!" << endl;
         atoms->du[i].x = du0.x;
//         atoms->du[i].y = 0.5*(atoms->du[i].y+du0.y);
//         atoms->du[i].z = 0.5*(atoms->du[i].z+du0.z);         
//         atoms->du[i] = du0;
         count_moved++;
      }
      continue;
   _END:
      cout << "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << endl;
      n_Errors++;
    }
    cout << "  count_moved = " << count_moved << endl;
    f_numb << "  count_moved = " << count_moved << endl;
    f_numb.close();

//  fout.close();
    cout << " --- n_a = " << n_a << "     crit = " << crit << endl;

  } else {  // !sw_iter
    for (int i=0; i<n_Atoms; i++) {
//    if ( i>=atoms->n_atoms-n_addAtoms ) continue;
      if ( i==n_a ) { cout<< "Error for n_a=" << n_a << "   i=" << i << endl;  continue; }
      glm::dvec3 dist1 = coord1[i] - cd;
      atoms->du[i] = mixed_u(i, dist1, be, bz);
    }
  
  }
  for (int i=0; i<n_Atoms; i++) //{
    atoms->du[i] = rot_inv*atoms->du[i];
//    atoms->beta[i] = rot_inv*mixed_beta(i, coord1[i]+atoms->du[i]-cd, be, bz)*rot_tensor;} //jeżeli jest potrzebne beta
  
}
---------------------------------------------------------------------------------
    rot_tensor = rotation_tensor();         // nowy kolejny obrót
    rot_inv = glm::transpose(rot_tensor);}   //  jego odwrotność
glm::dvec3 bV = ADDS->rotTens.at(i) * ADDS->fracts.at(i) * (&(INT->structList[ADDS->crCNum.at(i)]))->c2o * ADDS->millerVs.at(i);    
-------------------------------------------------------------------------------
glm::dvec3 mixed_u(int i, glm::dvec3 rotdist, double be, double bz)
{ double r2 = rotdist.x*rotdist.x + rotdist.y*rotdist.y;
  if ( r2<1e-15 ) {
     cout << " Atom " << i << " in the center of dislocation core" << endl;
     n_errors++;
//     return  glm::dvec3(0.25*be, 0., 0.);} 
     return  glm::dvec3(0., 0., 0.);} 
  else {
     double r = sqrt(r2);
     double x = rotdist.x/r;
     double y = rotdist.y/r;
     double r02 = rad_fact*be*be; // radius of inmobile ring relative to which the atoms in the core move up
     double ux =  be /(2.*pi)*(atan2(y,x)+x*y/(2.*(1.-nu)));// - 0.5*be;
     double uy = -be /(8.*pi*(1.-nu)) * ((1.-nu-nu)*log(r2/r02) + (x+y)*(x-y));
     return glm::dvec3(ux, uy, bz/(2.*pi)*atan2(y, x));} } // mixed_u

glm::dmat3 mixed_beta(int i, glm::dvec3 rotdist, double be, double bz)
{   glm::dmat3 b(0.,0.,0.,  0.,0.,0.,  0.,0.,0.);
    double x=rotdist.x;
    double x2=x*x;
    double y = rotdist.y;
    double y2=y*y;
    double r2=x2+y2;
    if ( r2<1.e-15 ) cout << " Atom " << i << " in the center of dislocation core" << endl;
    else {
       double a = be/(4. * pi * (1.-nu) * r2*r2);             // a = bx/(4. * pi * (1.-n) * r2*r2)
       double bxx = -a * y * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2); // u(4) = -a * y * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xx 
       double byx = -a * x * ((1.-2.*nu)*x2 + (3.-2.*nu)*y2); // u(5) = -a * x * ((1.-2.*n)*x*x + (3.-2.*n)*y*y) !yx
       double bzx = -bz/(2.*pi) * y/r2;                       // u(6) = -bz/(2.*pi) * y/r2                       !zx
       double bxy =  a * x * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2); // u(7) =  a * x * ((3.-2.*n)*x*x + (1.-2.*n)*y*y) !xy
       double byy =  a * y * ((1.+2.*nu)*x2 - (1.-2.*nu)*y2); // u(8) =  a * y * ((1.+2.*n)*x*x - (1.-2.*n)*y*y) !yy
       double bzy = -bz/(2.*pi) * x/r2;                       // u(9) = -bz/(2.*pi) * x/r2                       !zy
//       if ( r2>=1.*(be*be+bz*bz) )                            // on the base of ~/pawel/papers/IMCE09/beta.mac
       b = glm::dmat3( bxx, bxy, 0.,   byx, byy, 0.,    bzx, bzy, 0. );
    }
    return b;
}
	-------------------------------------------------------------------------------------------------------
                 for (int i=0; i<numbAtoms; i++ ) if ( !crysplane(Coords[i], toDouble(fields[0]), toDouble(fields[1]), toDouble(fields[2]), toInt(fields[3]), toInt(fields[4]), toInt(fields[5])) )
                                                         visible[i] = false;

bool crysplane(glm::dvec3 coord, double point_x, double point_y, double point_z, int mill1, int mill2, int mill3)
{
  //cout << "coord=" << coord.x << ",  " << coord.y << ",  " << coord.z << endl;
  double aa = 1./a * double(mill1);
  double bb = 1./(sqrt(3.)*a) * double(mill1) + 2./(sqrt(3.)*b) * double(mill2);
  double cc = 1./c * double(mill3);
  glm::dvec3 c1 = glm::dvec3(double(mill1), double(mill2), double(mill3)) * o2c;// glm::inverse(c2o);
// cout << "c1=" << c1.x << ",  " << c1.y << ",  " << c1.z << endl;
  if ( abs(aa-c1.x)>1.e-8 || abs(bb-c1.y)>1.e-8 || abs(cc-c1.z)>1.e-8 ) cout << "coord=" << coord.x << ",  " << coord.y << ",  " << coord.z << endl;
//cout << "abc=" << aa << ",  " << bb << ",  " << cc << endl;
  double dd = sqrt(aa*aa+bb*bb+cc*cc);
  aa /= dd;
  bb /= dd;
  cc /= dd;
  dd = - aa*point_x - bb*point_y - cc*point_z;
//  cout << "dd=" << dd << endl;
  return (aa*coord.x+bb*coord.y+cc*coord.z+dd)<=0.;
}

//bool pyrhex(double, double, double, double, double, double)
//{
//}
================
c     Eqn. \ref{gsfkk} in the manuscipt of a book by Paweł Dłużewski
      a =  1./aa(1)           *muller(1)              
      b =  1./(sqrt(3.)*aa(1))*muller(1) + 2./(sqrt(3.)*aa(2))*muller(2)
      c =  1./aa(3)           *muller(4)

c     Eqn (3) p.171, Geometria Analityczna by F.Leja PWN 1966.  
c     ax + by + cz + d = 0                                      
      d = sqrt(a*a+b*b+c*c)                                     
      a = a/d                                                   
      b = b/d                                                   
      c = c/d                                                   
      d = - a*xplane(1) - b*xplane(2) - c*xplane(3)             
                                                                
c     Is the coord upper or below the plane?                    
                                                                
      crysplane = (a*coord(1)+b*coord(2)+c*coord(3)+d) .le. 0.d0
===============	
*/
