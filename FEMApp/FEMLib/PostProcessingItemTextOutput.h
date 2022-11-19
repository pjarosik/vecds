#ifndef POSTPROCESSINGITEMTEXTOUTPUT_H
#define POSTPROCESSINGITEMTEXTOUTPUT_H

#include "PostProcessingItemOutput.h"
#include "FE_Equation.h"
#include "FEException.h"
#include "NodeSelector.h"
#include "ElemInstanceSelector.h"
#include <ostream>


class CPostProcessingItemTextOutput : public CPostProcessingItemOutput
{

public:

    CPostProcessingItemTextOutput( const OutputDef &od );
    ~CPostProcessingItemTextOutput() { }

    void SetFormat( const string &type, unsigned s, unsigned prec );

protected:

    string frm, par;
    unsigned precision, size, par_size;

    void WriteSpacedText( ostream &out, const string &str );
    bool FlushOutput( ostream &out, CFE_Equation *eq, bool IsBegin = false );

};

#endif // POSTPROCESSINGITEMTEXTOUTPUT_H
