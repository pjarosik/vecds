#pragma once

#include "../LangLib/Statement.h"
#include "FEMProject.h"

class CStatementTitle :
	public CStatement
{
public:
	CStatementTitle(CFEMProject &fp);
	virtual ~CStatementTitle(void);
	virtual void Execute( istream &i );
protected:
	CFEMProject &FP;

};

