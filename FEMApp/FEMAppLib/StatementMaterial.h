#pragma once

#include "../LangLib/Statement.h"
#include "FEMProject.h"

class CStatementMaterial :
	public CStatement
{
public:
	CStatementMaterial( CFEMProject &fp );
	virtual ~CStatementMaterial(void);

	virtual void Execute( istream &i );

protected:

	CFEMProject &FP;

};

