
#include "LagrangeInterpolator.h"

CLagrangeInterpolator::CLagrangeInterpolator( const vector<double> &a1, const vector<double> &y1 ):a( a1 ),y( y1 )
{
	unsigned k,l;
	double wa;
	for (k=0; k<a.size(); k++)
	{
		wa=1.0;
		for (l=0; l<a.size(); l++)	if ( k != l ) wa *= ( a[k] - a[l] );
		ma.push_back( wa );
	}
}

CLagrangeInterpolator::~CLagrangeInterpolator(void)
{
}
