#pragma once
#include "FEFunctorVector.h"

class CFEFunctorVectorNaturalXi :
	public CFEFunctorVector
{
public:
	CFEFunctorVectorNaturalXi( );
	virtual ~CFEFunctorVectorNaturalXi( void );
	CLONE_FN( CFEFunctorVectorNaturalXi )

	virtual void ManageDOFs( CFiniteElementInstance *fe1 )  {  }
	virtual void Compute( CFiniteElementInstance *fe1 ) {   }
};
