#include "FEFunctorMatrixMaterialPlaneStressIzo.h"


CFEFunctorMatrixMaterialPlaneStressIzo::CFEFunctorMatrixMaterialPlaneStressIzo( ):CFEFunctorMatrix("CFEFunctorMatrixMaterialPlaneStressIzo")
{

}

CFEFunctorMatrixMaterialPlaneStressIzo::CFEFunctorMatrixMaterialPlaneStressIzo( const CFEFunctorMatrixMaterialPlaneStressIzo &mf ):CFEFunctorMatrix( mf )
{

}

CFEFunctorMatrixMaterialPlaneStressIzo::~CFEFunctorMatrixMaterialPlaneStressIzo( void )
{

}

void CFEFunctorMatrixMaterialPlaneStressIzo::Compute( CFiniteElementInstance *fe1 )
{
	matrix &D=fv;
	double	E, nu,h;
	const CMaterial *mat = fe1->GetMaterial();
	E = mat->GetValue("E");
	nu = mat->GetValue("nu");
	h = mat->GetValue("h");
	double ww=h*E/(1.0-nu*nu);

	D.SetDim(3,3);

	D(0,0)=ww;    D(1,0)=ww*nu;
	D(0,1)=ww*nu; D(1,1)=ww;
								D(2,2)=ww*(1.0-nu)/2.0;
}