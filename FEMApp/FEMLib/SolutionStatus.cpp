#include "SolutionStatus.h"

CSolutionStatus::CSolutionStatus():t(0.0),dt(0.0)
{
}


string CSolutionStatus::GetFormattedTime( const string &tmn ) const
{
    map<string,CTimer>::const_iterator i = timers.find( tmn );
    if ( i == timers.end() )
    {
        return "";
    }
    return (*i).second.GetFormattedTime();
}
