#ifndef STATEMENTPLOT_H
#define STATEMENTPLOT_H

#include "../LangLib/Statement.h"
#include "FEMProject.h"

class CStatementPlot : public CStatement
{

public:

    CStatementPlot( CFEMProject &fp );
    virtual void Execute( istream &i );

protected:

    CFEMProject &FP;

};

#endif // STATEMENTPLOT_H
