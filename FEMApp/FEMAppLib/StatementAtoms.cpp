#include "StatementAtoms.h"

CStatementAtoms::CStatementAtoms(CMolecularStructure &m):MS(m)
{
}

CStatementAtoms::~CStatementAtoms(void)
{
}

void CStatementAtoms::Execute( istream &i )
{
	string filename;
	i >> filename;
	MS.Init( filename );
}

