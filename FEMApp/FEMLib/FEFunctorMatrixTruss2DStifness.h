#pragma once
#include "FEFunctorMatrix.h"

class CFEFunctorMatrixTruss2DStifness :

	public CFEFunctorMatrix {
public:
    CFEFunctorMatrixTruss2DStifness( );
    CFEFunctorMatrixTruss2DStifness( const CFEFunctorMatrixTruss2DStifness &mmf );
    virtual ~CFEFunctorMatrixTruss2DStifness( void ) { }
	virtual void ManageDOFs( CFiniteElementInstance *fe1);

protected:

    virtual void Compute( CFiniteElementInstance *fe1 );


};
 

