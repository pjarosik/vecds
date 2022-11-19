
#include "FEFunctorVector.h"


CFEFunctorVector::CFEFunctorVector( const string &nm ):CFEFunctor< CFiniteElementInstance >( nm )
{

}

CFEFunctorVector::CFEFunctorVector( const string &nm, CDOF* tp[], unsigned n ):CFEFunctor< CFiniteElementInstance >( nm ),al( tp, n )
{
	
}

CFEFunctorVector::~CFEFunctorVector(void)
{
}


void CFEFunctorVector::DebugPrint( ) const
{
	fv.print( objectname );
}


mvector& operator << ( mvector &L, const CFEFunctorVector &vf )
{
	unsigned k;
	valarray< unsigned > al( vf.GetDim() );
	vf.GetAllocationVector( al );
	for (k=0; k< vf.GetDim(); k++)
		L[ al[ k ] ] += vf[ k ];
	return L;
}

void CFEFunctorVector::CreateDOFs( CFiniteElementInstance *p )  
{ 
            al.CreateDOFs( p ); 
}