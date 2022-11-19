
#include "SpaceShapeLagrange3N_1D_3.h"

CSpaceShapeLagrange3N_1D_3::CSpaceShapeLagrange3N_1D_3():CSpaceShapeLagrange3N( 1 )
{
    dg=2;
}

CSpaceShapeLagrange3N_1D_3::~CSpaceShapeLagrange3N_1D_3(void)
{
}


void CSpaceShapeLagrange3N_1D_3::transform( const mvector &xi,mvector &N ) const
{
	double L[ 3 ];
	
	Calc_L( xi[ 0 ], L );


	N.SetDim(3);

	N[ 0 ] = L[ 0 ];
	N[ 1 ] = L[ 1 ];
	N[ 2 ] = L[ 2 ];
}

void CSpaceShapeLagrange3N_1D_3::jacobian( const mvector &xi, matrix &dN ) const
{
	double dL[ 3 ][ 4 ];

	Calc_dL( xi[ 0 ], dL );

	dN.SetDim(3,1);

	dN(0,0) = dL[0][1];
	dN(1,0) = dL[1][1];
	dN(2,0) = dL[2][1];
}


