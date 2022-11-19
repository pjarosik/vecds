#pragma once
#include "../LangLib/Statement.h"
#include "FEMProject.h"

class CStatementEquation :
		public CStatement
{

public:
	CStatementEquation( CFEMProject &fp );
	virtual ~CStatementEquation( void );

	virtual void Execute( istream &i );

protected:

	CFEMProject &FP;
	
};
