/* 
 * File:   CFEMAppInterface.cpp
 * Author: piotrek
 * 
 * Created on April 1, 2010, 10:37 AM
 */

#include "../FEMAppLib/StatementFeap.h"
#include "FEMAppInterface.h"
#include <iostream>
#include <fstream>

CFEMAppInterface::CFEMAppInterface() {
}

CFEMAppInterface::CFEMAppInterface(const CFEMAppInterface& orig) {
}

CFEMAppInterface::~CFEMAppInterface() {
    map< string, GraphData < double >* >::iterator i;
    for ( i = graphdatas.begin(); i != graphdatas.end(); i++ )
        delete i->second;
}

void CFEMAppInterface::ImportFEAP( const string &filename )
{
    ifstream fin( filename.c_str() );
    CStatementFeap feapin( FPR );
    feapin.Import( filename );
    ComputeSizes();
}

void CFEMAppInterface::ReadGraphData( const string &fnd )
{
    GraphData< double > *ngd = new GraphData<double>;
    ifstream fin( fnd.c_str() );
    string name;
    fin >> name >> *ngd;
    graphdatas[ fnd ] = ngd;
}

void CFEMAppInterface::ReadFEData( const string &ffe )
{
    ifstream fin( ffe.c_str() );
    unsigned nn,k;
    string type;
    fin >> type;
    fin.close();
    if ( type == "GRAPH_DEVICE" )
    {
       ReadGraphData( ffe );
       ComputeSizes();
    }
}

void CFEMAppInterface::GetNodalValuesMinMax(const string &value, double &min, double &max , bool boundary )
{
    const vector< CFENode* > &nodes =  GetNodes();
    vector< CFENode* >::const_iterator i;
    min =        1e300;
    max =       -1e300;
    for ( i = nodes.begin(); i!= nodes.end(); i++ )
    {
        if ( boundary && !(*i)->IsBoundary() ) continue;
        double res = (*i)->GetResult( value );
        if ( res < min ) min = res;
        if ( res > max ) max = res;
    }
}

void CFEMAppInterface::GetFacialValuesMinMax( const string &value, double &min, double &max, bool boundary , unsigned dn  )
{

    vector< CFiniteElementInstance* > &elems = FPR.GetElements();
    vector< CFiniteElementInstance* >::const_iterator i;
    min = 1e300; max = -1e300;
    for ( i=elems.begin(); i!= elems.end(); i++)
    {
         vector< CGeometricObjectShapeNodal* > faces;      // wektor zawierający ścianbhrlaf.com elementu
         (*i)->GetFaces( faces, true );      // pobranie brzegowych ścian ścian dla k-tego elementu ( do wektora o nazwie faces )
         vector< CGeometricObjectShapeNodal* >::const_iterator j;
         for (j=faces.begin(); j!=faces.end(); j++ )
         {
               if ( boundary && !(*j)->IsBoundary() ) continue;
               unsigned k,l;
               CScalarShapeSpaceApprox faprr = (*i)->CreateResultApproximator( *j, value );
               mvector ksi(2);
               for (k=0; k<=dn; k++)
                   for (l=0; l<=dn; l++)
                   {
                        ksi[0] = -1.0 + k * 2.0 / dn;
                        ksi[1] = -1.0 + l * 2.0 / dn;
                        double res = faprr.interpolate( ksi );
                        if ( res < min ) min = res;
                        if ( res > max ) max = res;

                   }
         }
    }

}


void CFEMAppInterface::ComputeSizes()
{
    minX = 1e300; maxX = -1e300;
    minY = 1e300; maxY = -1e300;
    minZ = 1e300; maxZ = -1e300;

    map< string, GraphData < double >* >::const_iterator i;
    for ( i = graphdatas.begin();  i != graphdatas.end(); i++ )
        i->second->MaxMins( minX, maxX, minY, maxY, minZ, maxZ );

    unsigned k;
    for( k=0; k<GetNodes().size(); k++) {
        mvector X(3);
        GetNodes()[k]->GetX(X);
        if ( X[0] < minX ) minX = X[0];  if ( X[0] > maxX ) maxX = X[0];
        if ( X[1] < minY ) minY = X[1];  if ( X[1] > maxY ) maxY = X[1];
        if ( X[2] < minZ ) minZ = X[2];  if ( X[2] > maxZ ) maxZ = X[2];
    }

}

