#include "FiniteElementTruss3D.h"

#include "FEFunctorVectorElemDOFs.h"
#include "FEFunctorVectorToLocalTransformation.h"
#include "FEFunctorVectorTruss3DResidual.h"
#include "FEFunctorVectorToGlobalTransformation.h"

#include "FEFunctorMatrixTruss3DTransformation.h"
#include "FEFunctorMatrixTruss3DTangent.h"
#include "FEFunctorMatrixToGlobalTransformation.h"


CFiniteElementTruss3D::CFiniteElementTruss3D(
        vector< FEDomain* > vdom,
        CMaterial &mat,
        CIntegrationManager &IM
        ):CFiniteElement("Truss3D",vdom,mat,IM)
{
}

void CFiniteElementTruss3D::CreateElement()
{
    CDOF* pDOFs[] = { &CDOFs::Ux, &CDOFs::Uy, &CDOFs::Uz };
    unsigned DOFs[] = { DOF_UX, DOF_UY, DOF_UZ };
    CFEFunctorVectorElemDOFs q( pDOFs, 3, "Truss 3D displacement vector" );


    CFEFunctorMatrixTruss3DTransformation L;
    CFEFunctorVectorToLocalTransformation qe( &L, &q ); //qe.printdiag=true;

    CFEFunctorMatrixTruss3DTangent Kte( &qe ); //Kte.printdiag=true;
    CFEFunctorVectorTruss3DResidual Re( &qe ); //Re.printdiag=true;

    CFEFunctorVectorToGlobalTransformation R( &L, &Re ); //R.printdiag=true;
    CFEFunctorMatrixToGlobalTransformation Kt( &L, &Kte );

    M["Kt"]	= dynamic_cast<CFEFunctorMatrix*>( Kt.CloneTree() );
    V["R"]	= dynamic_cast<CFEFunctorVector*>( R.CloneTree() );
    V["q"]	= dynamic_cast<CFEFunctorVector*>( q.CloneTree() );

}
