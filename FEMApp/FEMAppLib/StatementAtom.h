#pragma once
#include "../LangLib/Statement.h"
#include "../LangLib/statdef.h"
#include "../MolecularLib/MolecularStructure.h"

class CStatementAtom :
	public CStatement
{
public:
	CStatementAtom( CMolecularStructure &m );
	virtual ~CStatementAtom( void );

	virtual void Execute( istream &i );

protected:
	CMolecularStructure &MS;
	
};
