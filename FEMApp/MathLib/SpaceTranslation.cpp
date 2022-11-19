#include "SpaceTranslation.h"


CSpaceTranslation::CSpaceTranslation( const mvector &x, CSpace *sp ):CSpace( x.GetDim(), x.GetDim(), sp ),dx( x )
{

}

CSpaceTranslation::~CSpaceTranslation( void )
{

}


void CSpaceTranslation::transform( const mvector &x,  mvector &X )const
{
    X = x;
    X += dx;
}

void CSpaceTranslation::jacobian( const mvector &x,  matrix &j )const
{
    unsigned k;
    j.SetDim( dx.GetDim( ), dx.GetDim( ) );
    for ( k = 0; k < dx.GetDim(); k++ ) j( 0, 0 ) = 1.0;

}
