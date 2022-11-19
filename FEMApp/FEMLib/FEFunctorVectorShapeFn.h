   #pragma once
#include "FEFunctorVector.h"

class CFEFunctorVectorShapeFn :
	public CFEFunctorVector
{
public:
	CFEFunctorVectorShapeFn( CFEFunctorVector *x );
	CFEFunctorVectorShapeFn( const CFEFunctorVectorShapeFn &vf );
	CLONE_FN(CFEFunctorVectorShapeFn)
	virtual ~CFEFunctorVectorShapeFn( void );
	virtual void ManageDOFs( CFiniteElementInstance *fe1 )  {  }


protected:

    virtual void Compute( CFiniteElementInstance *fe1 ) { fe1->GetShapeSpace()->transform( (*xi), fv ); }

	CFEFunctorVector *xi;
};
