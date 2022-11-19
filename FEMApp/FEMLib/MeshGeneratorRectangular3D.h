/* 
 * File:   CMeshGeneratorRectangular3D.h
 * Author: piotrek
 *
 * Created on November 21, 2011, 12:46 PM
 */

#ifndef CMESHGENERATORRECTANGULAR3D_H
#define	CMESHGENERATORRECTANGULAR3D_H

#include "MeshGenerator.h"
#include "../MathLib/SpaceShape.h"

class CMeshGeneratorRectangular3D:public CMeshGenerator {
public:
    CMeshGeneratorRectangular3D( const CSpaceShape& sf, unsigned x, unsigned y, unsigned z );
    virtual ~CMeshGeneratorRectangular3D();
     virtual bool Generate();
    void GenerateRectangularArea( double x1, double y1,double z1, double x2, double y2, double z2);
    void GenerateShapeArea( const CSpace *s, const vector< mvector > &xp );
//    void GetElemIndices( unsigned k, unsigned &ix, unsigned &iy, unsigned &iz ) const { ix = k % nx; iy = k / nx; }

private:

    const CSpaceShape &sfe;
    unsigned nx, ny,nz, degree;
    vector< vector< vector< int > > > mesh;
};

#endif	/* CMESHGENERATORRECTANGULAR3D_H */

