#include "PostProcessingItemTextOutputOfstream.h"

CPostProcessingItemTextOutputOfstream::CPostProcessingItemTextOutputOfstream(const OutputDef &od ):CPostProcessingItemTextOutput(od )
{
    ofstream fout( (OD.name + ".txt").c_str() );
}


bool CPostProcessingItemTextOutputOfstream::userProcessing( CFE_Equation *eq, bool IsBegin )
{
    ofstream fout( (OD.name + ".txt").c_str(), ios::app );
    if ( !fout.is_open() ) throw CFEException( "Can't open output file \'" + OD.name + "\'" );
    FlushOutput( fout, eq, IsBegin );
    return true;
}
