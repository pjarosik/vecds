#pragma once
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"
#include "IntegrationManager.h"
#include "Integrator.h"

class CFEFunctorMatrixInt :
	public CFEFunctorMatrix
{
	
	CFEFunctorMatrix *integrand;
	CFEFunctorMatrix *Jacoby;
	CFEFunctorVector *xi;

public:

	CFEFunctorMatrixInt( const CIntegrationManager *g  , CFEFunctorVector *x, CFEFunctorMatrix *i, CFEFunctorMatrix *Jc  );
	CFEFunctorMatrixInt( const CIntegrationPoints  *ip , CFEFunctorVector *x, CFEFunctorMatrix *i, CFEFunctorMatrix *Jc  );
	CFEFunctorMatrixInt( const CFEFunctorMatrixInt &mf );
	virtual ~CFEFunctorMatrixInt( void );
    CLONE_FN( CFEFunctorMatrixInt )

    double GetVolume() const { return volume; }

protected:

    double volume;
	const CIntegrationManager *IM;
	const CIntegrationPoints *ip;
	virtual void Compute( CFiniteElementInstance *fe1 );

};
