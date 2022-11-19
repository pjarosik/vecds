/* 
 * File:   CLargeMatrixSkyLineCholesky2.cpp
 * Author: piotrek
 * 
 * Created on December 4, 2011, 10:48 PM
 */

#include "LargeMatrixSkyLineCholesky2.h"
#include <iostream>

CLargeMatrixSkyLineCholesky2::CLargeMatrixSkyLineCholesky2() {
}

CLargeMatrixSkyLineCholesky2::~CLargeMatrixSkyLineCholesky2() {
}

bool CLargeMatrixSkyLineCholesky2::Decompose( CProgress *prg )
{
        int i, i1, m, k, pr;
        double  r;

        if ( prg ) prg->SetRange( dim );
        dec_time.Start();
        for (k=0; k<(int)dim; k++)
        {
                r=0.0;
                for (i=gprf[k]; i<k; i++) r +=  mat[ k ][ k - i ] * mat[ k ][ k - i ];   // L(k,i) * L(k,i);
                if ( mat[k][0] - r < 0.0 )  return false;
                mat[k][0] =  sqrt( mat[k][0] - r );
                if ( mat[k][0] == 0.0 )  return false;
                       
                for (i=k+1; i<=cprf[k]; i++)
                {
                    if ( k >= gprf[i] )
                    {
                        r = 0.0;
                        pr =  gprf[i] > gprf[k] ? gprf[i] : gprf[k];
                        for (m=pr; m<k; m++)
                                r += mat[i][i-m] * mat[k][k-m]; // L(i,m) * L(k,m);
                        mat[i][i-k] = ( mat[i][i-k] - r ) / mat[k][0];  // L(i,k) = ( L(i,k)  - r ) / L(k,k);
                    }
                }
                if (prg) prg->NewValue(k);
        };

        dec_time.Stop();
        cout << " decomposition time =" << dec_time.GetFormattedTime() << endl;

        return true;
}

