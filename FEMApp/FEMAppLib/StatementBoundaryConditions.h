#pragma once

#include "../LangLib/Statement.h"
#include "FEMProject.h"
#include "../FEMLib/FENode.h"
#include "../LangLib/statdef.h"

class CStatementBoundaryConditions :
		public CStatement
{
    void FixNodes( const NamedParameterListTokenExpressions &vl );
public:
	CStatementBoundaryConditions( CFEMProject &fp );
	virtual ~CStatementBoundaryConditions( void );

	virtual void Execute( istream &i );


protected:

	CFEMProject &FP;
	

};

