
#define _USE_MATH_DEFINES
#include <cmath>

#include "CrackInInterface.h"
#include "DOFs.h"
#include "FENode.h"

#include "FEFunctorMatrixMaterialPlaneStressIzo.h"
#include "FEFunctorMatrixMaterialPlaneStrainIzo.h"
#include "FEFunctorVectorNaturalXi.h"
#include "FEFunctorVectorLagrangeX.h"
#include "FEFunctorVectorQweight.h"
#include "FEFunctorMatrixShapeFNLagrangeGrad.h"
#include "FEFunctorVectorUauxK1.h"
#include "FEFunctorVectorUauxK2.h"
#include "FEFunctorMatrixGradUauxK1.h"
#include "FEFunctorMatrixGradUauxK2.h"
#include "FEFunctorMatrixdJint.h"
#include "FEFunctorVectorStraindU.h"
#include "FEFunctorVectorStress.h"
#include "FEFunctorMatrixGradU_XFEM.h"
#include "FEFunctorMatrixFEJacoby.h"
#include "FEFunctorMatrixInt.h"

#include "../FEMLib/GaussPointsRectangular.h"
#include "../FEMLib/IntegrationPointsMultiTriangular.h"


CCrackInInterface::CCrackInInterface(const mvector &a1, const mvector &a2,CMaterial *m1,CMaterial *m2,bool ips):CCrackLine(a1,a2),mat1(m1),mat2(m2),is_plane_stress(ips)
{

	double nu1,nu2,E1,E2;

	nu1 = m1->GetValue("nu");
	nu2 = m2->GetValue("nu");
	E1  = m1->GetValue("E");
	E2  = m2->GetValue("E");

//	eps=0.076;

	if ( is_plane_stress )
	{
		k1 = ( 3.0 - nu1 ) / ( 1.0 + nu1 );
		k2 = ( 3.0 - nu2 ) / ( 1.0 + nu2 );
	}
	else
	{
		k1 = 3.0 - 4.0 * nu1;
		k2 = 3.0 - 4.0 * nu2;
	}

//	E / 2.0 / ( 1.0 - ni )

	mu1 = E1 / 2.0 / ( 1.0 - nu1 );
	mu2 = E2 / 2.0 / ( 1.0 - nu2 );

	alpha = (mu1*(k2+1.0)-mu2*(k1+1.0))/(mu1*(k2+1.0)+mu2*(k1+1.0));
	beta  = (mu1*(k2-1.0)-mu2*(k1-1.0))/(mu1*(k2+1.0)+mu2*(k1+1.0));

	eps = 1.0 / 2.0 / M_PI * log10( ( 1.0 - beta ) / ( 1.0 + beta ) );

//	eps = 0.076;
}

CCrackInInterface::~CCrackInInterface(void)
{

}

void CCrackInInterface::EnrichNodes()
{
	unsigned k,l,dim=0;
	CDOF *dof = 0;

	mvector X(2),X1(2),X2(2);
//	double dt = ( t2 - t1 ) / (double)ndiv;
	double a1 = angle(t1) + M_PI, a2 = angle( t2 );

	XYZ( t1, X1 );
	XYZ( t2, X2 );

	if ( Te1.size() )
	{
		tip1 = new CEnrichmentFunctionsBimaterial(X1,eps,a1);
		dim = tip1->GetN();
	}

	if ( Te2.size() )
	{
		tip2 = new CEnrichmentFunctionsBimaterial(X2,eps,a2);
		dim = tip2->GetN();
	}

	for (k=0; k<Hn.size(); k++)
	{
		dof = Hn[ k ]->FindDOF( DOF_AX );  if ( !dof ) { dof = CDOFs::Ax.Clone(); Hn[ k ]->AddDOF( dof );  }
		dof = Hn[ k ]->FindDOF( DOF_AY );  if ( !dof ) { dof = CDOFs::Ay.Clone(); Hn[ k ]->AddDOF( dof );  }
	}

	for (k=0; k<Tn.size(); k++)
	{
		for (l=0; l<dim; l++)
		{
			dof = Tn[ k ]->FindDOF( DOF_BX + 2 * l );  if ( !dof ) { dof = new CDOF_Bx( 2 * l ); Tn[ k ]->AddDOF( dof );  }
			dof = Tn[ k ]->FindDOF( DOF_BY + 2 * l );  if ( !dof ) { dof = new CDOF_By( 2 * l ); Tn[ k ]->AddDOF( dof );  }
		}
	}


}

void CCrackInInterface::ComputeSIFs(  double dist  )
{
	mvector X2( 2 );
	XYZ( t2, X2 );

	CFEFunctorMatrixMaterialPlaneStressIzo Dstress;
	CFEFunctorMatrixMaterialPlaneStressIzo Dstrain;

	CFEFunctorMatrix &D = is_plane_stress ? Dstress : Dstrain;
	CFEFunctorVectorNaturalXi xi;
	CFEFunctorVectorLagrangeX X(&xi);
	CFEFunctorMatrixShapeFNLagrangeGrad dN( &xi );
	CFEFunctorVectorQweight dq( &dN, X2, dist );

	CFEFunctorVectorUauxK1 UauxK1( this, &X );
	CFEFunctorVectorUauxK2 UauxK2( this, &X );

	CFEFunctorMatrixGradUauxK1 dUauxK1( this, &X );
	CFEFunctorMatrixGradUauxK2 dUauxK2( this, &X );
	CFEFunctorVectorStraindU eauxK1( &dUauxK1 );
	CFEFunctorVectorStraindU eauxK2( &dUauxK2 );
	CFEFunctorVectorStress sauxK1(&D,&eauxK1);
	CFEFunctorVectorStress sauxK2(&D,&eauxK2);
	CFEFunctorMatrixGradU_XFEM dU( this, tip2, &X, &dN );
	CFEFunctorVectorStraindU e(&dU);
	CFEFunctorVectorStress s(&D,&e);

	CFEFunctorMatrixdJint dJintK1(&sauxK1,&eauxK1,&dUauxK1,&s,&dU,&dq), dJintK2(&sauxK2,&eauxK2,&dUauxK2,&s,&dU,&dq);

	CGaussPointsRectangular gpr(2,10);
	CIntegrationPointsMultiTriangular gt(2,13);

	CFEFunctorMatrixFEJacoby Je( &xi );
	CFEFunctorMatrixInt K1h( &gt , &xi, &dJintK1, &Je  ), K1r( &gpr , &xi, &dJintK1, &Je  );
	CFEFunctorMatrixInt K2h( &gt , &xi, &dJintK2, &Je  ), K2r( &gpr , &xi, &dJintK2, &Je  );

	K1h.CreateUsedByVector();
	K1r.CreateUsedByVector();
	K2h.CreateUsedByVector();
	K2r.CreateUsedByVector();

	hFE.clear();
	rFE.clear();

	unsigned k;
	for (k=0; k<He.size(); k++)
		if ( dq.AcceptFE(He[ k ]) ) hFE.push_back( He[k] );


	for (k=0; k<Re.size(); k++)
		if ( dq.AcceptFE( Re[ k ] ) ) rFE.push_back( Re[k] );


	double I1=0.0, I2=0.0;

	for (k=0; k<hFE.size(); k++)
	{
		K1h( hFE[ k ] );
		const matrix &k1 = K1h;
		I1 -= k1(0,0);

		K2h( hFE[k] );
		const matrix &k2 = K2h;
		I2 -= k2(0,0);
	}

	for (k=0; k<rFE.size(); k++)
	{
		K1r( rFE[k] );
		const matrix &k1 = K1r;
		I1 -= k1(0,0);

		K2r( rFE[k] );
		const matrix &k2 = K2r;
		I2 -= k2(0,0);
	}

	double E1,E2,Eg;

	E1 = mat1->GetValue("E");
	E2 = mat2->GetValue("E");

	Eg = 2.0 / ( 1.0 / E1 + 1.0 / E2 );

	K0 = 100.0*sqrt(M_PI*0.17);
	K1=Eg*cosh(M_PI*eps)*cosh(M_PI*eps)*I1/2.0;
	K2=Eg*cosh(M_PI*eps)*cosh(M_PI*eps)*I2/2.0;
}
