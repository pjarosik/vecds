#include "FEFunctorMatrixDeformationGrad.h"



CFEFunctorMatrixDeformationGrad::CFEFunctorMatrixDeformationGrad( CFEFunctorMatrix *a ):CFEFunctorMatrix("DeformationGrad"),du(a)
{
    AddNode( a );
}

CFEFunctorMatrixDeformationGrad::CFEFunctorMatrixDeformationGrad( const CFEFunctorMatrixDeformationGrad &mf ):CFEFunctorMatrix(mf)
{
    CLONE_MEMBER( mf, du )
}

void CFEFunctorMatrixDeformationGrad::Compute( CFiniteElementInstance *fe1 )
{
    matrix I(3,3);
    I(0,0) = 1.0;
                    I(1,1) = 1.0;
                                    I(2,2) = 1.0;
    fv = I - (*du) ;
    fv.Inverse();
}
