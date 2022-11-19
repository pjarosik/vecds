#ifndef POSTPROCESSINGITEMPLOTGRAPH_H
#define POSTPROCESSINGITEMPLOTGRAPH_H

#include "PostProcessingItemPlot.h"

class CPostProcessingItemPlotGraph : public CPostProcessingItemPlot
{
public:
    CPostProcessingItemPlotGraph(const vector< string > &st,
                                const string &nm,
                                CFEMProject &fp,
                                const CPaintSetup &set);

    virtual bool userProcessing( CFE_Equation *eq, bool IsBegin = false );
};

#endif // POSTPROCESSINGITEMPLOTGRAPH_H
