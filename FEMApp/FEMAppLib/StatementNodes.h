#pragma once

#include "../LangLib/Statement.h"
#include "FEMProject.h"

class CStatementNodes :
		public CStatement
{
public:
	CStatementNodes( CFEMProject &fp );
	virtual ~CStatementNodes( void );

	virtual void Execute( istream &i );

protected:

	CFEMProject &FP;

};
