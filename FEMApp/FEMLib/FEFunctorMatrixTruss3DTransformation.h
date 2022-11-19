#pragma once
#include "FEFunctorMatrix.h"


class CFEFunctorMatrixTruss3DTransformation :
	public CFEFunctorMatrix
{

public:
	
	CFEFunctorMatrixTruss3DTransformation( void );
	CFEFunctorMatrixTruss3DTransformation( const CFEFunctorMatrixTruss3DTransformation &mf );
	virtual ~CFEFunctorMatrixTruss3DTransformation(void) { }
	CLONE_FN(CFEFunctorMatrixTruss3DTransformation)
	void ManageDOFs( CFiniteElementInstance *fe1 );

protected:

	void Compute( CFiniteElementInstance *fe1 );
};

