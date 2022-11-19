#pragma once
#include "FEFunctorMatrix.h"



class CFEFunctorMatrixToGlobalTransformation:
	public CFEFunctorMatrix {
public:
	CFEFunctorMatrixToGlobalTransformation( CFEFunctorMatrix *l, CFEFunctorMatrix *k );
	CFEFunctorMatrixToGlobalTransformation( const CFEFunctorMatrixToGlobalTransformation &mmf );
	virtual ~CFEFunctorMatrixToGlobalTransformation( void ) { }
	
	CLONE_FN(CFEFunctorMatrixToGlobalTransformation)

protected:

	CFEFunctorMatrix *L,*K;
	virtual void Compute(CFiniteElementInstance *fe1);


};




