#ifndef GRAPHFE_H
#define GRAPHFE_H
#include "../FEMLib/FiniteElement.h"
#include "../MathLib/GeometricObject.h"

class CGraphFE
{
public:
    CGraphFE( CFiniteElement & );
    virtual ~CGraphFE();
protected:
    CFiniteElement &fe;
    vector< CGeometricObject* > gfe, owner, points, edges, faces, solids;
};

#endif // GRAPHFE_H
