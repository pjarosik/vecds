
#include "SpaceShapeLagrange.h"

CSpaceShapeLagrange::CSpaceShapeLagrange( unsigned dim, unsigned d ):CSpaceShape( ST_RECTANGLE, dim, upow(d,dim) )
{
    dg = d;
    nodes.resize(upow(dg,dim));
	double dx = 2.0 / ( dg - 1 );
	vector<unsigned> ix( dim );
	unsigned k,l;
    for ( l = 0; l < dim; l++ ) ix[ l ] = 0;
    for ( k = 0; k < nodes.size( ); k++ )
	{
        nodes[k].SetDim(dim);
        for ( l=0; l<dim; l++ ) nodes[ k ][ l ] = -1.0 + dx * ix[ l ];
		l=0;
		while( l < dim && ++ix[ l ] >= dg ) ix[ l++ ] = 0;
	}



}

 CSpaceShapeLagrange::~CSpaceShapeLagrange( void )
{

}

