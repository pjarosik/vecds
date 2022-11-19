
#include "SpaceShapeLagrange4N.h"

CSpaceShapeLagrange4N::CSpaceShapeLagrange4N(unsigned dim):CSpaceShapeLagrange( dim, 4 )
{
}

CSpaceShapeLagrange4N::~CSpaceShapeLagrange4N(void)
{
}
/*

			L1x = -(xi[0]-1.0)*(3.0*xi[0]-1.0)*(3.0*xi[0]+1.0)/16.0,
			L2x = 9.0*(xi[0]-1.0)*(xi[0]+1.0)*(3.0*xi[0]-1.0)/16.0,
			L3x = -9.0*(xi[0]-1.0)*(xi[0]+1.0)*(3.0*xi[0]+1.0)/16.0,
			L4x = (xi[0]+1.0)*(3.0*xi[0]-1.0)*(3.0*xi[0]+1.0)/16.0;
*/

void  CSpaceShapeLagrange4N::Calc_L( double x, double  (&l)[4] ) const
{
	l[0] = -(x-1.0)*(3.0*x-1.0)*(3.0*x+1.0)/16.0;
	l[1] = 9.0*(x-1.0)*(x+1.0)*(3.0*x-1.0)/16.0; 
	l[2] = -9.0*(x-1.0)*(x+1.0)*(3.0*x+1.0)/16.0; 
	l[3] = (x+1.0)*(3.0*x-1.0)*(3.0*x+1.0)/16.0; 
}

void CSpaceShapeLagrange4N::Calc_dL(  double x, double  (&dl)[4][4] ) const
{

	dl[ 0 ][ 0 ] = -(x-1.0)*(3.0*x-1.0)*(3.0*x+1.0)/16.0;
	dl[ 1 ][ 0 ] = 9.0*(x-1.0)*(x+1.0)*(3.0*x-1.0)/16.0; 
	dl[ 2 ][ 0 ] = -9.0*(x-1.0)*(x+1.0)*(3.0*x+1.0)/16.0; 
	dl[ 3 ][ 0 ] = (x+1.0)*(3.0*x-1.0)*(3.0*x+1.0)/16.0; 

	dl[ 0 ][ 1 ] = -(27.0*x*x-18.0*x-1.0)/16.0;	
	dl[ 1 ][ 1 ] = 9.0*(9.0*x*x-2.0*x-3.0)/16.0;
	dl[ 2 ][ 1 ] = -9.0*(9.0*x*x+2.0*x-3.0)/16.0;	
	dl[ 3 ][ 1 ] = (27.0*x*x+18.0*x-1.0)/16.0; 

	dl[ 0 ][ 2 ] = -(54.0*x-18.0)/16.0;	;	
	dl[ 1 ][ 2 ] = 9.0*(18.0*x-2.0)/16.0;;			
	dl[ 2 ][ 2 ] = -9.0*(18.0*x+2.0)/16.0;	;	
	dl[ 3 ][ 2 ] = (54.0*x+18.0)/16.0; 			

	dl[ 0 ][ 3 ] = -54.0/16.0;	
	dl[ 1 ][ 3 ] = 162.0/16.0;			
	dl[ 2 ][ 3 ] = -162.0/16.0;	
	dl[ 3 ][ 3 ] = 54.0/16.0;			
}
