#pragma once
#include "FEFunctorMatrix.h"


class CFEFunctorMatrixColDOF :
	public CFEFunctorMatrix
{
public:
	
	CFEFunctorMatrixColDOF( const string &nm, bool c = true );
	~CFEFunctorMatrixColDOF( void );
	virtual CFEFunctorMatrixColDOF* Clone() =0;
	const CDOFVector& GetDOFVector() const { return alc.GetDOFs( ); }

};
