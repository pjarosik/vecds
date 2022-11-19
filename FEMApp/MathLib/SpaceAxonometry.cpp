
#include "SpaceAxonometry.h"

CSpaceAxonometry::CSpaceAxonometry(void):CSpace(3,2)
{
}

CSpaceAxonometry::~CSpaceAxonometry( void )
{
}

void CSpaceAxonometry::transform( const mvector &x,  mvector &X )const
{
	 X[0] = x[0] + x[1] / 2.0; 
	 X[1] = x[2] + x[1] / 2.0; 
}

void CSpaceAxonometry::jacobian( const mvector &x,  matrix &j )const
{

}
