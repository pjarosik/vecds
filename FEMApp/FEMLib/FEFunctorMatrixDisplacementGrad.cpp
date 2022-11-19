#include "FEFunctorMatrixDisplacementGrad.h"

CFEFunctorMatrixDisplacementGrad::CFEFunctorMatrixDisplacementGrad( CFEFunctorMatrix *nx, CFEFunctorMatrix *j ) :CFEFunctorMatrix("DisplacementGrad"),Nx(nx),J(j)
{

    AddNode( nx );
    AddNode( j );
}

CFEFunctorMatrixDisplacementGrad::CFEFunctorMatrixDisplacementGrad( const CFEFunctorMatrixDisplacementGrad &vf ):CFEFunctorMatrix( vf )
{
    CLONE_MEMBER( vf, Nx )
    CLONE_MEMBER( vf, J )
}


void CFEFunctorMatrixDisplacementGrad::Compute( CFiniteElementInstance *fe1 )
{
    fv = (*Nx) * (*J);
}
