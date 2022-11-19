
#include "ValueIndicator.h"



void CValueIndicator::Paint( CGraphDevice &gDV, double cc )
{
	double v,mlen = fabs( x2 - x1 ) / 30.0, wd=GetWidth( gDV );
    gDV.Line2D( cc + wd - mlen, x1, cc + wd - mlen, x2 );
	unsigned counter=0;
	for( v=Vmin; v<Vmax+unit; v += unit)
	{
		double len = mlen / 4.0,vv;
		int iv = (int)(floor( v / unit + 0.5 ) );
		vv = (double)iv * unit;
		if ( counter == 5 ) len = mlen / 2.0;
		if ( !counter || counter==10) 
		{ 
			len = mlen; 
			counter=0; 
			string s = ToString( vv );
//			gDV.Write( cc, sc.scale( vv ) - gDV.GetFontHeight() / 2.0, s.c_str() );
		}
		counter++;

	}

}

double CValueIndicator::GetWidth( CGraphDevice &gDV )
{
	double len = 0.0, v, unit = dist_mag / 10;
	for( v=Vmin; v<Vmax+unit; v += unit)
	{
		double vv;
		int iv = (int)(floor( v / unit + 0.5 ) );
		vv = (double)iv * unit;
		string s = ToString( vv );
//		vv = gDV.GetTextWidth( s.c_str( ) );
		if ( vv > len ) len = vv;
	}
	return len + 1.2 * fabs( x2 - x1 ) / 30.0 ;
}

string CValueIndicator::ToString( double v )
{
	stringstream ss;	
	if (is_scientific ) ss.setf( ios::scientific );
	ss.precision( prec );
	ss << v;
	return ss.str();
}
