// LargeMatrixAsymmetricalCrout.cpp: implementation of the CLargeMatrixAsymmetricalCrout class.
//
//////////////////////////////////////////////////////////////////////

#include "LargeMatrixAsymmetricalCrout.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLargeMatrixAsymmetricalCrout::CLargeMatrixAsymmetricalCrout()
{

}

CLargeMatrixAsymmetricalCrout::~CLargeMatrixAsymmetricalCrout()
{

}

bool CLargeMatrixAsymmetricalCrout::Decompose(CProgress *pr)
{

    int k,j,i,m;
    double v;
	if ( pr ) pr->SetRange( dim );
    dec_time.Start();
    for (k=0; k<(int)dim; k++)
    {
        v = 0.0;
        for ( m = max( Lprf[k], Uprf[k] ); m < k; m++ )
            v += L[k][k-m]*U[k][k-m]; //Lr(k,m) * Up(m,k);
        
        Lr(k,k) = Up(k,k) - v;
        
        if ( Lr(k,k) == 0 ) return false;
           
        for ( j=k; j<(int)dim; j++ )
        {
            if (k >= (int)Uprf[j] )
            {
                v = 0.0;
                for ( m = max( Lprf[k],Uprf[j]); m < k; m++ )
                        v += L[k][k-m]*U[j][j-m]; //Lr(k,m) * Up(m,j);
                U[j][j-k] = ( U[j][j-k] - v ) / L[k][0]; //Up( k, j ) = ( Up( k, j ) - v ) / Lr( k, k );
            }
        }
        
        if ( Up(k,k) == 0 ) return false;
        
        for ( i = k + 1; i < (int)dim; i++ )
        {
                if ( k >= (int)Lprf[ i ] )
                {
                        v=0.0;
                        for ( m = max( Lprf[ i ], Uprf[ k ] ); m < k; m++ )
                                v += L[i][i-m]*U[k][k-m]; //Lr( i, m ) * Up( m, k );
                        L[i][i-k] = ( L[i][i-k] - v ) / U[k][0]; //Lr( i, k ) = ( Lr( i, k ) - v ) / Up( k, k );
                }
        }
        
        if ( pr ) pr->NewValue( k );
    }
    
        dec_time.Stop();
        cout << "solution time =" << dec_time.GetFormattedTime() << endl;
    
    decomposed = true;
    return true;
}


void CLargeMatrixAsymmetricalCrout::Read( const char *nm )
{
    ifstream fin(nm);
    fin.close();
}

void CLargeMatrixAsymmetricalCrout::Save( const char *nm )
{
    ofstream fin(nm);
    fin.close();
}
