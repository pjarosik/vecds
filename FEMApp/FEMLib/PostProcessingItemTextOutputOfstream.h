#ifndef POSTPROCESSINGITEMTEXTOUTPUTOFSTREAM_H
#define POSTPROCESSINGITEMTEXTOUTPUTOFSTREAM_H

#include "PostProcessingItemTextOutput.h"

class CPostProcessingItemTextOutputOfstream : public CPostProcessingItemTextOutput
{
public:
    CPostProcessingItemTextOutputOfstream( const OutputDef &od );

 protected:

    virtual bool userProcessing( CFE_Equation *eq, bool IsBegin = false );

};

#endif // POSTPROCESSINGITEMTEXTOUTPUTOFSTREAM_H
