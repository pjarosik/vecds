
#include "SpacePerspective.h"

CSpacePerspective::CSpacePerspective(double d, double h, double xp):CSpace(3,2), deepth(d), height(h), xpoz(xp)
{
}


CSpacePerspective::~CSpacePerspective(void)
{
}

void CSpacePerspective::transform( const mvector &x,  mvector &X )const
{
	 X[ 0 ] = ( deepth * x[ 0 ] + xpoz   * x[ 1 ] ) / ( x[ 1 ] + deepth ); 
	 X[ 1 ] = ( height * x[ 1 ] + deepth * x[ 2 ] ) / ( x[ 1 ] + deepth );
}

void CSpacePerspective::jacobian( const mvector &x,  matrix &j ) const
{

}
