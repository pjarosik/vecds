#ifndef STATEMENTSPACE_H
#define STATEMENTSPACE_H
#include "FEMProject.h"
#include "../LangLib/statdef.h"

class CStatementSpace : public CStatement
{
public:
    CStatementSpace( CFEMProject &fp  );
    virtual void Execute( istream &i );
protected:
    CFEMProject &FP;
    NamedParameterListDoubleParameter2 params_par;
    double FindParam( const string &nm );
};

#endif // STATEMENTSPACE_H
