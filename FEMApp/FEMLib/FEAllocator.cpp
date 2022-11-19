#include "FEAllocator.h"


CFEAllocator::CFEAllocator( const CDOFVector &dvt ):ntypes( dvt ),last_fe(0)
{
	
}

CFEAllocator::CFEAllocator( CDOF* tp[], unsigned n ):last_fe(0)
{
	unsigned k;
		for (k=0; k<n; k++) 
			ntypes.push_back( tp[ k ] );
}

CFEAllocator::~CFEAllocator(void)
{
}




