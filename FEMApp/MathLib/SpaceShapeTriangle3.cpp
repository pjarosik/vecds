
#include "SpaceShapeTriangle3.h"
#include "SpaceShapeLagrange2N_1D_2.h"

CSpaceShapeTriangle3::CSpaceShapeTriangle3(void):CSpaceShape(ST_TRIANGLE,2,3)
{
    dg=1;
    unsigned k;
    for( k = 0; k < 3; k++ )
        edgeshapes.push_back( new CSpaceShapeLagrange2N_1D_2() );

    nodes.resize(3);
    nodes[0][0] =  1.0;  nodes[0][1] = 0.0;
    nodes[1][0] =  0.0;  nodes[1][1] = 1.0;
    nodes[2][0] =  0.0;  nodes[2][1] = 0.0;

    CreateInds();
}

CSpaceShapeTriangle3::~CSpaceShapeTriangle3(void)
{
}

void CSpaceShapeTriangle3::transform( const mvector &xi, mvector &N ) const
{
	N.SetDim(3);
	N[ 0 ] = xi[ 0 ];
	N[ 1 ] = xi[ 1 ];
	N[ 2 ] = 1.0 - xi[ 0 ] - xi[ 1 ];

}

void CSpaceShapeTriangle3::jacobian( const mvector &xi, matrix &dN ) const
{

	dN.SetDim(3,2);

	dN( 0, 0 ) =  1.0;
	dN( 1, 0 ) =  0.0;
	dN( 2, 0 ) = -1.0;

	dN( 0, 1 ) =  0.0;
	dN( 1, 1 ) =  1.0;
	dN( 2, 1 ) = -1.0;

}
