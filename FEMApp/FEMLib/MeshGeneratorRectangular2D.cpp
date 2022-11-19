/*
 * File:   CMeshGeneratorRectangular2D.cpp
 * Author: piotrek
 * 
 * Created on November 21, 2011, 12:45 PM
 */

#include "MeshGeneratorRectangular2D.h"
#include "../MathLib/Scaler.h"

CMeshGeneratorRectangular2D::CMeshGeneratorRectangular2D( const CSpaceShape& s, unsigned x, unsigned y, unsigned g ):sfe(s),nx(x),ny(y),degree( s.GetDegree( ) ), ground(g)
{

}


CMeshGeneratorRectangular2D::~CMeshGeneratorRectangular2D() {
}

bool CMeshGeneratorRectangular2D::Generate()
{
    unsigned k, l, m, dimx = nx * degree + 1, dimy = ny * degree + 1;
    nnode = 0;
    mesh.resize( dimx, vector< int >( dimy, -1 ) );
    
    const vector< mvector > &ln = sfe.GetLocalNodes();
    double dxi = 2.0 / double( degree );
    vector<unsigned> offX, offY, offXe, offYe;


    for (k=0;k<ln.size();k++)
    {
        for (l=0;l<degree+1;l++)
            for (m=0;m<degree+1;m++)
                if ( 
                        fabs( ln[ k ][ 0 ] - ( l * dxi - 1.0 ) ) < 1e-6 &&
                        fabs( ln[ k ][ 1 ] - ( m * dxi - 1.0 ) ) < 1e-6
                    )
                { 
                    offX.push_back( l );  
                    offY.push_back( m ); 
                }
    }
    
    elements.resize( nx * ny );
    for ( k = 0; k<nx; k++ )
        for ( l = 0; l<ny; l++ )
            for ( m = 0; m < ln.size(); m++ )
            {
                int &el = mesh[ k * degree + offX[ m ] ][ l * degree + offY[ m ] ];
                if ( el == -1 ) el = nnode++;
                elements[ k + l * nx ].push_back( el );
            }
   nelems = elements.size(); 
}

void CMeshGeneratorRectangular2D::GenerateRectangularArea( double x1, double y1, double x2, double y2 )
{
    unsigned k, l, m, dimx = nx * degree + 1, dimy = ny * degree + 1;
    
    CScaler sx(0,dimx-1,x1,x2);
    CScaler sy(0,dimy-1,y1,y2);
   
    points.resize( nnode );
    for ( k = 0; k < dimx; k++ )
        for ( l = 0; l < dimy; l++ )
                if ( mesh[ k ][ l ] != -1 ) 
                {
                        mvector x( 2 );
                        x[ 0 ] = sx.scale( k );
                        x[ 1 ] = sy.scale( l );
                        points[ mesh[ k ][ l ] ] = x;
                }

}
