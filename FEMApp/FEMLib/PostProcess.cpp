#include "PostProcess.h"
#include "FEException.h"
#include <algorithm>
#include <iostream>

CPostProcessing::CPostProcessing():currentstage("root")
{
}


void CPostProcessing::BeginStage( CFE_Equation *eq, const string &nstg )
{
     vector<string>::const_iterator s = find( stages.begin(), stages.end(), nstg );
     if ( s == stages.end() ) throw CFEException( "CPostProcessing::BeginStage : stage named \'" + nstg + "\' not registered" );
     stagestack.push_back( currentstage );
     currentstage = nstg;
     vector< CPostProcessingItem* >::iterator i;
     for (i=items.begin(); i!=items.end(); i++)
         (*i)->process( eq, currentstage, true );
     CTimer *ctimer = findTimer(currentstage);
     ctimer->Start();
}

void CPostProcessing::EndStage( CFE_Equation *eq )
{
    if ( currentstage == "root" ) throw CFEException("CPostProcessing::EndStage - can't end stage - root stage reached");
    // cout << "finished stage :" << currentstage << ". Computation time :";
    vector< CPostProcessingItem* >::iterator i;
    for (i=items.begin(); i!=items.end(); i++)
        (*i)->process( eq, currentstage );
    currentstage = stagestack.back();
    stagestack.pop_back();
    CTimer *ctimer = findTimer( currentstage );
    if (ctimer)
    {
        ctimer->Stop();
        // cout << ctimer->GetFormattedTime() << endl;
    }

}

void CPostProcessing::RegisterStage( const string &nstg )
 {
    if ( nstg.size() == 0 ) throw CFEException( "CPostProcessing::RegisterStage - stage name string can't be empty" );
    CTimer *timer = findTimer( nstg );
    if ( timer ) throw CFEException( "CPostProcessing::RegisterStage - stage \'" + nstg + "\' already registered" );
    timers[ nstg ] = CTimer();
    stages.push_back( nstg );
 }
