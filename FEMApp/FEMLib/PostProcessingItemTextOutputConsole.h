#ifndef POSTPROCESSINGITEMTEXTOUTPUTCONSOLE_H
#define POSTPROCESSINGITEMTEXTOUTPUTCONSOLE_H

#include "PostProcessingItemTextOutput.h"

class CPostProcessingItemTextOutputConsole : public CPostProcessingItemTextOutput
{
public:

    CPostProcessingItemTextOutputConsole( const OutputDef &od );

protected:
    virtual bool userProcessing( CFE_Equation *eq, bool IsBegin = false ) { FlushOutput( cout, eq, IsBegin ); }
};

#endif // POSTPROCESSINGITEMTEXTOUTPUTCONSOLE_H
