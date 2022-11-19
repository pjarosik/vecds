#include "FiniteElementElasticSolid.h"
#include "FEFunctorVectorElemDOFs.h"
#include "FEFunctorVectorNaturalXi.h"
#include "FEFunctorVectorStrain.h"
#include "FEFunctorVectorStress.h"
#include "FEFunctorVectorMulti.h"

#include "FEFunctorMatrixShapeFNLagrangeGrad.h"
#include "FEFunctorMatrixFEJacoby.h"
#include "FEFunctorMatrixStrainDerivSolid.h"
#include "FEFunctorMatrixBDBType.h"
#include "FEFunctorMatrixInt.h"
#include "FEFunctorMatrixInGaussPoints.h"
#include "FEFunctorMatrixToNodalTransformation.h"

CFiniteElementElasticSolid::CFiniteElementElasticSolid(
        const vector< FEDomain* > &vdom,
        CMaterial &mat,
        CIntegrationManager &IM,
        CFEFunctorMatrix &d
        ):CFiniteElement("ElasticSolid",vdom,mat,IM), D( (CFEFunctorMatrix*)d.CloneTree( ) )
{

}


void CFiniteElementElasticSolid::CreateElement()
{

// Finite Element Definition

    CDOF* tp[] = { &CDOFs::Ux, &CDOFs::Uy, &CDOFs::Uz };
    CFEFunctorVectorElemDOFs q( tp, 3, "Solid displacement vector" );

    V["q"] = dynamic_cast< CFEFunctorVector* > ( q.CloneTree() );

    CFEFunctorVectorNaturalXi xi;
    CFEFunctorMatrixShapeFNLagrangeGrad dNx( &xi ); //dNx.printdiag=true;
    CFEFunctorMatrixFEJacoby J( &xi );           //J.printdiag=true;
    CFEFunctorMatrixStrainDerivSolid B( &dNx );  //B.printdiag=true;
    CFEFunctorMatrixBDBType dKs( &B, D, &B );   //dKs.printdiag=true;

    CFEFunctorMatrixInt K( &IM, &xi, &dKs, &J ); // K.printdiag=true;

    M["K"] = dynamic_cast< CFEFunctorMatrix* > ( K.CloneTree() );

// Results Definition

    CFEFunctorVectorStrain strain( &B,  &q );
    CFEFunctorVectorStress stress( D, &strain );
    CFEFunctorVectorMulti mres;
    mres.Add( &stress );
    mres.Add( &strain );

    CFEFunctorMatrixInGaussPoints gpres( IM, &xi, &mres );
    CFEFunctorMatrixToNodalTransformation gptnd( IM, &gpres );
    CFEFunctorMatrixToNodalTransformation *temp = dynamic_cast<CFEFunctorMatrixToNodalTransformation*>( gptnd.CloneTree() );

    MR["gauss"] = dynamic_cast< CFEFunctorMatrix* >( temp->GetGaussPointsResults() );
    MR["nodal"] = dynamic_cast< CFEFunctorMatrix* >( temp );

}
