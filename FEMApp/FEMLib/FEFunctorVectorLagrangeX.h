#pragma once

#include "FEFunctorVector.h"

class CFEFunctorVectorLagrangeX :
	public CFEFunctorVector
{
public:
	CFEFunctorVectorLagrangeX( CFEFunctorVector *x );
	CFEFunctorVectorLagrangeX( const CFEFunctorVectorLagrangeX &vf );
	virtual ~CFEFunctorVectorLagrangeX( void );
	CLONE_FN( CFEFunctorVectorLagrangeX )

	virtual void ManageDOFs( CFiniteElementInstance *fe1 )  {  }
    virtual void Compute( CFiniteElementInstance *fe1 ) {  fe1->GetShapeSpace()->Transform( (const mvector&)(*xi), fv ); }

protected:
	CFEFunctorVector *xi;
};
