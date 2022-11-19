
#include "SpaceLine.h"
#include "../MathLib/MathException.h"

CSpaceLine::CSpaceLine( const mvector &a, const mvector &b, CSpace *sp ):CSpace( 1, a.GetDim(), sp ),X1( a ), X2( b )
{
    if ( a.GetDim() != b.GetDim() )
        throw CMathException( "CSpaceLine::CSpaceLine( const mvector &a, const mvector &b ) - diferent dimension of mvectors a b" );
}


CSpaceLine::~CSpaceLine(void)
{

}

void CSpaceLine::transform( const mvector &x,  mvector &X ) const
{
    X = 0.5 *( x[0] * ( X2 - X1 ) + X2 + X1 );
}

void CSpaceLine::jacobian( const mvector &x,  matrix &j ) const
{
    j = 0.5 *( X2 - X1 );
}
