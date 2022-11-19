
#include "FEFunctorVectorRowExtractor.h"

CFEFunctorVectorRowExtractor::CFEFunctorVectorRowExtractor( CFEFunctorMatrix *m, unsigned r, const string &nm ):CFEFunctorVector( nm ),M(m),row(r)
{
	AddNode( M );
}

CFEFunctorVectorRowExtractor::CFEFunctorVectorRowExtractor( const CFEFunctorVectorRowExtractor &vf ):CFEFunctorVector(vf),row(vf.row)
{
	CLONE_MEMBER( vf, M )
}

CFEFunctorVectorRowExtractor::~CFEFunctorVectorRowExtractor( void )
{

}

void CFEFunctorVectorRowExtractor::Compute( CFiniteElementInstance *fe1  )
{
	const matrix &m = (*M);
	m.ExtractRow( row, fv );
}
