/* 
 * File:   CMeshGeneratorRectangular3D.cpp
 * Author: piotrek
 * 
 * Created on November 21, 2011, 12:46 PM
 */

#include "MeshGeneratorRectangular3D.h"
#include "../MathLib/Scaler.h"

CMeshGeneratorRectangular3D::CMeshGeneratorRectangular3D( const CSpaceShape& s, unsigned x, unsigned y, unsigned z ):sfe(s),nx(x),ny(y),nz(z),degree( s.GetDegree( ) )  {
}

CMeshGeneratorRectangular3D::~CMeshGeneratorRectangular3D() {
}

bool CMeshGeneratorRectangular3D::Generate()
{
  unsigned k, l, m,n, dimx = nx * degree + 1, dimy = ny * degree + 1,  dimz = nz * degree + 1;
    nnode = 0;
    mesh.resize( dimx, vector< vector< int > >( dimy, vector<int>( dimz, -1.0 ) )  );
    
/*    for (k=0; k<dimx; k++)
    {
        mesh[ k ].resize( dimy );
        for (l=0; l<dimy; l++)
        {
            mesh[ k ][ l ].resize( dimz );
            for (m=0; m<dimz; m++)
                   mesh[ k ][ l ][ m ] = -1;
        }
    } */
    
    const vector< mvector > &ln = sfe.GetLocalNodes();
    double dxi = 2.0 / double(degree);
    vector<unsigned> offX, offY, offZ;
    
    for (k=0;k<ln.size();k++)
    {
        for (l=0;l<degree+1; l++)
            for (m=0;m<degree+1; m++)
                for (n=0;n<degree+1; n++)
                if ( 
                        fabs( ln[ k ][ 0 ] - ( l * dxi - 1.0 ) ) < 1e-6 &&
                        fabs( ln[ k ][ 1 ] - ( m * dxi - 1.0 ) ) < 1e-6 &&
                        fabs( ln[ k ][ 2 ] - ( n * dxi - 1.0 ) ) < 1e-6
                    )
                { 
                    offX.push_back( l );  
                    offY.push_back( m ); 
                    offZ.push_back( n ); 
                }
    }
    
    elements.resize( nx * ny * nz );
    unsigned el_ind = 0;
    for ( k = 0; k<nx; k++ )
        for ( l = 0; l<ny; l++ )
            for ( n = 0; n<nz; n++ )
            {
                for ( m = 0; m < ln.size(); m++ )
                {
                        int &el = mesh[ k * degree + offX[ m ] ][ l * degree + offY[ m ] ][ n * degree + offZ[ m ] ];
                        if ( el == -1 ) el = nnode++;
                        elements[ el_ind ].push_back( el );
                }
                el_ind++;
            }
   nelems = elements.size();   
}

void CMeshGeneratorRectangular3D::GenerateRectangularArea( double x1, double y1,double z1, double x2, double y2, double z2 )
{
    unsigned k, l, m, dimx = nx * degree + 1, dimy = ny * degree + 1, dimz = nz * degree + 1;
    
    CScaler sx(0,dimx-1,x1,x2);
    CScaler sy(0,dimy-1,y1,y2);
    CScaler sz(0,dimz-1,z1,z2);
   
    points.resize( nnode );
    for ( k = 0; k < dimx; k++ )
        for ( l = 0; l < dimy; l++ )
            for ( m = 0; m < dimz; m++ )
                if ( mesh[ k ][ l ][ m ] != -1 ) 
                {
                        mvector x( 3 );
                        x[ 0 ] = sx.scale( k );
                        x[ 1 ] = sy.scale( l );
                        x[ 2 ] = sz.scale( m );
                        points[ mesh[ k ][ l ][ m ] ] = x;
                }

}


void CMeshGeneratorRectangular3D::GenerateShapeArea( const CSpace *s, const vector< mvector > &xp )
{
    unsigned k, l, m, j, dimx = nx * degree + 1, dimy = ny * degree + 1, dimz = nz * degree + 1;

    CScaler sx(0,dimx-1,-1.0,1.0);
    CScaler sy(0,dimy-1,-1.0,1.0);
    CScaler sz(0,dimz-1,-1.0,1.0);

    points.resize( nnode );
    for ( k = 0; k < dimx; k++ )
        for ( l = 0; l < dimy; l++ )
            for ( m = 0; m < dimz; m++ )
                if ( mesh[ k ][ l ][ m ] != -1 )
                {
                        mvector x( 3 ),X( 3 ),N( xp.size() );
                        x[ 0 ] = sx.scale( k );
                        x[ 1 ] = sy.scale( l );
                        x[ 2 ] = sz.scale( m );
                        s->Transform( x, N );
                        for (j=0; j<N.GetDim(); j++)
                        {
                            X[0] +=  N[j] * xp[j][0];
                            X[1] +=  N[j] * xp[j][1];
                            X[2] +=  N[j] * xp[j][2];
                        }

                        points[ mesh[ k ][ l ][ m ] ] = X;
                }
}
