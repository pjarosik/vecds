#pragma once
#include "FEFunctorMatrix.h"


class CFEFunctorMatrixToLocalTransformation:
	public CFEFunctorMatrix {
public:
	CFEFunctorMatrixToLocalTransformation( CFEFunctorMatrix *l, CFEFunctorMatrix *k );
	CFEFunctorMatrixToLocalTransformation( const CFEFunctorMatrixToLocalTransformation &mmf );
	virtual ~CFEFunctorMatrixToLocalTransformation( void ) { }

protected:

	CFEFunctorMatrix *L,*K;
	virtual void Compute( CFiniteElementInstance *fe1 );


};
