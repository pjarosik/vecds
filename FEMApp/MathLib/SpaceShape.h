#ifndef SPACESHAPE_H
#define SPACESHAPE_H

#include "Space.h"
#include "valarray"
#include "constants.h"

using namespace std;

/*

   *****************************************************************
  * Class: CSpaceShape                                              *
 *  Base class for shapes approximations                             *
*                                                                     *
*                                                                     *
 *                                                                   *
  * Piotr Tauzowski (C) 2012                                        *
   *****************************************************************

*/


inline unsigned upow( unsigned a, int b )
{
    unsigned c = a;
    while( --b ) c *= a;
    return c;
}

vector< unsigned >& operator << ( vector< unsigned > &ov, unsigned i);

class CSpaceShape : public CSpace
{

public:

    CSpaceShape(SHAPE_TYPES t,unsigned a, unsigned b);
    CSpaceShape(SHAPE_TYPES t,unsigned a, unsigned b,unsigned ne,unsigned ns);
    CSpaceShape(const CSpaceShape &s );
    // CSpaceShape(CCopyer &cpr,const CSpaceShape &s);
    virtual ~CSpaceShape();

    virtual CSpaceShape* Clone( ) const = 0;
    void SelfTest( ostream &out ) const;
    unsigned GetDegree() const { return dg; }
    SHAPE_TYPES GetShapeType() const { return shape_type; }
    const vector< mvector >& GetLocalNodes() const { return nodes; }
    void GetCornerNodes(vector<unsigned> &cn ) const;

    const vector< CSpaceShape* >& GetEdgeShapes(  ) const { return edgeshapes; }
    const vector< vector< unsigned > > & GetEdgeInds() const { return edgeinds; }
    const vector< CSpaceShape* >& GetFaceShapes(  ) const { return faceshapes; }
    const vector< vector< unsigned > > & GetFaceInds() const { return faceinds; }

protected:

    vector< mvector > nodes;
    vector< CSpaceShape* > edgeshapes, faceshapes;
    vector< vector< unsigned > > edgeinds, faceinds;

    unsigned dim, dg;
    SHAPE_TYPES shape_type;

    void CreateInds();
    void CreateInds( const vector< CSpaceShape* > &ss, vector< vector< unsigned > > &si );
    int FindIndex( const mvector &ksi) const;
    void ExtractIndices( const CSpaceShape &os, vector< vector< unsigned > > &inds );
};

#endif // SPACESHAPE_H
