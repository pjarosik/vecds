#pragma once

#include <vector>
#include <assert.h>
#include <cmath>
using namespace std;

class SVM
{
public:
	SVM( void );

	static double DotProduct( const vector<double> &a, const vector<double> &b )
	{
		unsigned k;
		double s=0.0;
		assert( a.size( ) == b.size( ) );
		for ( k = 0; k < (unsigned)a.size(); k++ ) s += a[ k ] * b[ k ];
		return s;
	}

	static double Norm( const vector<double> &a )
	{
		double r=0.0;
		for (unsigned k=0; k<(unsigned)a.size(); k++) r += a[ k ] * a[ k ];
		return sqrt( r );
	}

	static double Angle( const vector<double> &a, const vector<double> &b )
	{
		return acos ( DotProduct( a , b ) / ( Norm( a ) * Norm( b ) ) );
	}

	static double Distance( const vector<double> &a, const vector<double> &b, unsigned n=0 )
	{
		double r=0.0,dx;
		for (unsigned k=0; k<(unsigned)a.size()-n; k++) 
		{
			dx = a[ k ] - b[ k ];
			r += dx * dx;
		}
		return sqrt( r );
	}

	static void Diff( const vector<double> &a, const vector<double> &b, vector<double> &diff )
	{
		assert( (a.size( ) == b.size( ) ) && ( b.size( ) == diff.size() ) );
		for (unsigned k=0; k<(unsigned)a.size(); k++) diff[k] = a[ k ] - b[ k ];
	}

	static void Sum( const vector<double> &a, const vector<double> &b, vector<double> &sum )
	{
		assert( (a.size( ) == b.size( )) && ( b.size( ) == sum.size() ) );
		for (unsigned k=0; k<(unsigned)a.size(); k++) sum[k] = a[ k ] + b[ k ];
	}

	static void Mult( double v, vector<double> &a )
	{
		for (unsigned k=0; k<(unsigned)a.size(); k++) a[ k ] = v * a[ k ];
	}

	static void ToUnit( vector<double> &a )
	{
		 Mult( 1.0 / Norm( a ), a );
	}

public:
	~SVM(void);
};
