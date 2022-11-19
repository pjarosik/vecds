#include "FEFunctorMatrixMaterialSolidIzo.h"
#include "FEException.h"

CFEFunctorMatrixMaterialSolidIzo::CFEFunctorMatrixMaterialSolidIzo(  ):CFEFunctorMatrix("CFEFunctorMatrixMaterialSolidIzo")
{

}

CFEFunctorMatrixMaterialSolidIzo::CFEFunctorMatrixMaterialSolidIzo( const CFEFunctorMatrixMaterialSolidIzo &mf ):CFEFunctorMatrix( mf )
{

}

CFEFunctorMatrixMaterialSolidIzo::~CFEFunctorMatrixMaterialSolidIzo( void )
{

}

void CFEFunctorMatrixMaterialSolidIzo::Compute( CFiniteElementInstance *fe1 )
{
	matrix &D = fv;

	const CMaterial *mat = fe1->GetMaterial();
	if (!mat) throw CFEException("Material not assigned to finite element instance");
	double	E, nu;
	E = mat->GetValue("E");
	nu = mat->GetValue("nu");
	
	D.SetDim(6,6);
	D(0,0) = D(1,1) = D(2,2) = 1.0 - nu;
	D(3,3) = D(4,4) = D(5,5) = (1.0 - 2.0 * nu) / 2.0;
	D(0,1) = D(1,0) = D(1,2) = D(2,1) = D(0,2) = D(2,0) = nu;

	D *= E / ( 1.0 + nu ) / ( 1 - 2.0 * nu );
}