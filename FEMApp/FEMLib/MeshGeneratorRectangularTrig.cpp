#include "MeshGeneratorRectangularTrig.h"

CMeshGeneratorRectangularTrig::CMeshGeneratorRectangularTrig( const CSpaceShape& sf, unsigned x, unsigned y, TMPattern pt ):CMeshGeneratorRectangular2D( sf, x, y, 0 ), pattern( pt )
{
    degree = 1;
    if ( pattern == TM_QUAD ) degree = 2;
}


bool CMeshGeneratorRectangularTrig::Generate( )
{
    unsigned k, l, dimx = nx * degree + 1, dimy = ny * degree + 1, elind = 0;
    nnode = 0;
    mesh.resize( dimx, vector< int >( dimy, -1 ) );

    switch (pattern ) {
    case TM_DOUBLE:
        elements.resize( 2 * nx * ny );
        for ( k = 0; k<nx; k++ )
            for ( l = 0; l<ny; l++ )
            {
                PutNode( k   , l+1, nnode ); elements[ elind   ].push_back( mesh[ k   ][ l+1 ] );
                PutNode( k   , l  , nnode ); elements[ elind   ].push_back( mesh[ k   ][ l   ] );
                PutNode( k+1 , l  , nnode ); elements[ elind++ ].push_back( mesh[ k+1 ][ l   ] );

                PutNode( k   , l+1, nnode ); elements[ elind   ].push_back( mesh[ k   ][ l+1 ] );
                PutNode( k+1 , l  , nnode ); elements[ elind   ].push_back( mesh[ k+1 ][ l   ] );
                PutNode( k+1 , l+1, nnode ); elements[ elind++ ].push_back( mesh[ k+1 ][ l+1 ] );

            }
        break;
    case TM_DOUBLE2:
        elements.resize( 2 * nx * ny );
        for ( k = 0; k<nx; k++ )
            for ( l = 0; l<ny; l++ )
            {


                PutNode( k   , l  , nnode ); elements[ elind   ].push_back( mesh[ k   ][ l   ] );
                PutNode( k+1 , l  , nnode ); elements[ elind   ].push_back( mesh[ k+1 ][ l   ] );
                PutNode( k+1 , l+1, nnode ); elements[ elind++ ].push_back( mesh[ k+1 ][ l+1 ] );

                PutNode( k   , l,   nnode ); elements[ elind   ].push_back( mesh[ k   ][ l   ] );
                PutNode( k+1 , l+1, nnode ); elements[ elind   ].push_back( mesh[ k+1 ][ l+1 ] );
                PutNode( k   , l+1, nnode ); elements[ elind++ ].push_back( mesh[ k   ][ l+1 ] );


            }
        break;
    case TM_QUAD:
        elements.resize( 4 * nx * ny );
        for ( k = 0; k<nx; k++ )
            for ( l = 0; l<ny; l++ )
            {
                int ks = 2*k+1, ls=2*l+1, kb=2*k, ke=2*(k+1), lb=2*l, le=2*(l+1);

                PutNode( kb, lb , nnode ); elements[ elind   ].push_back( mesh[ kb ][ lb ] );
                PutNode( ke, lb , nnode ); elements[ elind   ].push_back( mesh[ ke ][ lb ] );
                PutNode( ks, ls , nnode ); elements[ elind++ ].push_back( mesh[ ks ][ ls ] );

                PutNode( ke, lb , nnode ); elements[ elind   ].push_back( mesh[ ke ][ lb ] );
                PutNode( ke, le , nnode ); elements[ elind   ].push_back( mesh[ ke ][ le ] );
                PutNode( ks, ls , nnode ); elements[ elind++ ].push_back( mesh[ ks ][ ls ] );

                PutNode( ke, le , nnode ); elements[ elind   ].push_back( mesh[ ke ][ le ] );
                PutNode( kb, le , nnode ); elements[ elind   ].push_back( mesh[ kb ][ le ] );
                PutNode( ks, ls , nnode ); elements[ elind++ ].push_back( mesh[ ks ][ ls ] );

                PutNode( kb, le , nnode ); elements[ elind   ].push_back( mesh[ kb ][ le ] );
                PutNode( kb, lb , nnode ); elements[ elind   ].push_back( mesh[ kb ][ lb ] );
                PutNode( ks, ls , nnode ); elements[ elind++ ].push_back( mesh[ ks ][ ls ] );

            }

    }

    nelems = elements.size();
    return true;
}
