
#include "FEFunctorMatrixStrainDerivSolid.h"
#include "DOFs.h"

CFEFunctorMatrixStrainDerivSolid::CFEFunctorMatrixStrainDerivSolid( CFEFunctorMatrix *mf ):CFEFunctorMatrixColDOF("CFEFunctorMatrixStrainDerivSolid",true),dSF(mf)
{
	CDOFVector tdv;
	tdv.push_back( &CDOFs::Ux );
	tdv.push_back( &CDOFs::Uy );
	tdv.push_back( &CDOFs::Uz );

	alc.Init( tdv );

	AddNode( dSF );

	rows_names.push_back("exx");
	rows_names.push_back("eyy");
	rows_names.push_back("ezz");
	rows_names.push_back("gxy");
	rows_names.push_back("gzy");
	rows_names.push_back("gzx");

}

CFEFunctorMatrixStrainDerivSolid::CFEFunctorMatrixStrainDerivSolid( const CFEFunctorMatrixStrainDerivSolid &mf ):CFEFunctorMatrixColDOF(mf)
{
	CLONE_MEMBER( mf, dSF )
}

CFEFunctorMatrixStrainDerivSolid::~CFEFunctorMatrixStrainDerivSolid( void )
{
}


void CFEFunctorMatrixStrainDerivSolid::Compute( CFiniteElementInstance *fe1 )
{
    unsigned dim = fe1->GetShapeFunctions().GetXdim(), k;
	matrix &B = fv, &dN = *dSF;
	B.SetDim( 6, dim * 3 );
	for (k=0; k<dim; k++)
	{
		B( 0, 3 * k     ) = dN( k, 0 );
		B( 1, 3 * k + 1 ) = dN( k, 1 );
		B( 2, 3 * k + 2 ) = dN( k, 2 );

		B( 3, 3 * k     ) = dN( k, 1 );  B( 3, 3 * k + 1 ) = dN( k, 0 );
		B( 4, 3 * k + 1 ) = dN( k, 2 );  B( 4, 3 * k + 2 ) = dN( k, 1 );
		B( 5, 3 * k     ) = dN( k, 2 );  B( 5, 3 * k + 2 ) = dN( k, 0 );
	}
}

