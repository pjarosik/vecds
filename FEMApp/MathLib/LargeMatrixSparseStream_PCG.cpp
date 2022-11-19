#include "LargeMatrixSparseStream_PCG.h"
#include <iostream>

CLargeMatrixSparseStream_PCG::CLargeMatrixSparseStream_PCG( ):CLargeMatrixSparseStream(false) { }

CLargeMatrixSparseStream_PCG::CLargeMatrixSparseStream_PCG(const CLargeMatrixSparseStream_PCG& orig):CLargeMatrixSparseStream(orig) { }

CLargeMatrixSparseStream_PCG::~CLargeMatrixSparseStream_PCG() { }

bool CLargeMatrixSparseStream_PCG::Solve( const mvector &b, mvector &x )
{
    unsigned k;
    mvector r( dim ), h( dim ), s( dim ), h1( dim ), r1( dim ), C( dim ), As( dim ), x1( dim ), x0( dim );
    double alpha, beta, conv, b_norm = b.GetNorm(), rh;

    x = 0.0;
    r = b;

    cout << endl;
    CTimer prev_time,time;
    prev_time.Start();
    time.Start();

    cout << "StreamPCG Started" << endl;

    for (k=0; k<dim; k++)
    {
        unsigned ind = blines[ k ] + distance( lines[ k ].begin(), find( lines[ k ].begin(), lines[ k ].end(), k ) );
        C[ k ]  =  data[ ind ].value;
        h[ k ]    =  r[ k ] / C[ k ];
    }

    s = h;
    unsigned iteration = 0;

    do {
        rh = r % h;
        MultiplyVector( s, As );
        alpha = rh / ( s % As );
        double rh1 = 0.0;
        for (k=0; k<dim; k++)
        {
            r1[k] = r[k] - alpha * As[k];
            x1[k] = x[k] + alpha * s[k];
            h1[ k ] = r1[ k ] / C[ k ];
        }
        beta = ( r1 % h1 ) / rh ;
        for (k=0; k<dim; k++) s[k] = h1[k] + beta * s[k];
        x = x1;
        h = h1;
        r = r1;

        conv = r.GetNorm() / b_norm ;

        if ( prev_time.PreviewTime() > 20 )
        {
            cout << "iteration " << iteration << "...";
            cout << "conv = " << conv << " xNorm=" << x.GetNorm() << " time:" << time.PreviewTimeFormatted() << endl; cout.flush();
            prev_time.Start();
        }

        iteration++;

    } while( conv > 1.0E-7 );

    time.Stop();


    MultiplyVector( x, As );

    x0 = As - b;

    cout << "iterations :" << iteration << "...";
    cout << "xNorm      =" << x.GetNorm() << endl; cout.flush();
    cout << "error norm =" << x0.GetNorm() << endl; cout.flush();
    cout << endl << "solution time =" << time.GetFormattedTime() << endl;

 }
