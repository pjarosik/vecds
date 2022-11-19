#include "FEFunctorMatrixMaterialPlaneStrainIzo.h"

CFEFunctorMatrixMaterialPlaneStrainIzo::CFEFunctorMatrixMaterialPlaneStrainIzo( ):CFEFunctorMatrix("CFEFunctorMatrixMaterialPlaneStrainIzo")
{

}

CFEFunctorMatrixMaterialPlaneStrainIzo::CFEFunctorMatrixMaterialPlaneStrainIzo( const CFEFunctorMatrixMaterialPlaneStrainIzo &mf ):CFEFunctorMatrix( mf )
{

}

CFEFunctorMatrixMaterialPlaneStrainIzo::~CFEFunctorMatrixMaterialPlaneStrainIzo( void )
{

}

void CFEFunctorMatrixMaterialPlaneStrainIzo::Compute( CFiniteElementInstance *fe1 )
{
	matrix &D=fv;
	double	E, nu;
	const CMaterial *mat = fe1->GetMaterial();
	E = mat->GetValue("E");
	nu = mat->GetValue("nu");
	double ww = E/(1.0-2.0*nu)/(1.0+nu);
 
	fv.SetDim(3,3);

	D(0,0)=(1.0-nu)*ww;  D(1,0)=ww*nu;
	D(0,1)=ww*nu;        D(1,1)=(1.0-nu)*ww;
											 D(2,2)=ww*(1.0-2.0*nu)/2.0;
}