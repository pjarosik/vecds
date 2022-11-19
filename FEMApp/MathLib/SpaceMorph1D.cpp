#include "SpaceMorph1D.h"
#include "MathException.h"

CSpaceMorph1D::CSpaceMorph1D( const CSpace *s1, const CSpace *s2, CSpace *s ):CSpace( s1->GetXdim(), s1->GetXdim() + 1, s ), S1( s1 ), S2( s2 )
{
    if ( s1->Getxdim() != s2->Getxdim() )
        throw CMathException( "SpaceMorph1D::CSpaceMorph1D( const CSpace *s1, const CSpace *s2 ) - diferent dimension of spaces s1 and s2" );

    if ( s1->GetXdim() != s2->GetXdim() )
        throw CMathException( "SpaceMorph1D::CSpaceMorph1D( const CSpace *s1, const CSpace *s2 ) - diferent dimension of spaces s1 and s2" );
}


CSpaceMorph1D::~CSpaceMorph1D(void)
{

}

void CSpaceMorph1D::transform( const mvector &x,  mvector &X ) const
{

    mvector xs ( x.GetDim() - 1 ), X1, X2 ;
    double xm = x[ xdim - 1 ];
    double alpha = 0.5 * ( 1.0 - xm ), beta = 0.5 * ( xm - 1.0 );

    unsigned k;
    for ( k = 0; k < xdim - 1; k++) xs[ k ] = x[ k ];

    S1->Transform( xs, X1 );
    S2->Transform( xs, X2 );

    X = alpha * X1 +  beta * X2 ;

}

void CSpaceMorph1D::jacobian( const mvector &x,  matrix &j ) const
{
    mvector xs ( x.GetDim() - 1 ), X1, X2 ;
    double xm = x[ xdim - 1 ];
    double alpha = 0.5 * ( 1.0 - xm ), beta = 0.5 * ( xm - 1.0 ), dalpha = -0.5, dbeta = 0.5;

    matrix j1, j2;

    S1->jacobian( xs, j1 );
    S2->jacobian( xs, j2 );

    S1->transform( xs, X1 );
    S2->transform( xs, X2 );

    j.SetDim( Xdim ,Xdim );

    unsigned k, l;
    for (k=0; k<Xdim-1; k++)
        for (l=0; l<Xdim; l++)
            j(k,l) = alpha * j1(k,l) + beta * j2(k,l);

    for (l=0; l<Xdim; l++) j(Xdim-1,l) = dalpha * X1[l] + dbeta * X2[l];
}
