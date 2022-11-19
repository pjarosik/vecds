/* 
 * File:   CLargeMatrixSparseCR_PCG.cpp
 * Author: piotrek
 * 
 * Created on November 19, 2011, 2:25 PM
 */

#include "LargeMatrixSparseCR_PCG.h"
#include <iostream>
using namespace std;

CLargeMatrixSparseCR_PCG::CLargeMatrixSparseCR_PCG(bool ipc):CLargeMatrixSparseCR(false),m_ipc(ipc) {
}

CLargeMatrixSparseCR_PCG::CLargeMatrixSparseCR_PCG(const CLargeMatrixSparseCR_PCG& orig):CLargeMatrixSparseCR(orig) {
}

CLargeMatrixSparseCR_PCG::~CLargeMatrixSparseCR_PCG() {
}

bool CLargeMatrixSparseCR_PCG::Solve( const mvector &b, mvector &x )
 {
    unsigned k,l;
    mvector r( dim ), h( dim ), s( dim ), h1( dim ), r1( dim ), C( dim ), As( dim ), x1( dim ), x0( dim ), f_conv;
    double alpha, beta, conv, b_norm = b.GetNorm(), rh,r_norm_i, r_norm_0;
    
    x = 0.0;
    r = b;
    
    cout << endl;
    CTimer prev_time,time;
    prev_time.Start();
    time.Start();

    cout << "PCG Started" << endl;
    
    if ( m_ipc )
    {
        cout << "Inclomplete LU..."; cout.flush();
        CLargeMatrixSparseCR &lcr = m_Prc;
        lcr = (CLargeMatrixSparseCR&)*this;
        m_Prc.Decompose();
        cout << "done!"; cout.flush();
        m_Prc.MultiplyVector(r,h);
    } else {
        for (k=0; k<dim; k++)
        {
            list<unsigned>::iterator i = find(lines[k].begin(),lines[k].end(),k);
            C[ k ] =  data[ blines_raw[ k ]  + distance(lines[k].begin(),i ) ];
            h[k] = r[k]/C[k];
        }
    } 
    
    s = h;
    unsigned iteration = 0;

    do {
        rh = r % h;
        MultiplyVector( s, As );
        alpha = rh / ( s % As );
        for (k=0; k<dim; k++)
        {
            r1[k] = r[k] - alpha * As[k];
            x1[k] = x[k] + alpha * s[k];
            h1[ k ] = r1[ k ] / C[ k ];
        }
        if ( m_ipc ) m_Prc.MultiplyVector(r1,h1);
        beta = ( r1 % h1 ) / rh ;
        for (k=0; k<dim; k++) s[k] = h1[k] + beta * s[k];
        x = x1;
        h = h1;
        r = r1;
        
        if ( iteration == 0 )
        {
            conv = 1.0;
            r_norm_0 = r.GetNorm();
        }
        else
        {
            r_norm_i = r.GetNorm();
            conv = r_norm_i / r_norm_0;
        }

        // conv = r.GetNorm() / b_norm ;

        if ( prev_time.PreviewTime() > 20 ) 
        {
            cout << "iteration " << iteration << "...";
            cout << "conv = " << conv << " rNorm=" << r.GetNorm() << " time:" << time.PreviewTimeFormatted() << endl; cout.flush();
            prev_time.Start();
        }
  
        iteration++;

    } while( conv > 1.0E-07 );
 
    time.Stop();    
    
    
    MultiplyVector( x, As );
    
    x0 = As - b;
    
    cout << "iterations :" << iteration << endl;
    cout << "conv       = " << conv << " xNorm=" << x.GetNorm() << endl; cout.flush();
    cout << "errNorm    = " << x0.GetNorm() << endl; cout.flush();
    cout << endl << "solution time =" << time.GetFormattedTime() << endl;

 }
