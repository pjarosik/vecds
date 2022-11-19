#include "FiniteElementHyperelastic.h"
#include "FEFunctorVectorElemDOFs.h"
#include "FEFunctorVectorNaturalXi.h"
#include "FEFunctorMatrixShapeFNLagrangeGrad.h"
#include "FEFunctorMatrixFEJacoby.h"
#include "FEFunctorVectorShapeFn.h"
#include "FEFunctorMatrixDisplacementGrad.h"
#include "FEFunctorMatrixDeformationGrad.h"
#include "FEFunctorMatrixAAA.h"

CFiniteElementHyperelastic::CFiniteElementHyperelastic(
                                                            vector< FEDomain* > vdom,
                                                            CMaterial &mat,
                                                            CIntegrationManager &IM,
                                                            CFEFunctorMatrix &d
        ):CFiniteElement( "Hyperelastic", vdom, mat, IM ), D( (CFEFunctorMatrix*)d.CloneTree() )

{

}


void CFiniteElementHyperelastic::CreateElement()
{
    CDOF* tp[] = { &CDOFs::Ux, &CDOFs::Uy, &CDOFs::Uy };
    CFEFunctorVectorElemDOFs q( tp, 3, "Plane displacement vector" );

    V["q"] = dynamic_cast< CFEFunctorVector* > ( q.CloneTree() );

    CFEFunctorVectorNaturalXi xi;
    CFEFunctorVectorShapeFn sfn( &xi );
    CFEFunctorMatrixFEJacoby J( &xi );                  //J.printdiag=true;
    CFEFunctorMatrixShapeFNLagrangeGrad dNx( &xi ); //dNx.printdiag=true;
    CFEFunctorMatrixDisplacementGrad du( &dNx, &J );     //dNx.printdiag=true;
    CFEFunctorMatrixDeformationGrad Fe( &du );
    CFEFunctorMatrixAAA AAA( &Fe );

}
