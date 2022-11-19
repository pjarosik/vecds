
#include "FEFunctorVectorColExtractor.h"

CFEFunctorVectorColExtractor::CFEFunctorVectorColExtractor( CFEFunctorMatrix *m, unsigned c, const string &nm ):CFEFunctorVector( nm ),M(m),col(c)
{
	AddNode( M );
	mnames = m->GetRowsNames();
}

CFEFunctorVectorColExtractor::CFEFunctorVectorColExtractor( const CFEFunctorVectorColExtractor &vf ):CFEFunctorVector(vf),col(vf.col)
{
	CLONE_MEMBER( vf, M )
}

CFEFunctorVectorColExtractor::~CFEFunctorVectorColExtractor( void )
{

}

void CFEFunctorVectorColExtractor::Compute( CFiniteElementInstance *fe1 )
{
	const matrix &m=(*M);
	m.ExtractColumn( col, fv );
}
