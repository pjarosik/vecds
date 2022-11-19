
#define _USE_MATH_DEFINES
#include <cmath>

#include "CrackLine.h"

CCrackLine::CCrackLine( const mvector &a1, const mvector &a2 ):CCrack(),x1(a1), x2(a2)
{
}

CCrackLine::~CCrackLine(void)
{
}


bool CCrackLine::XYZ( double t, mvector &X ) const
{
	X = 0.5 *(  t * ( x2 - x1 ) + x2 + x1 );
	if ( t < t1*0.99 || t > t2*1.01) return false;
	return true;
}

bool CCrackLine::dXYZ( double t, mvector dX ) const
{
	dX = 0.5 *( x2 - x1 );
	if ( t < t1*0.99 || t > t2*1.01) return false;
	return true;
}

double CCrackLine::angle( double t ) const
{
	double dx = x2[0] - x1[0],
		   dy = x2[1] - x1[1], len = sqrt(dx*dx+dy*dy);

	double angle = asin( dy / len );
	if ( dx < 0.0 ) angle = M_PI - angle;
//	double degree = angle * 180.0 / M_PI;

	return angle;
}

double CCrackLine::distance( const mvector &x ) const
{
    double dx = x2[0]-x1[0], dy = x2[1]-x1[1], A = -dy, B = dx, C = x1[0]*dy-x1[1]*dx;
	return (A*x[0]+B*x[1]+C) / sqrt(A*A+B*B);
}

bool CCrackLine::IsNear( const mvector &x, double eps) const
{
	double dist = distance(x);
	if ( fabs(dist) > eps ) return false;
	double ax = 0.5 * ( x2[0] - x1[0] ), bx = 0.5 * (x2[0] + x1[0]);
	double t = ( x[0] - bx ) / ax;

	if ( t < t1-eps || t > t2 + eps ) return false;

	return true;

}
