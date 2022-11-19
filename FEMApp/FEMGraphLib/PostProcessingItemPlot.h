#ifndef POSTPROCESSINGITEMPLOT_H
#define POSTPROCESSINGITEMPLOT_H

#include "../FEMLib/PostProcessItem.h"
#include "../FEMLib/FEException.h"
#include "../FEMGraphLib/PaintSetup.h"
#include "../MathLib/GeometricDomain.h"
#include "../FEMLib/FENode.h"
#include "../FEMLib/FiniteElementInstance.h"
#include "../FEMAppLib/FEMProject.h"

class CPostProcessingItemPlot : public CPostProcessingItem
{

public:



    CPostProcessingItemPlot(
                                const vector< string > &st,
                                const string &nm,
                                CFEMProject &fp,
                                const CPaintSetup &set
    );

    virtual bool userProcessing( CFE_Equation *eq, bool IsBegin = false );

protected:

    string filename;
    CFEMProject &FP;
    CPaintSetup setup;

    void FlushVTK();

};

#endif // POSTPROCESSINGITEMPLOT_H
