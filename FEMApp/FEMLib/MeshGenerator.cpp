/* 
 * File:   CMeshGenerator.cpp
 * Author: piotrek
 * 
 * Created on November 21, 2011, 12:09 PM
 */

#include "MeshGenerator.h"
#include <iomanip>

CMeshGenerator::CMeshGenerator():nnode( 0 ), nelems( 0 ) {
}

CMeshGenerator::~CMeshGenerator() 
{  }

void CMeshGenerator::Save( const string &fn )
{
    ofstream fout(fn.c_str());
    fout << "Nodal coordinates"<< endl;
    fout.setf(ios::scientific);
    unsigned k,l;
    for(k=0; k<points.size(); k++)
    {
        fout << k<< "  - ";
        for (l=0; l<points[k].GetDim(); l++)
            fout << setw( 15 ) << points[k][l];
        fout << endl;
    }
    
    fout << endl << "Elements connection" << endl;
    for( k=0; k<elements.size(); k++ )
    {
        for ( l=0; l<elements[k].size(); l++ )
            fout << setw( 8 ) << elements[k][l];
        fout << endl;
    }
}