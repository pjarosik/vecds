#ifndef POSTPROCESS_H
#define POSTPROCESS_H
#include "FEA_Stages.h"
#include "../MathLib/Timer.h"
#include "PostProcessItem.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

enum Alignment
{
    AL_LEFT,
    AL_CENTER,
    AL_RIGHT
};



class CPostProcessing
{

public:

    CPostProcessing();
    virtual void BeginStage( CFE_Equation *eq, const string &stage );
    virtual void EndStage( CFE_Equation *eq );
    void RegisterStage( const string &stg );
    void RegisterStages( const vector< string > &vstg ) { for( int k=0; k<vstg.size(); k++) RegisterStage( vstg[k] ); }
    const string& GetCurrentStage() const { return currentstage; }
    void AddItem( CPostProcessingItem *it ) { items.push_back( it ); }

protected:

    CTimer* findTimer( const string &st );

    vector< string > stages, stagestack;
    map< string, CTimer > timers;
    string currentstage;
    vector< CPostProcessingItem* > items;

};



inline CTimer* CPostProcessing::findTimer( const string &st )
{
    map< string, CTimer >::iterator i = timers.find( st );
    if ( i == timers.end() ) return 0;
    return &(*i).second;
}


#endif // POSTPROCESS_H
