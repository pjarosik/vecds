#include "PostProcessItem.h"
#include <algorithm>
#include "FE_Equation.h"

CPostProcessingItem::CPostProcessingItem( const vector< string > &st ):stages( st )
{
    if ( stages.size() == 0 ) stages.push_back("Analysis");
}


bool CPostProcessingItem::process( CFE_Equation *eq, const string &stage, bool IsBegin )
{
    if ( stage == "now" ) return userProcessing( eq, IsBegin );
    vector<string>::const_iterator i = find( stages.begin(), stages.end(), stage );
    if ( i == stages.end() ) return false;
    currentstage = stage;
    return userProcessing( eq, IsBegin );
}
