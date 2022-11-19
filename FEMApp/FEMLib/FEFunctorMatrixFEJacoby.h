#pragma once
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"


class CFEFunctorMatrixFEJacoby :
	public CFEFunctorMatrix
{
public:
	CFEFunctorMatrixFEJacoby( CFEFunctorVector *x );
	CFEFunctorMatrixFEJacoby( const CFEFunctorMatrixFEJacoby &mf );
	virtual ~CFEFunctorMatrixFEJacoby(void);
	CLONE_FN( CFEFunctorMatrixFEJacoby )
	virtual void ManageDOFs(CFiniteElementInstance *fe1 ) { }

protected:
	
	virtual void Compute( CFiniteElementInstance *fe1 ) { fe1->GetJacobyMatrix( (*xi), fv ); }
	CFEFunctorVector *xi;
};
