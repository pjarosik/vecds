#pragma once

#include "../LangLib/Statement.h"
#include "FEMProject.h"

class CStatementResults :
		public CStatement
{
public:
	CStatementResults( CFEMProject &fp );
	virtual ~CStatementResults( void );
	virtual void Execute( istream &i );
protected:
	CFEMProject &FP;

};

