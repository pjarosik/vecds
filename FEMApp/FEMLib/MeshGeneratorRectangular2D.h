/* 
 * File:   CMeshGeneratorRectangular2D.h
 * Author: piotrek
 *
 * Created on November 21, 2011, 12:45 PM
 */

#ifndef CMESHGENERATORRECTANGULAR2D_H
#define	CMESHGENERATORRECTANGULAR2D_H

#include "MeshGenerator.h"
#include "../MathLib/SpaceShape.h"

class CMeshGeneratorRectangular2D: public CMeshGenerator {
public:
    CMeshGeneratorRectangular2D(const CSpaceShape& sf, unsigned x, unsigned y, unsigned g = 0 );
    virtual ~CMeshGeneratorRectangular2D();
    virtual bool Generate();
    void GenerateRectangularArea( double x1, double y1, double x2, double y2 );
    void GetElemIndices( unsigned k, unsigned &ix, unsigned &iy ) const { ix = k % nx; iy = k / nx; }

protected:
    
    const CSpaceShape &sfe;
    unsigned nx, ny, degree, ground;
    vector< vector< int > > mesh;

    void PutNode( unsigned k, unsigned l, unsigned &n ) { if ( mesh[k][l] == -1 ) { mesh[k][l] = n++; } }
};

#endif	/* CMESHGENERATORRECTANGULAR2D_H */

