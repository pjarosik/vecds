
#include "SpaceSpherical.h"

CSpaceSpherical::CSpaceSpherical( ):CSpace( 3, 3 )
{

}

CSpaceSpherical::~CSpaceSpherical(void)
{
}


void CSpaceSpherical::transform( const mvector &x,  mvector &X )const
{
    double r=x[0], phi=x[1], theta=x[ 2 ];
    X.SetDim(3);
	X[ 0 ] = r * cos(phi) * sin(theta); 
	X[ 1 ] = r * sin(phi) * sin(theta); 
	X[ 2 ] = r * cos(theta); 
}

void CSpaceSpherical::jacobian( const mvector &x,  matrix &j )const
{
	double r = x[ 0 ], phi = x[ 1 ], theta = x[ 2 ];
    j.SetDim(3,3);

	j(0,0) = cos(phi)*sin(phi);		j(0,1) = sin(phi)*sin(theta);		j(0,2) = cos(theta);
	j(1,0) = r*cos(phi)*cos(theta);	j(1,1) = cos(phi)*sin(theta);		j(1,2) = 0.0;
	j(2,0) = -r*sin(phi)*sin(theta);	j(2,1) = r*sin(phi)*cos(theta);	j(2,2) = - r * sin(theta) ;
}
