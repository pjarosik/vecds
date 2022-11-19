#include "FEFunctorMatrixTruss2DStifness.h"


CFEFunctorMatrixTruss2DStifness::CFEFunctorMatrixTruss2DStifness(  ):CFEFunctorMatrix("CFEFunctorMatrixTruss2DStifnessMatrix")
{
    

    
}

CFEFunctorMatrixTruss2DStifness::CFEFunctorMatrixTruss2DStifness( const CFEFunctorMatrixTruss2DStifness &mf ):CFEFunctorMatrix(mf)
{
   
}


void CFEFunctorMatrixTruss2DStifness::ManageDOFs( CFiniteElementInstance *fe1 )
{


}

void CFEFunctorMatrixTruss2DStifness::Compute( CFiniteElementInstance *fe1 )
{

} 
