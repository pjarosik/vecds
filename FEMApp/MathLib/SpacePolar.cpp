
#include "SpacePolar.h"

CSpacePolar::CSpacePolar(const mvector &x, double f, CSpace *sp ):CSpace(2,2,sp), xs( x ), fi0( f )
{
    dg = 10;
}

CSpacePolar::~CSpacePolar(void)
{
}

void CSpacePolar::transform( const mvector &x,  mvector &X )const
{
    X.SetDim(2);
    X[ 0 ] = xs[0] + x[ 0 ] * cos( fi0 + x[ 1 ] );
    X[ 1 ] = xs[1] + x[ 0 ] * sin( fi0 + x[ 1 ] );
}

void CSpacePolar::jacobian( const mvector &x, matrix &j )const
{
	j.SetDim(2,2);
	double r = x[ 0 ],  phi = x[ 1 ];
    j(0,0) =  cos(fi0+phi);		j(0,1) = sin(fi0+phi);
	j(1,0) = -r*sin(fi0+phi);	j(1,1) = r*cos(fi0+phi);
}
