#ifndef POSTPROCESSINGITEMXMLOUTPUT_H
#define POSTPROCESSINGITEMXMLOUTPUT_H

#include "../FEMLib/PostProcessingItemOutput.h"
#include "../XMLLib/XML.h"
#include "FEMProject.h"

class CPostProcessingItemXMLOutput : public CPostProcessingItemOutput
{

public:

    CPostProcessingItemXMLOutput( const OutputDef &od, CFEMProject &fp );
    virtual bool userProcessing( CFE_Equation *eq, bool IsBegin = false );

protected:

    CFEMProject &FP;
    unsigned precision, size;

};

#endif // POSTPROCESSINGITEMXMLOUTPUT_H
