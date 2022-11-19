#include "../MathLib/SpaceShapeLagrange2N_1D_2.h"
#include "BarFiniteElement.h"
#include "FEException.h"

CSpaceShapeLagrange2N_1D_2 sf_bar3D;

CFiniteElementInstanceBar::CFiniteElementInstanceBar( CGeometricDomain &gd , const CSpaceShapeApproxPtr< CFENode > &s):CFiniteElementInstance(gd,s)
{
    if ( s.GetPoints().size() != 2 ) throw CFEException("CFiniteElementInstanceBar::CFiniteElementInstanceBar - bar element allways should have two nodes");
}

CFiniteElementInstanceBar::~CFiniteElementInstanceBar(void)
{
}


void CFiniteElementInstanceBar::GetXi( const mvector &X, mvector &xi ) const
{

}

void CFiniteElementInstanceBar::GetX( const mvector &xi, mvector &X ) const
{
	X.SetDim( xi.GetDim() );
    mvector x1 , x2 ;
    const vector< CFENode* > &nodes = main_shape->GetPoints();
    nodes[0]->GetX( x1 );
    nodes[1]->GetX( x2 );

	X = 0.5 * ( ( x2 - x1 ) * xi + ( x1 + x2 ) );
}

double CFiniteElementInstanceBar::ComputeLength() const
{
    mvector X;
    const vector< CFENode* > &nodes = main_shape->GetPoints();
     nodes[ 1 ]->GetX( X );
    return nodes[0]->Distance( X );
}
