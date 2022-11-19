#include "PostProcessingItemPlotGraph.h"
#include "../GraphLib/GraphDevicePrm.h"

CPostProcessingItemPlotGraph::CPostProcessingItemPlotGraph(
                                                        const vector< string > &st,
                                                        const string &nm,
                                                        CFEMProject &fp,
                                                        const CPaintSetup &set
        ):CPostProcessingItemPlot( st, nm, fp, set )
{

}


bool CPostProcessingItemPlotGraph::userProcessing( CFE_Equation *eq, bool IsBegin  )
{
    CGraphDeviceStorage GDV(filename);
}
