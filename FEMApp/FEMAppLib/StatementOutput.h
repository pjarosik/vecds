#pragma once
#include "../LangLib/Statement.h"
#include "FEMProject.h"

class CStatementOutput :
	public CStatement
{
public:

	CStatementOutput( CFEMProject &fp );
    virtual ~CStatementOutput( void );
    virtual void Execute( istream &i );

protected:

	 CFEMProject &FP; 
};

