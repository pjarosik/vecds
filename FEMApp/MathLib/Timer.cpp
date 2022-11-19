// Timer.cpp: implementation of the CTimer class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "Timer.h"
#include <cstdio>
#include <cstring>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

string CTimer::FormatTime(double tm) const
{
	int    hours, minutes;
	double seconds = tm;
	hours    = (int)(seconds  /  3600);
	seconds -= hours    *  3600;
	minutes  = (int)(seconds  /   60);
	seconds -= minutes  *    60;
	char resp[50], buf[10];

	sprintf( resp,  "%02d",   hours  );
    strcat(  resp,  ":"              );

	sprintf(  buf,  "%02d", minutes  );
	strcat(  resp,  buf              );

	strcat(  resp,  ":"              );
	sprintf(  buf,  "%02d", (int)seconds );
	strcat(  resp,  buf              );

	return resp;
}

bool CTimer::EstimateTime( unsigned k, double &t )
{
	if ( k == 0 || N == 0 ) return false;
	time_t t2; time( &t2 );
	double time = difftime( t2, wt );
	if ( time < display_int ) return false;
	double  tt = difftime( t2, t1 ), dt = double( N ) / double( k );
	t  = tt * ( dt - 1.0 );
	wt = t2;
	return tt > 1e-16;
}
