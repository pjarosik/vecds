// Timer.h: interface for the CTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMER_H__0AA91DC2_7216_11D5_8C4F_F9627B1E4260__INCLUDED_)
#define AFX_TIMER_H__0AA91DC2_7216_11D5_8C4F_F9627B1E4260__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>
#include <string>	// Added by ClassView
using namespace std;

class CTimer
{
    time_t t1,wt;
	clock_t ct1,cdiff;
    double  duration, display_int;
	unsigned N;
	void   calc_duration()           { }
public:

	CTimer(double di=20.0):t1(0),wt(0),duration(0.0),display_int(di),N(0) { }
    CTimer( const CTimer& t )                { t1 = t.t1;  duration = t.duration; }
	~CTimer() { }

	string GetFormattedTime() const  { return FormatTime( duration ); }
	double GetDuration()      const  { return duration; }
	double GetDurationWithMiliseconds()      const  { return (double)cdiff / (double)CLOCKS_PER_SEC; }
	double PreviewTime()      const  { time_t t2; time( &t2 ); return difftime( t2, t1 ); }
	string PreviewTimeFormatted() const  { return FormatTime( PreviewTime( ) ); }
	void Stop()                          {  time_t t2; time( &t2 );    duration = difftime( t2, t1 );   cdiff=clock()-ct1;  }
	bool EstimateTime( unsigned k, double &t );
	void Start(unsigned n=0)             { time( &t1 );  N=n; ct1=clock(); }
	void operator=( const CTimer &t )    { t1 = t.t1; duration = t.duration;  }
	string FormatTime( double tm ) const;
     static void wait( clock_t pr )
     {
          clock_t atime;
          atime = pr + clock();
          while( atime > clock() );
     }

private:

};

#endif // !defined(AFX_TIMER_H__0AA91DC2_7216_11D5_8C4F_F9627B1E4260__INCLUDED_)
