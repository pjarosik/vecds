
#include "SpaceExtend2D3D.h"

CSpaceExtend2D3D::CSpaceExtend2D3D( unsigned a, double v ):CSpace(2,3),axis( a ),value( v )
{
	switch( a )
	{
		case 0 : ox=1; oy=2; break;
		case 1 : ox=0; oy=2; break;
		case 2 : ox=0; oy=1; break;
		default : assert(0);
	}
}

CSpaceExtend2D3D::~CSpaceExtend2D3D(void)
{
}


void CSpaceExtend2D3D::transform( const mvector &x,  mvector &X )const
{
	X[ ox ]   = x[ 0 ];	
	X[ oy ]	  = x[ 1 ];
	X[ axis ] = value;
}

void CSpaceExtend2D3D::jacobian( const mvector &x,  matrix &j )const
{

}
