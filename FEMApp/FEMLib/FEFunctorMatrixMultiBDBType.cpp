
#include "FEFunctorMatrixMultiBDBType.h"

CFEFunctorMatrixMultiBDBType::CFEFunctorMatrixMultiBDBType( const vector< CFEFunctorMatrixColDOF* > &b, CFEFunctorMatrix *d ):CFEFunctorMatrix("CFEFunctorMatrixMultiBDBType"), dim( b.size( ) ), alv( b.size() ), D( d ), B( b )
{
	unsigned k;
	AddNode( d );
	for (k=0; k<dim; k++)
	{
		AddNode( b[ k ] );
		alr.Merge( b[ k ]->GetColAllocator() );
	}
	alc = alr;
}


CFEFunctorMatrixMultiBDBType::CFEFunctorMatrixMultiBDBType( const CFEFunctorMatrixMultiBDBType &mmf ):CFEFunctorMatrix( mmf ), dim( mmf.dim ),D(mmf.D),B( mmf.dim )
{
	unsigned k;
	CLONE_MEMBER( mmf, D )
	for ( k=0; k<dim; k++ )
		CLONE_MEMBER( mmf, B[ k ] )

}

CFEFunctorMatrixMultiBDBType::~CFEFunctorMatrixMultiBDBType( void )
{

}


void CFEFunctorMatrixMultiBDBType::Compute( CFiniteElementInstance *fe1 )
{
	unsigned k, l, i, j;
	matrix ke;
    
	fv.SetDim( alr.GetDim(), alc.GetDim() );
	alr.CollectDOFs( fe1 );
	alc = alr;

	for ( k = 0; k < B.size(); k++ )
	{
		B[ k ]->ManageDOFs( fe1 );
		B[ k ]->GetColAllocator().GetAllocationVector( alc.GetDOFs(), alv[ k ] );
	}


	for ( k = 0; k < B.size(); k++ )
		for ( l = k; l < B.size(); l++ )
		{
			if ( B[ k ]->GetDOFVector( ).size() &&  B[ l ]->GetDOFVector( ).size() )
			{
				ke = ( ~(*B[k])) * (*D) * (*B[l] );
				for (i=0; i<alv[k].size(); i++)
					for (j=0; j<alv[l].size(); j++)
						fv( alv[ k ][ i ], alv[ l ] [ j ] ) = ke( i, j );

				if ( k != l )
					for (i=0; i<alv[k].size(); i++)
						for (j=0; j<alv[l].size(); j++)
							fv( alv[ l ][ j ], alv[ k ][ i ] ) = ke( i, j );

			}
		}
}

