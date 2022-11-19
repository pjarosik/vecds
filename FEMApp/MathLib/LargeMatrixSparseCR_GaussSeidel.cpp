/* 
 * File:   CLargeMatrixSparseCR_GaussSeidel.cpp
 * Author: piotrek
 * 
 * Created on November 19, 2011, 4:13 PM
 */

#include "LargeMatrixSparseCR_GaussSeidel.h"
#include <iostream>
using namespace std;

CLargeMatrixSparseCR_GaussSeidel::CLargeMatrixSparseCR_GaussSeidel():CLargeMatrixSparseCR(false) {
}

CLargeMatrixSparseCR_GaussSeidel::CLargeMatrixSparseCR_GaussSeidel(const CLargeMatrixSparseCR_GaussSeidel& orig):CLargeMatrixSparseCR(orig) {
}

CLargeMatrixSparseCR_GaussSeidel::~CLargeMatrixSparseCR_GaussSeidel() {
}

bool CLargeMatrixSparseCR_GaussSeidel::Solve( const mvector &b, mvector &x )
{
    mvector  x1(dim), d( dim ), xe( dim ), x0( dim );
    unsigned k,l,iteration,ind;
    double conv, xj1, xj2;
    
    for (k=0; k<dim; k++) 
    {
        list<unsigned>::iterator i = find(lines[k].begin(),lines[k].end(),k);
        d[ k ] =  data[ blines_raw[ k ]  + distance(lines[k].begin(),i ) ];
    } 
    
    x.SetDim( dim );
    x = x1 = 0.0;
    
    cout << endl;
    CTimer prev_time,time;
    prev_time.Start();
    time.Start();
    iteration = 0;
    cout << "GaussSeidel Started" << endl;
    do {
        for( k=0; k<dim; k++)
        {
            xj1 = xj2 = 0.0;
            unsigned brow = blines_raw[ k ],bl=blines_raw[k],el=blines_raw[ k + 1 ];
            double *rdata = &data[ brow ];
            for (l=bl; l!=el; l++)
            {
                if ( ind < k ) xj1  += *rdata * x1[ cols_raw[ l ] ];
                if ( ind > k ) xj2  += *rdata *  x[ cols_raw[ l ] ];
                rdata++;
            }
            x1[ k ] = ( b[ k ] - xj1 - xj2 ) / d[ k ];
        }
                      
        conv = fabs( x.GetNorm() - x1.GetNorm() );
        
        x = x1;
        
        if ( prev_time.PreviewTime() > 20  ) 
        {
            cout << "iteration " << iteration << "...";
            cout << "conv = " << conv << " xNorm=" << x.GetNorm() << endl; cout.flush();
            prev_time.Start();
        }
    
        iteration++;

    } while( conv > 1E-12 && iteration < 100000 );
    time.Stop();
    
    MultiplyVector( x, xe );
    
    x0 = xe - b;
    
    cout << "iteration " << iteration << "...";
    cout << "conv = " << conv << " xNorm=" << conv << endl; cout.flush();
    cout << "solution time =" << time.GetFormattedTime() << endl;
    
    ofstream fout("GaussSeidel_solution.txt");
    cout << "x - Ax - b" << endl;
    double max_error=0.0;
    for (k=0; k<x0.GetDim(); k++)
    {
        if ( fabs( x0[ k ] ) > max_error ) max_error = fabs( x0[ k ] );
        fout << x[k] << " - " << x0[ k ] << "   relative- " << fabs( x0[k] ) / fabs( x[k] ) * 100.0 << " %" << endl;
    }
    cout << "max_error =" << max_error << " %";
    
}
