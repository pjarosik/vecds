
#include "SpaceShapeLagrange2N_1D_2.h"

CSpaceShapeLagrange2N_1D_2::CSpaceShapeLagrange2N_1D_2():CSpaceShapeLagrange2N( 1 )
{
    dg=1;
}

CSpaceShapeLagrange2N_1D_2::~CSpaceShapeLagrange2N_1D_2(void)
{
}


void CSpaceShapeLagrange2N_1D_2::transform( const mvector &xi, mvector &N ) const
{
	double L[ 2 ];
	
	Calc_L( xi[ 0 ], L );


	N.SetDim(2);

	N[ 0 ] = L[ 0 ];
	N[ 1 ] = L[ 1 ];
}

void CSpaceShapeLagrange2N_1D_2::jacobian( const mvector &xi, matrix &dN ) const
{
	double dL[ 2 ][ 4 ];

	Calc_dL( xi[ 0 ], dL );

	dN.SetDim(2,1);

	dN(0,0) = dL[0][1];
	dN(1,0) = dL[1][1];
}
