#pragma once

#include "../LangLib/Statement.h"
#include "FEMProject.h"
#include "../LangLib/statdef.h"


class CStatementElemDef :
		public CStatement
{
public:
	class CProblem
	{
		public:
			TokenParam type, domain,  material;
			CProblem():type(string("type")), domain(string("domain")),  material(string("material")) { }

	};

	CStatementElemDef( CFEMProject &fp );
	virtual ~CStatementElemDef( void );

	virtual void Execute( istream &i );

protected:

	CFEMProject &FP;


};

istream& operator >> ( istream &ist,CStatementElemDef::CProblem &pr );
