// NCounter.cpp: implementation of the CNCounter class.
//
//////////////////////////////////////////////////////////////////////

#include "NCounter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNCounter::CNCounter( unsigned d, unsigned e ):dim( d ), size( 1 ), index(0)
{
    int k;
    degree.resize( dim );
    for ( k = 0; k < dim; k++ )
    {
        degree[ k ] = e;
        size *= e;
    }
}

CNCounter::CNCounter( unsigned d, const vector< unsigned > &e ):dim( d ), size( 1 ), index(0)
{
    int k;
    degree.resize( dim );
    for ( k = 0; k < dim; k++ )
    {
        degree[ k ] = e[ k ];
        size *= e[ k ];
    }
}

CNCounter::~CNCounter( )
{

}

void CNCounter::Reset()
{
    int k;
    i.resize( dim );
    for ( k = 0; k < dim; k++ ) i[ k ] = 0;
    index=0;
}

bool CNCounter::operator ++( int )
{
    index++;
	int id = 0;
	while( id < dim )
	{
		i[ id ]++;
        if ( i[ id ] < degree[ id ] ) return true;
		i[ id++ ] = 0;
	}
	return false;
}
