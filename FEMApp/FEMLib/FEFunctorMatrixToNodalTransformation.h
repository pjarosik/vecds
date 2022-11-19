#pragma once
#include "FEFunctorMatrix.h"
#include "Integrator.h"

class CFEFunctorMatrixToNodalTransformation:
	public CFEFunctorMatrix {
public:
    CFEFunctorMatrixToNodalTransformation( const CIntegrationManager &im, CFEFunctorMatrix *m );
	CFEFunctorMatrixToNodalTransformation( const CFEFunctorMatrixToNodalTransformation &mmf );
	virtual ~CFEFunctorMatrixToNodalTransformation( void ) { }
    CLONE_FN( CFEFunctorMatrixToNodalTransformation )
	virtual void ManageDOFs( CFiniteElementInstance *fe1 );
    CFEFunctorMatrix* GetGaussPointsResults() { return  gf; }

protected:

    CFEFunctorMatrix *gf;
    const CIntegrationManager &IM;
	virtual void Compute( CFiniteElementInstance *fe1 );

};
 
