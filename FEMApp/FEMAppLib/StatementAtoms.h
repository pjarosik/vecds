#pragma once

#include "../LangLib/Statement.h"
#include "../MolecularLib/MolecularStructure.h"

class CStatementAtoms :
	public CStatement
{
public:
	CStatementAtoms(CMolecularStructure &m);
	virtual ~CStatementAtoms(void);

	virtual void Execute( istream &i );

protected:
	CMolecularStructure &MS;
};
