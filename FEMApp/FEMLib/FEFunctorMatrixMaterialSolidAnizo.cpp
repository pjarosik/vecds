#include "FEFunctorMatrixMaterialSolidAnizo.h"

CFEFunctorMatrixMaterialSolidAnizo::CFEFunctorMatrixMaterialSolidAnizo(  ):CFEFunctorMatrix("CFEFunctorMatrixMaterialSolidAnizo")
{

}

CFEFunctorMatrixMaterialSolidAnizo::CFEFunctorMatrixMaterialSolidAnizo( const CFEFunctorMatrixMaterialSolidAnizo &mf ):CFEFunctorMatrix( mf )
{

}

CFEFunctorMatrixMaterialSolidAnizo::~CFEFunctorMatrixMaterialSolidAnizo( void )
{

}

void CFEFunctorMatrixMaterialSolidAnizo::Compute( CFiniteElementInstance *fe1 )
{
	matrix &D=fv;

	const CMaterial *mat = fe1->GetMaterial();

	D.SetDim(6,6);
	
	D(0,0) = mat->GetValue( "D00" ); D(0,1) = mat->GetValue( "D01" ); D(0,2) = mat->GetValue( "D02" ); D(0,3) = mat->GetValue( "D03" ); D(0,4) = mat->GetValue( "D04" ); D(0,5) = mat->GetValue( "D05"  );
	D(1,0) = mat->GetValue( "D10" ); D(1,1) = mat->GetValue( "D11" ); D(1,2) = mat->GetValue( "D12" ); D(1,3) = mat->GetValue( "D13" ); D(1,4) = mat->GetValue( "D14" ); D(1,5) = mat->GetValue( "D15"  );
	D(2,0) = mat->GetValue( "D20" ); D(2,1) = mat->GetValue( "D21" ); D(2,2) = mat->GetValue( "D22" ); D(2,3) = mat->GetValue( "D23" ); D(2,4) = mat->GetValue( "D24" ); D(2,5) = mat->GetValue( "D25"  );
	D(3,0) = mat->GetValue( "D30" ); D(3,1) = mat->GetValue( "D31" ); D(3,2) = mat->GetValue( "D32" ); D(3,3) = mat->GetValue( "D33" ); D(3,4) = mat->GetValue( "D34" ); D(3,5) = mat->GetValue( "D35"  );
	D(4,0) = mat->GetValue( "D40" ); D(4,1) = mat->GetValue( "D41" ); D(4,2) = mat->GetValue( "D42" ); D(4,3) = mat->GetValue( "D43" ); D(4,4) = mat->GetValue( "D44" ); D(4,5) = mat->GetValue( "D55"  );
	D(5,0) = mat->GetValue( "D50" ); D(5,1) = mat->GetValue( "D51" ); D(5,2) = mat->GetValue( "D52" ); D(5,3) = mat->GetValue( "D53" ); D(5,4) = mat->GetValue( "D54" ); D(5,5) = mat->GetValue( "D65"  );

}