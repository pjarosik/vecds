
#include "VectorMaxMin.h"

template< class V >
CVectorMaxMin<V>::CVectorMaxMin( unsigned l, const V &m ):len( l )
{
	reset( l );
}

template< class V >
CVectorMaxMin<V>::~CVectorMaxMin(void)
{
}

template< class V >
void CVectorMaxMin<V>::reset(const V &v)
{
	vmax = v;
	vmin = v;
	vmax = -1e300;
	vmin =  1e300;
}

template< class V >
void CVectorMaxMin<V>::Update( const V &v )
{
	unsigned k,l;
	for (k=0; k<len; k++)
	{
		if ( v[ k ] < vmin[ k ] ) vmin[ k] = v[ k ];
		if ( v[ k ] > vmax[ k ] ) vmax[ k] = v[ k ];
	}
}