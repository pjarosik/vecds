#include "FiniteElementElasticPlane.h"
#include "FEFunctorMatrixShapeFNLagrangeGrad.h"
#include "FEFunctorMatrixFEJacoby.h"
#include "FEFunctorMatrixStrainDerivPlane.h"
#include "FEFunctorMatrixBDBType.h"
#include "FEFunctorMatrixInGaussPoints.h"
#include "FEFunctorMatrixInt.h"

#include "FEFunctorVectorNaturalXi.h"
#include "FEFunctorVectorStrain.h"
#include "FEFunctorVectorStress.h"
#include "FEFunctorVectorMulti.h"
#include "FEFunctorVectorElemDOFs.h"
#include "FEFunctorMatrixToNodalTransformation.h"

CFiniteElementElasticPlane::CFiniteElementElasticPlane(
        vector< FEDomain* > vdom,
        CMaterial &mat,
        CIntegrationManager &IM,
        CFEFunctorMatrix &d
        ):CFiniteElement("ElasticSolid",vdom,mat,IM),D( (CFEFunctorMatrix*)d.CloneTree() )
{
}

void CFiniteElementElasticPlane::CreateElement()
{


// Finite Element Definition

            CDOF* tp[] = { &CDOFs::Ux, &CDOFs::Uy };
            CFEFunctorVectorElemDOFs q( tp, 2, "Plane displacement vector" );

            AddVector( "q", q );

            CFEFunctorVectorNaturalXi xi;
            CFEFunctorMatrixShapeFNLagrangeGrad dNx( &xi );     //dNx.printdiag=true;
            CFEFunctorMatrixFEJacoby J( &xi );                  //J.printdiag=true;
            CFEFunctorMatrixStrainDerivPlane B( &dNx );         // B.printdiag=true;
            CFEFunctorMatrixBDBType dKs( &B, D, &B );           //dKs.printdiag=true;
            CFEFunctorMatrixInt K( &IM, &xi, &dKs, &J );        // K.printdiag=true;

            AddMatrix( "K", K );

// Results Definition

            CFEFunctorVectorStrain strain( &B,  &q );
            CFEFunctorVectorStress stress( D, &strain );
            CFEFunctorVectorMulti mres;

            mres.Add( &stress );
            mres.Add( &strain );

            CFEFunctorMatrixInGaussPoints gpres( IM, &xi, &mres );
            CFEFunctorMatrixToNodalTransformation gptnd( IM, &gpres );

            CFEFunctorMatrix *temp = dynamic_cast< CFEFunctorMatrix* > ( gptnd.CloneTree() );
            MatrixFunctors.push_back( temp );

            AddResultMatrix( "nodes" , *temp );
            AddResultMatrix( "gauss" , gpres, false );


}
