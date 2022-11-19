#pragma once

#include "../LangLib/Statement.h"
#include "FEMProject.h"


class CStatementElements :
	public CStatement
{
public:
	CStatementElements( CFEMProject &fp );
	virtual ~CStatementElements( void );

	virtual void Execute( istream &i );

protected:

	CFEMProject &FP;
	

};
