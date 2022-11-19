
#include "FiniteElementInstance1D.h"


CFiniteElementInstance1D::CFiniteElementInstance1D( CGeometricDomain &gd, CSpaceShapeApproxPtr< CFENode > &s):CFiniteElementInstance(gd,s)
{
}

CFiniteElementInstance1D::~CFiniteElementInstance1D(void)
{
}

double CFiniteElementInstance1D::GetLength( ) const 
{ 
    const vector< CFENode* > &nodes = main_shape->GetPoints();
    if ( nodes.size() != 2 ) throw CFEException("CFiniteElementInstance1D::GetLength( ), element 1D should have two nodes");
    mvector X;
    nodes[ 1 ]->GetX(X);
    return nodes[ 0 ]->Distance( X );
}

void CFiniteElementInstance1D::ApproximateAllNodalResults( )
{

}
