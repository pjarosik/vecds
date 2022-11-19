#pragma once

#include "FEFunctorVector.h"
#include "FEFunctorMatrix.h"

class CFEFunctorVectorToLocalTransformation:
	public CFEFunctorVector {

public:

	CFEFunctorVectorToLocalTransformation( CFEFunctorMatrix *l, CFEFunctorVector *k );
	CFEFunctorVectorToLocalTransformation( const CFEFunctorVectorToLocalTransformation &mmf );
	virtual ~CFEFunctorVectorToLocalTransformation( void ) { }
	CLONE_FN( CFEFunctorVectorToLocalTransformation )

protected:

	CFEFunctorMatrix *L;
	CFEFunctorVector *K;
	virtual void Compute(CFiniteElementInstance *fe1 );


};

