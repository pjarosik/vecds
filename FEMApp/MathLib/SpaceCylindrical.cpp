
#include "SpaceCylindrical.h"

CSpaceCylindrical::CSpaceCylindrical( CSpace *sp ):CSpace(3,3,sp)
{
    dg = 10;
}

CSpaceCylindrical::~CSpaceCylindrical( void )
{

}



void CSpaceCylindrical::transform( const mvector &x,  mvector &X )const
{
	X[0] = x[0] * cos( x[1] );
	X[1] = x[0] * sin( x[1] );
	X[2] = x[2];
}

void CSpaceCylindrical::jacobian( const mvector &x,  matrix &j )const
{
    double r = x[ 0 ], phi = x[ 1 ];

	j(0,0) = cos(phi);		j(0,1) = sin(phi);		j(0,2) = 0.0;
	j(1,0) = -r*sin(phi);	j(1,1) = r*cos(phi);	j(1,2) = 0.0;
	j(2,0) = 0.0;			j(2,1) = 0.0;			j(2,2) = 1.0;
}
