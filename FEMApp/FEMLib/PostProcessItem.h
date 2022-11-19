#ifndef POSTPROCESSITEM_H
#define POSTPROCESSITEM_H

#include <string>
#include <vector>

using namespace std;

class CFE_Equation;
class CPostProcessingItem
{

public:

    CPostProcessingItem( const vector< string > &st );
    bool process( CFE_Equation *eq, const string &stage, bool IsBegin = false ) ;

protected:

    virtual bool userProcessing( CFE_Equation *eq, bool IsBegin = false )=0;
    vector< string > stages;
    string currentstage;
};

#endif // POSTPROCESSITEM_H
