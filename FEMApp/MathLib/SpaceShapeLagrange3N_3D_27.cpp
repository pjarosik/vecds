
#include "SpaceShapeLagrange3N_3D_27.h"
#include "SpaceShapeLagrange3N_1D_3.h"
#include "SpaceShapeLagrange3N_2D_9.h"

CSpaceShapeLagrange3N_3D_27::CSpaceShapeLagrange3N_3D_27():CSpaceShapeLagrange3N( 3 )
{
    unsigned k;
    for( k = 0; k < 12; k++ )
        edgeshapes.push_back( new CSpaceShapeLagrange3N_1D_3 );

    for( k = 0; k < 6; k++ )
        faceshapes.push_back( new CSpaceShapeLagrange3N_2D_9 );

    CreateInds();
    dg=2;
}

CSpaceShapeLagrange3N_3D_27::~CSpaceShapeLagrange3N_3D_27(void)
{

}


void CSpaceShapeLagrange3N_3D_27::transform( const mvector &xi, mvector &N ) const
{
	double Lx[ 3 ], Ly[ 3 ], Lz[ 3 ];
	
	Calc_L( xi[ 0 ], Lx );
	Calc_L( xi[ 1 ], Ly );
	Calc_L( xi[ 2 ], Lz );

    unsigned k,l,m,ind=0, dgN = dg + 1;

	N.SetDim( 27 );

    for(k=0; k<dgN; k++)
        for(l=0; l<dgN; l++)
            for(m=0; m<dgN; m++)
				N[ ind++ ] = Lx[ m ] * Ly[ l ] * Lz[ k ];

}

void CSpaceShapeLagrange3N_3D_27::jacobian( const mvector &xi, matrix &dN ) const
{
	double dLx[ 3 ][ 4 ], dLy[ 3 ][ 4 ], dLz[ 3 ][ 4 ];

	Calc_dL( xi[ 0 ], dLx );
	Calc_dL( xi[ 1 ], dLy );
	Calc_dL( xi[ 2 ], dLz );

    unsigned k, l, m, ind = 0,dgN = dg + 1;

	dN.SetDim( 27, 3 );

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



