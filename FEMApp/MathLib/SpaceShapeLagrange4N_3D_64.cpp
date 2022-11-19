
#include "SpaceShapeLagrange4N_3D_64.h"
#include "SpaceShapeLagrange4N_1D_4.h"
#include "SpaceShapeLagrange4N_2D_16.h"

CSpaceShapeLagrange4N_3D_64::CSpaceShapeLagrange4N_3D_64():CSpaceShapeLagrange4N( 3 )
{
    unsigned k;
    for( k = 0; k < 12; k++ )
        edgeshapes.push_back( new CSpaceShapeLagrange4N_1D_4 );

    for( k = 0; k < 6; k++ )
        faceshapes.push_back( new CSpaceShapeLagrange4N_2D_16 );

    CreateInds();
    dg=3;
}

CSpaceShapeLagrange4N_3D_64::~CSpaceShapeLagrange4N_3D_64(void)
{
}


void CSpaceShapeLagrange4N_3D_64::transform( const mvector &xi, mvector &N ) const
{
	double Lx[ 4 ], Ly[ 4 ], Lz[ 4 ];
	
	Calc_L( xi[ 0 ], Lx );
	Calc_L( xi[ 1 ], Ly );
	Calc_L( xi[ 2 ], Lz );

    unsigned k,l,m,ind=0,dgN = dg + 1;

	N.SetDim( 64 );

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
            for(m=0; m<dgN; m++)
				N[ ind++ ] = Lx[ m ] * Ly[ l ] * Lz[ k ];

}

void CSpaceShapeLagrange4N_3D_64::jacobian( const mvector &xi, matrix &dN ) const
{
	double dLx[ 4 ][ 4 ], dLy[ 4 ][ 4 ], dLz[ 4 ][ 4 ];

	Calc_dL( xi[ 0 ], dLx );
	Calc_dL( xi[ 1 ], dLy );
	Calc_dL( xi[ 2 ], dLz );

    unsigned k, l, m, ind = 0, dgN = dg + 1;

	dN.SetDim( 64, 3 );

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
            for(m=0; m<dgN; m++)
				dN( ind++, 0 ) = dLx[ m ][ 1 ] * dLy[ l ][ 0 ] * dLz[ k ][ 0 ];

	ind = 0;

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
            for(m=0; m<dgN; m++)
				dN( ind++, 1 ) = dLx[ m ][ 0 ] * dLy[ l ][ 1 ] * dLz[ k ][ 0 ];

	ind = 0;

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
            for(m=0; m<dgN; m++)
				dN( ind++, 2 ) = dLx[ m ][ 0 ] * dLy[ l ][ 0 ] * dLz[ k ][ 1 ];
}

