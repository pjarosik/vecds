
#include "SpaceShapeLagrange4N_1D_4.h"

CSpaceShapeLagrange4N_1D_4::CSpaceShapeLagrange4N_1D_4():CSpaceShapeLagrange4N( 1 )
{
    dg=3;
}

CSpaceShapeLagrange4N_1D_4::~CSpaceShapeLagrange4N_1D_4(void)
{
}


void CSpaceShapeLagrange4N_1D_4::transform( const mvector &xi, mvector &N ) const
{
	double L[ 4 ];
	
	Calc_L( xi[ 0 ], L );


	N.SetDim(4);

	N[ 0 ] = L[ 0 ];
	N[ 1 ] = L[ 1 ];
	N[ 2 ] = L[ 2 ];
	N[ 3 ] = L[ 3 ];
}

void CSpaceShapeLagrange4N_1D_4::jacobian( const mvector &xi, matrix &dN ) const
{
	double dL[ 4 ][ 4 ];

	Calc_dL( xi[ 0 ], dL );

	dN.SetDim( 4, 1 );

	dN( 0, 0 ) = dL[ 0 ][ 1 ];
	dN( 1, 0 ) = dL[ 1 ][ 1 ];
	dN( 2, 0 ) = dL[ 2 ][ 1 ];
	dN( 3, 0 ) = dL[ 3 ][ 1 ];
}


