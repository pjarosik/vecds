#include "FEFunctorMatrixMaterialSolidOrtho.h"

CFEFunctorMatrixMaterialSolidOrtho::CFEFunctorMatrixMaterialSolidOrtho(  ):CFEFunctorMatrix("CFEFunctorMatrixMaterialSolidOrtho")
{

}

CFEFunctorMatrixMaterialSolidOrtho::CFEFunctorMatrixMaterialSolidOrtho( const CFEFunctorMatrixMaterialSolidOrtho &mf ):CFEFunctorMatrix( mf )
{

}

CFEFunctorMatrixMaterialSolidOrtho::~CFEFunctorMatrixMaterialSolidOrtho( void )
{

}

void CFEFunctorMatrixMaterialSolidOrtho::Compute( CFiniteElementInstance *fe1 )
{
	double	Ex, Ey, Ez, nuxy, nuyz, nuxz, Gxy, Gyz, Gxz;
	const CMaterial *mat = fe1->GetMaterial();

	Ex = mat->GetValue( "Ex" );		Ey = mat->GetValue( "Ey" );		Ez = mat->GetValue( "Ez" );
	nuxy = mat->GetValue( "nuxy" );	nuyz = mat->GetValue( "nuyz" );	nuxz = mat->GetValue( "nuxz" );
	Gxy = mat->GetValue( "Gxy" );	Gyz = mat->GetValue( "Gyz" );	Gxz = mat->GetValue( "Gxz" );

	double nuyx =  nuxy * Ey / Ex, nuzy =  nuyz * Ez / Ey, nuzx =  nuxz * Ez / Ex;
	double dV = (1.0-nuxy*nuyx-nuyz*nuzy-nuxz*nuxz-2.0*nuxy*nuyz*nuxz)/Ex/Ey/Ez;

	matrix &D = *this;

	D.SetDim(6,6);

	D(0,0)=(1.0-nuyz*nuzy)/Ey/Ez/dV;	D(0,1)=(nuyx+nuzx*nuyz)/Ey/Ez/dV;	D(0,2)=(nuzx+nuyx*nuzy)/Ey/Ez/dV;
	D(1,0)=(nuxy+nuxz*nuzy)/Ex/Ez/dV;	D(1,1)=(1.0-nuzx*nuxz)/Ex/Ez/dV;	D(1,2)=(nuzy+nuzx*nuxy)/Ex/Ez/dV;
	D(2,0)=(nuxz+nuxy*nuyz)/Ex/Ey/dV;	D(2,1)=(nuyz+nuxz*nuyx)/Ex/Ey/dV;	D(2,2)=(1.0-nuxy*nuyx)/Ex/Ey/dV;

	D(3,3) = 2.0 * Gyz;
	D(4,4) = 2.0 * Gxz;
	D(5,5) = 2.0 * Gxy;	
}