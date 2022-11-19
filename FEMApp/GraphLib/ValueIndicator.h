#pragma once

#include "../MathLib/Scaler.h"
#include "GraphDevice.h"
#include <cmath>

/*
	1
	|_______

*/

class CValueIndicator
{
public:

	CValueIndicator( double mn, double mx, double c1, double c2, unsigned p=3,bool iv = true, bool l=true, bool s=false )
		                               :is_vertical(iv), is_left(l), is_scientific(s), sc(Vmin,Vmax,0,1), prec( p ),
								 x1(c1), x2(c2)
	{

		double Vdist = mx - mn;
	//	dist_base = ( Vdist / pow( 10.0,  Exp10( Vdist ) ) );
		dist_mag = pow( 10.0,  Exp10( Vdist ) );
	//	dist_exp = Exp10( Vdist );
		
		unit = dist_mag / 10;
	 	Vmin =  ( floor( mn / dist_mag ) ) * dist_mag ;
		Vmax =  ( ceil(  mx / dist_mag ) ) * dist_mag ;

		sc = CScaler( Vmin, Vmax, c1, c2 );

		if ( fabs( mn - Vmin ) / Vdist < 0.25 && fabs( Vmax - mx ) / Vdist < 0.25 ) return;

		dist_mag = pow( 10.0,  Exp10( Vdist ) - 1 );
	//	dist_exp = Exp10( Vdist );
		
		unit = dist_mag / 10;
	 	Vmin =  ( floor( mn / dist_mag ) ) * dist_mag ;
		Vmax =  ( ceil(  mx / dist_mag ) ) * dist_mag ;

		sc = CScaler( Vmin, Vmax, c1, c2 );

	}

	~CValueIndicator( void )
	{

	}

	void Paint( CGraphDevice &gDV, double cc  );

	double GetMin() { return Vmin; }
	double GetMax() { return Vmax; }
	double GetWidth( CGraphDevice &gDV );
	string ToString( double v );

protected:

	int Exp10( double x ) { return (int)( fabs( x ) > 1.0 ? log10( fabs( x ) ) : x == 0.0 ? 0.0 : log10( fabs( x ) ) - 1); }
	double sgn( double x ) { return x > 0.0 ? 1.0 : x < 0.0 ? -1.0 : 0.0;  }
	   int sgn(    int x ) { return x > 0 ? 1 : x < 0 ? -1 : 0 ;           }

//	double 

	CScaler sc;
//	double Vmax, Vmin, dist_base, dist_mag, dist_exp,x1,x2,unit;
	double Vmax, Vmin,  dist_mag, x1,x2,unit;
	bool   is_vertical, is_left, is_scientific;
	unsigned prec;

};
