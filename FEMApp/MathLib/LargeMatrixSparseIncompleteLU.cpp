#include "LargeMatrixSparseIncompleteLU.h"

CLargeMatrixSparseIncompleteLU::CLargeMatrixSparseIncompleteLU():CLargeMatrixSparseCR( false )
{
}

bool CLargeMatrixSparseIncompleteLU::Decompose(CProgress *pr )
{

    unsigned r,i,j,m,n;
    double d,e;
    for ( r=0; r<dim; r++)
    {
        d = 1.0 / data[ diag_raw[ r ] ];
        for ( i = r + 1; i < dim; i++ )
        {
            for (m=blines_raw[i]; m<blines_raw[i+1]; m++)
                if ( cols_raw[ m ] == i )
                {
                    e = d * data[ m ];
                    data[ m ] = e;
                    for (j=blines_raw[ i ]; j<blines_raw[i+1]; j++)
                    {
                        for ( n = blines_raw[r]; n<blines_raw[r+1]; n++ )
                            if ( cols_raw[ j ] > r && cols_raw[ n ] == j )
                                data[ j ] = data[ j ] - e * data[ n ];
                    }
                }
        }
    }

}

bool CLargeMatrixSparseIncompleteLU::Solve( const mvector &b, mvector &x )
{
}
