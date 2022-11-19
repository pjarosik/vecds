#pragma once

#include "../LangLib/Statement.h"
#include "FEMProject.h"

class CStatementSolve :
		public CStatement
{
public:
	CStatementSolve( CFEMProject &fp );
	virtual ~CStatementSolve( void );

	virtual void Execute( istream &i );

protected:

	CFEMProject &FP;
	

};
