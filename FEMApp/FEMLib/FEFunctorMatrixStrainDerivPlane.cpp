
#include "FEFunctorMatrixStrainDerivPlane.h"
#include "DOFs.h"

CFEFunctorMatrixStrainDerivPlane::CFEFunctorMatrixStrainDerivPlane( CFEFunctorMatrix *dn ):CFEFunctorMatrixColDOF("CFEFunctorMatrixStrainDerivPlane",true),dSF( dn )
{
	CDOFVector tdv;
	tdv.push_back( &CDOFs::Ux );
	tdv.push_back( &CDOFs::Uy );
	alc.Init( tdv );
	AddNode( dSF );
	rows_names.push_back("exx");
	rows_names.push_back("eyy");
	rows_names.push_back("gxy");
}

CFEFunctorMatrixStrainDerivPlane::CFEFunctorMatrixStrainDerivPlane( const CFEFunctorMatrixStrainDerivPlane &mf ):CFEFunctorMatrixColDOF(mf)
{
	CLONE_MEMBER( mf, dSF )
}

CFEFunctorMatrixStrainDerivPlane::~CFEFunctorMatrixStrainDerivPlane(void)
{

}

void CFEFunctorMatrixStrainDerivPlane::Compute( CFiniteElementInstance *fe1 )
{
    unsigned dim = fe1->GetShapeFunctions().GetXdim(), k;
	matrix &B = fv, &dN = (*dSF);
	fv.SetDim( 3, dim * 2 );
	for (k=0; k<dim; k++)
	{
		B( 0, 2 * k     ) = dN( k, 0 );
		B( 1, 2 * k + 1 ) = dN( k, 1 );
		B( 2, 2 * k     ) = dN( k, 1 );   B( 2, 2 * k + 1 ) = dN( k, 0 );
	}
}

