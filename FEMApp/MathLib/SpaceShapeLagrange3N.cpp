
#include "SpaceShapeLagrange3N.h"

CSpaceShapeLagrange3N::CSpaceShapeLagrange3N(unsigned dim):CSpaceShapeLagrange( dim, 3 )
{
}

CSpaceShapeLagrange3N::~CSpaceShapeLagrange3N(void)
{
}

void  CSpaceShapeLagrange3N::Calc_L( double x, double  (&l)[3] ) const
{

	l[ 0 ] = x * ( x - 1.0 ) / 2.0;
	l[ 1 ] = ( 1.0 - x * x ); 
	l[ 2 ] = x * ( x + 1.0 ) / 2.0; 

}

void CSpaceShapeLagrange3N::Calc_dL(  double x, double  (&dl)[3][4] ) const
{

	dl[ 0 ][ 0 ] = x * ( x - 1.0 ) / 2.0;	
	dl[ 1 ][ 0 ] = ( 1.0 - x * x );
	dl[ 2 ][ 0 ] = x * ( x + 1.0 ) / 2.0;

	dl[ 0 ][ 1 ] = x - 0.5;	
	dl[ 1 ][ 1 ] = - 2.0 * x;
	dl[ 2 ][ 1 ] = x + 0.5;

	dl[ 0 ][ 2 ] = 1.0;	
	dl[ 1 ][ 2 ] = 2.0;
	dl[ 2 ][ 2 ] = 1.0;

	dl[ 0 ][ 3 ] = 0.0;	
	dl[ 1 ][ 3 ] = 0.0;			
	dl[ 2 ][ 3 ] = 0.0;

}
