
#include "SpaceProjection.h"


CSpaceProjection::CSpaceProjection(unsigned plane):CSpace(3,2),ox(0),oy(0)
{
	switch( plane )
	{
		case 0 : ox=1; oy=2; break;
		case 1 : ox=0; oy=2; break;
		case 2 : ox=0; oy=1; break;
		default : assert(0);
	}
}


CSpaceProjection::~CSpaceProjection(void)
{

}



void CSpaceProjection::transform( const mvector &x,  mvector &X )const
{
	X[ 0 ] = x[ ox ];	
	X[ 1 ] = x[ oy ];
}

void CSpaceProjection::jacobian( const mvector &x,  matrix &j )const
{
    j.SetDim(2,3);
    j(0,ox)=1.0;
    j(1,oy)=1.0;
}
