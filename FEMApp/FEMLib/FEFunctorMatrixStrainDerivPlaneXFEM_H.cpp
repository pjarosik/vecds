
#include "FEFunctorMatrixStrainDerivPlaneXFEM_H.h"
#include "DOFs.h"

CFEFunctorMatrixStrainDerivPlaneXFEM_H::CFEFunctorMatrixStrainDerivPlaneXFEM_H( CCrack *c, CFEFunctorMatrix *mf, CFEFunctorVector *vf ):CFEFunctorMatrixColDOF( "CFEFunctorMatrixStrainDerivPlaneXFEM_H", false ), dSF( mf ), X( vf ), crack( c )
{

	CDOFVector tdv;
	tdv.Add( &CDOFs::Ax );
	tdv.Add( &CDOFs::Ay );

	alc.Init( tdv );

	AddNode( dSF );
	AddNode( X );

	rows_names.push_back("exx");
	rows_names.push_back("eyy");
	rows_names.push_back("gxx");

}

CFEFunctorMatrixStrainDerivPlaneXFEM_H::CFEFunctorMatrixStrainDerivPlaneXFEM_H( const CFEFunctorMatrixStrainDerivPlaneXFEM_H &mf ):CFEFunctorMatrixColDOF(mf)
{
	CLONE_MEMBER( mf, dSF );
	CLONE_MEMBER( mf, X );
}

CFEFunctorMatrixStrainDerivPlaneXFEM_H::~CFEFunctorMatrixStrainDerivPlaneXFEM_H( void )
{
	
}

void CFEFunctorMatrixStrainDerivPlaneXFEM_H::Compute( CFiniteElementInstance *fe1 )
{
	unsigned k,i=0;
	vector<unsigned> inds;
	for (k=0; k<fe1->GetNodes().size(); k++)
		if ( fe1->GetElemDofs()[ k ].FindDOF( &CDOFs::Ax, i ) ) inds.push_back( k );
	unsigned dim = inds.size() , ind;
	matrix &B = *this, &dN = *dSF;
	B.SetDim( 3, dim * 2 );
	for (k=0;  k<dim; k++)
	{
		ind = inds[ k ];
		B( 0, 2 * k     ) = crack->H( (*X) ) * dN( ind, 0 );
		B( 1, 2 * k + 1 ) = crack->H( (*X) ) * dN( ind, 1 );
		B( 2, 2 * k     ) = crack->H( (*X) ) * dN( ind, 1 );   B( 2, 2 * k + 1 ) = crack->H( (*X) ) * dN( ind, 0 );
	}
}
