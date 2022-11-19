
#include "SpaceShapeLagrange3N_2D_9.h"
#include "SpaceShapeLagrange3N_1D_3.h"

CSpaceShapeLagrange3N_2D_9::CSpaceShapeLagrange3N_2D_9():CSpaceShapeLagrange3N( 2 )
{
    unsigned k;
    for( k = 0; k < 4; k++ )
        edgeshapes.push_back( new CSpaceShapeLagrange3N_1D_3 );

    dg=2;
    CreateInds();
}

CSpaceShapeLagrange3N_2D_9::~CSpaceShapeLagrange3N_2D_9(void)
{
}


void CSpaceShapeLagrange3N_2D_9::transform( const mvector &xi,mvector &N ) const
{
	double Lx[ 3 ], Ly[ 3 ];
	
	Calc_L( xi[ 0 ], Lx );
	Calc_L( xi[ 1 ], Ly );

    unsigned k, l, ind=0, dgN = dg + 1;

	N.SetDim(9);

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
			N[ ind++ ] = Lx[ l ] * Ly[ k ];

}

void CSpaceShapeLagrange3N_2D_9::jacobian( const mvector &xi, matrix &dN ) const
{
	double dLx[ 3 ][ 4 ], dLy[ 3 ][ 4 ];

	Calc_dL( xi[ 0 ], dLx );
	Calc_dL( xi[ 1 ], dLy );

    unsigned k, l, ind = 0,dgN = dg + 1;

	dN.SetDim(9,2);

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
			dN( ind++, 0 ) = dLx[ l ][ 1 ] * dLy[ k ][ 0 ];

	ind = 0;

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
			dN( ind++, 1 ) = dLx[ l ][ 0 ] * dLy[ k ][ 1 ];
}




