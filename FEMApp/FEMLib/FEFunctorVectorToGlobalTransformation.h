#pragma once

#include "FEFunctorVector.h"
#include "FEFunctorMatrix.h"


class CFEFunctorVectorToGlobalTransformation:
	public CFEFunctorVector {
public:
	CFEFunctorVectorToGlobalTransformation( CFEFunctorMatrix *l, CFEFunctorVector *k );
	CFEFunctorVectorToGlobalTransformation( const CFEFunctorVectorToGlobalTransformation &mmf );
	virtual ~CFEFunctorVectorToGlobalTransformation( void ) { }
	CLONE_FN( CFEFunctorVectorToGlobalTransformation )

protected:

	CFEFunctorMatrix *L;
	CFEFunctorVector *K;
	virtual void Compute( CFiniteElementInstance *fe1);


};

