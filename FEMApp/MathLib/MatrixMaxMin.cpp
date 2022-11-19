
#include "MatrixMaxMin.h"

template< class M >
CMatrixMaxMin<M>::CMatrixMaxMin(unsigned r, unsigned c, const M &m ):rows( r ), cols( c )
{
	reset(m);
}

template< class M >
CMatrixMaxMin<M>::~CMatrixMaxMin(void)
{
}

template< class M >
void CMatrixMaxMin<M>::reset(const M &m)
{
	max = -1e300;
	min =  1e300;
}

template< class M >
void CMatrixMaxMin<M>::Update( const M &m )
{
	unsigned k,l;
	for (k=0; k<rows; k++)
		for (l=0; l<cols; l++)
		{
			if ( m( k, l ) < min( k, l ) ) min( k, l ) = m( k, l );
			if ( m( k, l ) > max( k, l ) ) max( k, l ) = m( k, l );
		}
}