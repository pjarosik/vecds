
#include "FEFunctorVectorElemDOFs.h"

//unsigned a[], unsigned n,


CFEFunctorVectorElemDOFs::CFEFunctorVectorElemDOFs( CDOF* tp[], unsigned n, const string &nm ):CFEFunctorVector( nm, tp, n )
{
	
		
}

CFEFunctorVectorElemDOFs::~CFEFunctorVectorElemDOFs( void )
{
	
}

void CFEFunctorVectorElemDOFs::Compute( CFiniteElementInstance *fe1 ) 
{
	fv.SetDim( al.GetDOFs().size() );
	unsigned k;
	for ( k = 0; k < al.GetDOFs().size(); k++ )
		fv[ k ] = al.GetDOFs()[ k ]->GetSolution( );
}