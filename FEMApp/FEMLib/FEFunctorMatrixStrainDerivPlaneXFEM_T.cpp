
#include "FEFunctorMatrixStrainDerivPlaneXFEM_T.h"
#include "DOFs.h"

CFEFunctorMatrixStrainDerivPlaneXFEM_T::CFEFunctorMatrixStrainDerivPlaneXFEM_T( CEnrichmentFunctions *e, CFEFunctorVector *vf1, CFEFunctorVector *vf2, CFEFunctorMatrix *mf1, CFEFunctorMatrix *mf2 ):CFEFunctorMatrixColDOF( "CFEFunctorMatrixStrainDerivPlaneXFEM_T", false ),N(vf1),T(vf2),dN(mf1),dT(mf2),ef( e )
{
	CDOFVector tdv;
	unsigned l;
	if ( e )
	{
		for (l=0; l<e->GetN(); l++)
		{
			tdv.push_back( new CDOF_Bx( 2 * l ) );
			tdv.push_back( new CDOF_By( 2 * l ) );
		}
	}

	alc.Init( tdv );

	AddNode( N );
	AddNode( T );
	AddNode( dN );
	AddNode( dT );

	rows_names.push_back("exx");
	rows_names.push_back("eyy");
	rows_names.push_back("gxx");

}

CFEFunctorMatrixStrainDerivPlaneXFEM_T::CFEFunctorMatrixStrainDerivPlaneXFEM_T( const CFEFunctorMatrixStrainDerivPlaneXFEM_T &mf ):CFEFunctorMatrixColDOF(mf)
{
	AddNode( N = mf.N->Clone() );
	AddNode( T = mf.T->Clone() );
	AddNode( dN = mf.dN->Clone() );
	AddNode( dT = mf.dT->Clone() );
}

CFEFunctorMatrixStrainDerivPlaneXFEM_T::~CFEFunctorMatrixStrainDerivPlaneXFEM_T( void )
{

}

void CFEFunctorMatrixStrainDerivPlaneXFEM_T::Compute( CFiniteElementInstance *fe1)
{
	vector<unsigned> inds;
	unsigned k,i=0;

	for (k=0; k<fe1->GetNodes().size(); k++)
		if ( fe1->GetElemDofs()[k].FindDOF( &CDOFs::Bx[0], i ) ) inds.push_back( k );

	unsigned dim = inds.size(), ind=0;
	matrix &B = fv;

	matrix  &dT1= (*dT),&dN1 = (*dN);
	const mvector &T1 = (*T), &N1=(*N);

	fv.SetDim( 3, dim * 2 * T->GetDim() );

	for (k=0; k<dim; k++)
	{
		ind = inds[ k ];
		for (unsigned l=0; l<ef->GetN(); l++)
		{
			B( 0, 2 * i     ) = T1[ l ] * dN1( ind, 0 ) + dT1( l, 0 ) * N1[ ind ];
			B( 1, 2 * i + 1 ) = T1[ l ] * dN1( ind, 1 ) + dT1( l, 1 ) * N1[ ind ];

			B( 2, 2 * i     ) = T1[ l ] * dN1( ind, 1 ) + dT1( l, 1 ) * N1[ ind ];   B( 2, 2 * i + 1 ) = T1[ l ] * dN1( ind,  0 ) + dT1( l, 0 ) * N1[ ind ];
			i++;
		}
	}
}
