
#include "FEFunctorMatrixTabElemVectors.h"

CFEFunctorMatrixTabElemVectors::CFEFunctorMatrixTabElemVectors(  const vector< mvector > &p, CFEFunctorVector *x, const string &name  ):CFEFunctorMatrix( name ),
																		xi(x), pts( p )
{
	AddNode( xi );
}

CFEFunctorMatrixTabElemVectors::~CFEFunctorMatrixTabElemVectors( void )
{

}

CFEFunctorMatrixTabElemVectors::CFEFunctorMatrixTabElemVectors( const CFEFunctorMatrixTabElemVectors &mf ):CFEFunctorMatrix( mf ), vfs( mf.vfs.size() )
{
	CLONE_MEMBER( mf, xi )
	unsigned k;
	for (k=0; k<vfs.size(); k++)
		CLONE_MEMBER( mf, vfs[k] )
}

CFEFunctorMatrixTabElemVectors* CFEFunctorMatrixTabElemVectors::operator<<( CFEFunctorVector *vf )
{
	vfs.push_back( vf );
	AddNode( vf );
	unsigned k;
	for (k=0; k<vf->GetNames().size(); k++)
		cols_names.push_back( vf->GetNames()[ k ] );
	return this;
}

void CFEFunctorMatrixTabElemVectors::Compute(CFiniteElementInstance *fe1)
{
	unsigned k,l,m,i;
	matrix &mf = *this;
	mf.SetDim( pts.size(), cols_names.size() );
	for (k=0; k<pts.size( ); k++)
	{
		*xi = pts[ k ];
		i=0;
		for (l=0; l<vfs.size( ); l++)
		{
			for ( m = 0; m < vfs[l]->GetDim(); m++ )
				mf( k, i++ ) = (*vfs[l])[m];
		}
	}
}
