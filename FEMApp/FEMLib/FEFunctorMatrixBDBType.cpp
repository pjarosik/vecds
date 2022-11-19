
#include "FEFunctorMatrixBDBType.h"

CFEFunctorMatrixBDBType::CFEFunctorMatrixBDBType( CFEFunctorMatrixColDOF *b1, CFEFunctorMatrix *d, CFEFunctorMatrixColDOF *b2 ):CFEFunctorMatrix("CFEFunctorMatrixBDBType"),B1( b1 ), B2( b2 ), D( d )
{
	AddNode( B1 );
	AddNode( D );
	AddNode( B2 );
	alr.Init( B1->GetColAllocator() );
	alc.Init( B2->GetColAllocator() );
}

CFEFunctorMatrixBDBType::~CFEFunctorMatrixBDBType( void )
{

}

CFEFunctorMatrixBDBType::CFEFunctorMatrixBDBType( const CFEFunctorMatrixBDBType &mf ):CFEFunctorMatrix( mf )
{

	AddNode( D = mf.D->Clone() );
	AddNode( B1 = mf.B1->Clone() );
	AddNode( B2 = mf.B2->Clone() );

}


void CFEFunctorMatrixBDBType::Compute( CFiniteElementInstance *fe1 )
{
    fv =  (~(*B1)) * (*D) * (*B2);
}

