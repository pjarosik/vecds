
#include "SpaceShapeLagrange2N.h"

CSpaceShapeLagrange2N ::CSpaceShapeLagrange2N(unsigned dim):CSpaceShapeLagrange( dim, 2 )
{
}

CSpaceShapeLagrange2N::~CSpaceShapeLagrange2N(void)
{
}


void  CSpaceShapeLagrange2N::Calc_L( double x, double  (&l)[2] ) const
{
	l[0] = ( 1.0 - x ) / 2.0;
	l[1] = ( 1.0 + x ) / 2.0; 

}

void CSpaceShapeLagrange2N::Calc_dL(  double x, double  (&dl)[2][4] ) const
{

	dl[ 0 ][ 0 ] = ( 1.0 - x ) / 2.0;	
	dl[ 1 ][ 0 ] = ( 1.0 + x ) / 2.0;

	dl[ 0 ][ 1 ] = - 0.5;	
	dl[ 1 ][ 1 ] = 0.5;

	dl[ 0 ][ 2 ] = 0.0;	
	dl[ 1 ][ 2 ] = 0.0;			

	dl[ 0 ][ 3 ] = 0.0;	
	dl[ 1 ][ 3 ] = 0.0;				
}
