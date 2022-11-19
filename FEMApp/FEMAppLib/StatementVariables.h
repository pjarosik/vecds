#ifndef STATEMENTVARIABLES_H
#define STATEMENTVARIABLES_H

#include "../LangLib/Statement.h"
#include"../MathLib/DBNode.h"

class CStatementVariables : public CStatement
{
public:
    CStatementVariables( CDBNode &s );
    virtual void Execute( istream &i );
protected:
    CDBNode &System;
};

#endif // STATEMENTVARIABLES_H
