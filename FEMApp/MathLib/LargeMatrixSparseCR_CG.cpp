/* 
 * File:   CLargeMatrixSparseCR_CG.cpp
 * Author: piotrek
 * 
 * Created on November 18, 2011, 2:22 PM
 */

#include "LargeMatrixSparseCR_CG.h"
#include "../MathLib/Timer.h"
#include <iostream>

CLargeMatrixSparseCR_CG::CLargeMatrixSparseCR_CG():CLargeMatrixSparseCR(false) {
}

CLargeMatrixSparseCR_CG::CLargeMatrixSparseCR_CG(const CLargeMatrixSparseCR_CG& orig):CLargeMatrixSparseCR(orig) {
}

CLargeMatrixSparseCR_CG::~CLargeMatrixSparseCR_CG() {
}

bool CLargeMatrixSparseCR_CG::Solve( const mvector &b, mvector &x )
{
    unsigned k,l;
    mvector r( dim ), h( dim ), s( dim ), h1( dim ), r1( dim ), C( dim ), As( dim ), x1( dim ), x0( dim ), f_conv;
    double alpha=0, beta=0, conv=0, b_norm = b.GetNorm(),rr;
    
    x = 0.0;
    r = b;
    
    x = 0.0;
    r = b;
    s = r;
    k=0;
    cout << endl;
    CTimer prev_time,time;
    prev_time.Start();
    time.Start();
    cout << "CG Started" << endl;
    do {
        
        MultiplyVector( s, As );
        rr = r % r;
        alpha =  rr / ( s % As );
        for (l=0; l<dim; l++) r1[ l ] = r[ l ] - alpha * As[ l ];
        for (l=0; l<dim; l++) x1[ l ] = x[ l ] + alpha *  s[ l ];
        beta = ( r1 % r1 ) / rr;
        for (l=0; l<dim; l++) s[ l ] = r1[ l ] + beta * s[ l ];
        x = x1;
//      h = h1;
        r = r1;
       
        conv = r.GetNorm() / b_norm ;
        
        if ( prev_time.PreviewTime() > 20 ) 
        {
            cout << "iteration " << k << "...";
            cout << "conv = " << conv << " xNorm=" << x.GetNorm() << " time:" << time.PreviewTimeFormatted() << endl; cout.flush();
            prev_time.Start();
        }
    
        k++;

    } while( conv > 1E-12 );
    time.Stop();
    
    MultiplyVector( x, As );
    
    x0 = As - b;
    
    cout << "iteration " << k++ << "...";
    cout << "conv = " << conv << " xNorm=" << x.GetNorm() << endl; cout.flush();
    cout << "solution time =" << time.GetFormattedTime() << endl;
    
    ofstream fout("CG_solution.txt");
    cout << "x - Ax - b" << endl;
    double max_error=0.0;
    for (k=0; k<x0.GetDim(); k++)
    {
        if ( fabs( x0[ k ] ) > max_error ) max_error = fabs( x0[ k ] );
        fout << x[k] << " - " << x0[ k ] << "   relative- " << fabs( x0[k] ) / fabs( x[k] ) * 100.0 << " %" << endl;
    }
    cout << "max_error =" << max_error << " %";
}
