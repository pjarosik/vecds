
#include "SpaceShapeLagrange4N_2D_16.h"
#include "SpaceShapeLagrange4N_1D_4.h"

CSpaceShapeLagrange4N_2D_16::CSpaceShapeLagrange4N_2D_16():CSpaceShapeLagrange4N( 2 )
{
    unsigned k;
    for( k = 0; k < 4; k++ )
        edgeshapes.push_back( new CSpaceShapeLagrange4N_1D_4 );

    CreateInds();
    dg=3;
}

CSpaceShapeLagrange4N_2D_16::~CSpaceShapeLagrange4N_2D_16(void)
{
}


void CSpaceShapeLagrange4N_2D_16::transform( const mvector &xi, mvector &N ) const
{
	double Lx[ 4 ], Ly[ 4 ];
	
	Calc_L( xi[ 0 ], Lx );
	Calc_L( xi[ 1 ], Ly );

    unsigned k,l,ind=0,dgN = dg + 1;

	N.SetDim(16);

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
			N[ ind++ ] = Lx[ l ] * Ly[ k ];

}

void CSpaceShapeLagrange4N_2D_16::jacobian( const mvector &xi, matrix &dN ) const
{
	double dLx[ 4 ][ 4 ], dLy[ 4 ][ 4 ];

	Calc_dL( xi[ 0 ], dLx );
	Calc_dL( xi[ 1 ], dLy );

    unsigned k, l, ind = 0, dgN = dg + 1;

	dN.SetDim(16,2);

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
			dN( ind++, 0 ) = dLx[ l ][ 1 ] * dLy[ k ][ 0 ];

	ind = 0;

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
			dN( ind++, 1 ) = dLx[ l ][ 0 ] * dLy[ k ][ 1 ];
}


