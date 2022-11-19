
#include "Curve.h"

CCurve::CCurve(double a,double b):t1(a),t2(b),ndiv(100)
{
}

CCurve::~CCurve(void)
{
}

void CCurve::linearize( vector< mvector > &lc )
{
	unsigned k;
	double dt = ( t2 - t1 ) / (double)ndiv;
	mvector x(2);
	for (k=0; k<=ndiv; k++)
	{
		XYZ( t1 + k * dt, x );
		lc.push_back( x );
	}
}

bool CCurve::Intersection( const CCurve &c, mvector &xi )
{
	
    double norm = 1.0, dist;
    
	xi.SetDim( 2 );
	xi[ 0 ] = Xs();
	xi[ 1 ] = c.Xs();

	mvector X0(2), X1(2), dX(2), dy(2), dX0(2), dX1(2), dxi(2);

	XYZ( xi[ 0 ], X0 );
	c.XYZ( xi[ 1 ], X1 );
	dX = X1 - X0;
	dist = dX.GetNorm();

    while( dist > 0.000001 )
    {
		XYZ( xi[ 0 ], X0 );
		c.XYZ( xi[ 1 ], X1 );

		dX = X1 - X0;
		dist = dX.GetNorm() * dX.GetNorm() ;

		dXYZ( xi[ 0 ], dX0 );
		c.dXYZ( xi[ 1 ], dX1 );

		dy[0] = 2.0 * ( X1[ 0 ] - X0[ 0 ] ) * (    dX1[0] );
		dy[1] = 2.0 * ( X1[ 1 ] - X0[ 1 ] ) * (  - dX0[1] );
		
		dxi = dist * dy;
        xi += dxi;
        norm = dxi.GetNorm();
	
    }

	return true;
}