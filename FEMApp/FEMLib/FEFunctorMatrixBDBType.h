#pragma once
#include "FEFunctorMatrixColDOF.h"

class CFEFunctorMatrixBDBType :
	public CFEFunctorMatrix
{

public:

	CFEFunctorMatrixBDBType( CFEFunctorMatrixColDOF *b1, CFEFunctorMatrix *d, CFEFunctorMatrixColDOF *b2 );
	CFEFunctorMatrixBDBType( const CFEFunctorMatrixBDBType &mf );
	virtual ~CFEFunctorMatrixBDBType( void );
	CLONE_FN( CFEFunctorMatrixBDBType )

protected:

	virtual void Compute( CFiniteElementInstance *fe1 );
	CFEFunctorMatrixColDOF *B1, *B2;
	CFEFunctorMatrix *D;

};
