#include "../include/gener.h"
#include <iostream>

extern Internal *INT;
extern Lattice *LATT;
extern Atoms *AT;

int Gener::genLattice(const int nx0, const int ny0, const int nz0, const int nx1, const int ny1, const int nz1)
{
    int ak, count = 0;
    QList<int> numbers;
    numbers.clear();
    LATT->clearL();
    for (int i=0; i<INT->crC->numCellAt; i++) {
       ak = INT->crC->cellAt.at(i);
       numbers << ak;
       if ( LATT->n_k==0 ) { LATT->nK->push_back(ak);  LATT->n_k=1;  LATT->kName << AT->namea.at(ak); }
       bool sw = true;
       for (int j=0; j<LATT->n_k; j++)  if ( ak==LATT->nK.get()->at(j) ) sw = false;
       if ( sw ) { LATT->nK->push_back(ak); ++LATT->n_k;  LATT->kName << AT->namea.at(ak); }       
    }
 //std::cout << "Gener n_k=" << LATT->n_k << std::endl;
    for (int k=nz0; k<nz1; k++) {
       for (int j=ny0; j<ny1; j++) {
          for (int i=nx0; i<nx1; i++) {
             for (int an=0; an<INT->crC->numCellAt; an++) {
	        //LATT->marked.push_back(0); 
		count++;
		glm::dvec3 hic = glm::dvec3(double(i)+INT->crC->cellCoord.at(an).x, double(j)+INT->crC->cellCoord.at(an).y, double(k)+INT->crC->cellCoord.at(an).z);
		glm::dvec3 pos = INT->crC->c2o * hic;
		LATT->coords.push_back(pos); ////LATT->coords->push_back( osg::Vec3d(x, y, z));//, static_cast<double>(ak)) );
		double x = pos.x;  double y = pos.y;  double z = pos.z;
		if ( x<LATT->xMin ) LATT->xMin = x;
		if ( x>LATT->xMax ) LATT->xMax = x;
		if ( y<LATT->yMin ) LATT->yMin = y;
		if ( y>LATT->yMax ) LATT->yMax = y;
		if ( z<LATT->zMin ) LATT->zMin = z;
		if ( z>LATT->zMax ) LATT->zMax = z;
		LATT->nAt->push_back(numbers.at(an));
    }  }  }  }
    LATT->scDim = std::max(LATT->xMax-LATT->xMin, std::max(LATT->yMax-LATT->yMin, LATT->zMax-LATT->zMin));
    LATT->n_bonds = 0;    

    LATT->marked.assign(count, 0);
    LATT->u.assign(count, glm::dvec3(0., 0., 0.));
    LATT->du.assign(count, glm::dvec3(0., 0., 0.));

    return count;
}

// Generate bonds between atoms based on proximity. Two atoms are bonded when
// their distance is no larger than the sum of their covalent radii plus a
// tolerance, and they are of different elements (no homogeneous bonds).
// Mirrors create_bonds() from the wurtzite generator. Atom indices stored in
// bond1/bond2 are 1-based (the scene renderer subtracts 1). Any previously
// generated bonds are cleared first, so this also serves to regenerate bonds
// after the atom coordinates change.
int Gener::genBonds(double tolerance)
{
    LATT->bond1->clear();
    LATT->bond2->clear();

    const int n = LATT->n_atoms;
    for (int i=0; i<n; i++) {
       int ai = LATT->nAt.get()->at(i);
       double ri = AT->a_rad1[ai];
       glm::dvec3 ci = LATT->coords[i];
       for (int j=i+1; j<n; j++) {
          int aj = LATT->nAt.get()->at(j);
          if ( ai==aj ) continue;  // avoid homogeneous bonds
          double maxDist = ri + AT->a_rad1[aj] + tolerance;
          glm::dvec3 d = ci - LATT->coords[j];
          double dist2 = d.x*d.x + d.y*d.y + d.z*d.z;
          if ( dist2 <= maxDist*maxDist ) {
             LATT->bond1->push_back(i+1);
             LATT->bond2->push_back(j+1);
          }
       }
    }
    LATT->n_bonds = static_cast<int>(LATT->bond1->size());
    return LATT->n_bonds;
}
