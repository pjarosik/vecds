
#include "SpaceShapeLagrange2N_3D_8.h"
#include "SpaceShapeLagrange2N_1D_2.h"
#include "SpaceShapeLagrange2N_2D_4.h"

CSpaceShapeLagrange2N_3D_8::CSpaceShapeLagrange2N_3D_8():CSpaceShapeLagrange2N( 3 )
{
    unsigned k;
    for( k = 0; k < 12; k++ )
        edgeshapes.push_back( new CSpaceShapeLagrange2N_1D_2 );

    for( k = 0; k < 6; k++ )
        faceshapes.push_back( new CSpaceShapeLagrange2N_2D_4 );

    CreateInds();
    dg=1;
}

CSpaceShapeLagrange2N_3D_8::~CSpaceShapeLagrange2N_3D_8(void)
{
}


void CSpaceShapeLagrange2N_3D_8::transform( const mvector &xi,mvector &N ) const
{
	double Lx[ 2 ], Ly[ 2 ], Lz[ 2 ];
	
	Calc_L( xi[ 0 ], Lx );
	Calc_L( xi[ 1 ], Ly );
	Calc_L( xi[ 2 ], Lz );

    unsigned k,l,m,ind=0, dgN = dg + 1;

	N.SetDim( 8 );

    for(k=0; k<dgN ; k++)
        for(l=0; l<dgN ; l++)
            for(m=0; m<dgN ; m++)
				N[ ind++ ] = Lx[ m ] * Ly[ l ] * Lz[ k ];

}

void CSpaceShapeLagrange2N_3D_8::jacobian( const mvector &xi, matrix &dN ) const
{
	double dLx[ 2 ][ 4 ], dLy[ 2 ][ 4 ], dLz[ 2 ][ 4 ];

	Calc_dL( xi[ 0 ], dLx );
	Calc_dL( xi[ 1 ], dLy );
	Calc_dL( xi[ 2 ], dLz );

    unsigned k, l, m, ind = 0,  dgN = dg + 1;

	dN.SetDim( 8, 3 );

    for(k=0; k<dgN ; k++)
        for(l=0; l<dgN ; l++)
            for(m=0; m<dgN ; m++)
				dN( ind++, 0 ) = dLx[ m ][ 1 ] * dLy[ l ][ 0 ] * dLz[ k ][ 0 ];

	ind = 0;

    for(k=0; k<dgN ; k++)
        for(l=0; l<dgN ; l++)
            for(m=0; m<dgN ; m++)
				dN( ind++, 1 ) = dLx[ m ][ 0 ] * dLy[ l ][ 1 ] * dLz[ k ][ 0 ];

	ind = 0;

    for(k=0; k<dgN ; k++)
        for(l=0; l<dgN ; l++)
            for(m=0; m<dgN ; m++)
				dN( ind++, 2 ) = dLx[ m ][ 0 ] * dLy[ l ][ 0 ] * dLz[ k ][ 1 ];
}


