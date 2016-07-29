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
