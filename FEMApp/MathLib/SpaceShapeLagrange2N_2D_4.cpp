
#include "SpaceShapeLagrange2N_2D_4.h"
#include "SpaceShapeLagrange2N_1D_2.h"

CSpaceShapeLagrange2N_2D_4::CSpaceShapeLagrange2N_2D_4():CSpaceShapeLagrange2N( 2 )
{
    unsigned k;
    for( k = 0; k < 4; k++ )
        edgeshapes.push_back( new CSpaceShapeLagrange2N_1D_2 );

    dg = 1;
    CreateInds();

}

CSpaceShapeLagrange2N_2D_4::~CSpaceShapeLagrange2N_2D_4( void )
{
}


void CSpaceShapeLagrange2N_2D_4::transform( const mvector &xi, mvector &N ) const
{
	double Lx[ 2 ], Ly[ 2 ];
	
	Calc_L( xi[ 0 ], Lx );
	Calc_L( xi[ 1 ], Ly );

    unsigned k, l, ind = 0, dgN = dg + 1;

    N.SetDim( 4 );

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
			N[ ind++ ] = Lx[ l ] * Ly[ k ];

}

void CSpaceShapeLagrange2N_2D_4::jacobian( const mvector &xi, matrix &dN ) const
{
	double dLx[ 2 ][ 4 ], dLy[ 2 ][ 4 ];

	Calc_dL( xi[ 0 ], dLx );
	Calc_dL( xi[ 1 ], dLy );

    unsigned k, l, ind = 0, dgN = dg + 1;

	dN.SetDim(4,2);

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
			dN( ind++, 0 ) = dLx[ l ][ 1 ] * dLy[ k ][ 0 ];

	ind = 0;

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
			dN( ind++, 1 ) = dLx[ l ][ 0 ] * dLy[ k ][ 1 ];
}


