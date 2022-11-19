
#include "SpaceRotation3D.h"

CSpaceRotation3D::CSpaceRotation3D( double t1, double t2, double t3 ):CSpace( 3, 3 )
{
	Init( t1, t2, t3 );

}


CSpaceRotation3D::~CSpaceRotation3D( void ) {  }


void CSpaceRotation3D::Init( double t1, double t2, double t3 )
{
	matrix Rx(3,3), Ry(3,3), Rz(3,3);

	rx = t1 * M_PI / 180.0,
	ry = t2 * M_PI / 180.0,
    rz = t3 * M_PI / 180.0;


	Rx(0,0) = 1.0;
	Rx(1,1) =  cos(rx); Rx(1,2) = sin(rx);
	Rx(2,1) = -sin(rx); Rx(2,2) = cos(rx);

	Ry(0,0) = cos(ry); Ry(0,2) = -sin(ry);
	Ry(1,1) = 1.0;
	Ry(2,0) = sin(ry); Ry(2,2) = cos(ry);

	Rz(0,0) =  cos(rz); Rz(0,1) = sin(rz);
	Rz(1,0) = -sin(rz); Rz(1,1) = cos(rz);
	Rz(2,2) = 1.0;


	R = Rz * Ry * Rx;
}

void CSpaceRotation3D::transform( const mvector &x,  mvector &X ) const
{
    X = R *  x;
}


 void CSpaceRotation3D::jacobian( const mvector &x,  matrix &j ) const
{
	j = R;
}
