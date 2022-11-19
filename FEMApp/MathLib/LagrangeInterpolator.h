#pragma once

#include <vector>
using namespace std;

class CLagrangeInterpolator
{
public:
	CLagrangeInterpolator(const vector<double> &a1,const vector<double> &y1);
	CLagrangeInterpolator(const CLagrangeInterpolator &li):a(li.a),y(li.y),ma(li.ma) { }
	const CLagrangeInterpolator& operator=(const CLagrangeInterpolator &li) { a = li.a;  y = li.y;  ma = li.ma; return *this; }
	~CLagrangeInterpolator(void);
	double operator()(double x);
protected:
	vector<double> a, y, ma;
};

inline double CLagrangeInterpolator::operator()(double x)
{
	unsigned n = (unsigned)a.size(), k;
	/*double sa=1.0, v=0.0;
	unsigned n = (unsigned)a.size(), k;
	for( k=0; k<n; k++) sa *= x - a[ k ];
	for( k=0; k<n; k++)  v += sa * y[k] / ( x - a[ k ] ) / ma[ k ]; */
	double m,v=0.0;
	for( k=0; k < n; k++ )
		if ( a[ k ] > x )
		{
			m = ( y[k] - y[k-1] ) / ( a[k] - a[k-1] );
			return y[ k - 1 ] + m * ( x - a[ k - 1 ] );
		}
	return v;
}
