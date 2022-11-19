/* 
 * File:   CLargeMatrixSparseCRJacoby.cpp
 * Author: piotrek
 * 
 * Created on November 17, 2011, 3:12 PM
 */

#include "LargeMatrixSparseCRJacoby.h"
#include <iostream>

CLargeMatrixSparseCRJacoby::CLargeMatrixSparseCRJacoby():CLargeMatrixSparseCR(false) {
}

CLargeMatrixSparseCRJacoby::CLargeMatrixSparseCRJacoby(const CLargeMatrixSparseCRJacoby& orig):CLargeMatrixSparseCR(orig) {
}

CLargeMatrixSparseCRJacoby::~CLargeMatrixSparseCRJacoby() {
}

bool CLargeMatrixSparseCRJacoby::Solve( const mvector &b, mvector &x )
{
    unsigned k=0;
    mvector x1( b.GetDim() ), d( dim ), xA( b.GetDim() );
    for (k=0; k<dim; k++) 
    {
        list<unsigned>::iterator i = find(lines[k].begin(),lines[k].end(),k);
        if ( i != lines[k].end() ) d[ k ] = data[ blines[ k ]  + distance(lines[k].begin(),i ) ] ;
        else
        {
            int a=1;
        }
    }
    x = x1 = 0.003;
    vector<double> perf;
    double perfv;
    CTimer prev_time,time;
    prev_time.Start();
    time.Start();
    cout << "Jacoby Started" << endl;
    while(true)
    {
        MultiplyVector( x1, xA );
        perfv = 0.0;
        for (k=0; k<dim; k++) 
        {
            x[ k ] = x1[ k ] - (xA[ k ] - b[ k ]) / d[ k ];
            perfv += fabs( xA[ k ] - b[ k ] );
        }
        
        if ( prev_time.PreviewTime() > 20 ) 
        {
            cout << "iteration " << k << "...";
            cout << "conv = " << perfv << " xNorm=" << x.GetNorm() << endl; cout.flush();
            prev_time.Start();
        }
    
        k++;
        
        perf.push_back( perfv );
        x1 = x;
    }
    
}
