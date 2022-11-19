#ifndef SOLUTIONSTATUS_H
#define SOLUTIONSTATUS_H
#include <map>
#include "../MathLib/Timer.h"
#include "FEException.h"

class CSolutionStatus
{
public:
    CSolutionStatus();
    void SetTime( double tm ) { t = tm; }
    void SetDt( double dtm ) { dt = dtm; }
    void SetTimer( const string &tmn ) { timers[ tmn ] = CTimer(); }
    bool GetTimer( const string &tmn, CTimer &rtm, bool must_exists = true );
    string GetFormattedTime( const string &tmn ) const;

    double GetT() const { return t; }
    double GetdT() const { return dt; }

protected:
    double t, dt;
    map< string, CTimer > timers;
};


inline bool CSolutionStatus::GetTimer(const string &tmn, CTimer &rtm, bool must_exists )
{
     map<string,CTimer>::iterator i = timers.find(tmn);
     if ( i == timers.end() )
     {
         if ( must_exists ) throw CFEException( "CSolutionStatus::GetTimer - unknown timer :" + tmn );
         return false;
     }
     rtm = (*i).second;
     return true;
}

#endif // SOLUTIONSTATUS_H
