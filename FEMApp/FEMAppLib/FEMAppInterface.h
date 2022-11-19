/* 
 * File:   CFEMAppInterface.h
 * Author: piotrek
 *
 * Created on April 1, 2010, 10:37 AM
 */

#ifndef _CFEMAPPINTERFACE_H
#define	_CFEMAPPINTERFACE_H

#include "GraphData.h"
#include "../FEMAppLib/FEMProject.h"

using namespace std;

class CFEMAppInterface {

public:

    CFEMAppInterface();
    CFEMAppInterface(const CFEMAppInterface& orig);
    virtual ~CFEMAppInterface();

    void RunCalculations( const string &filename ) {  FPR.ReadFile( filename ); }
    void ImportFEAP( const string &filename );
    const vector< CFENode* > & GetNodes() const { return FPR.GetNodes(); }
    const vector< CFiniteElementInstance* >& GetElements() const { return FPR.GetElements(); }

    void GetMaxX( double &x1,double &x2) const { x1 = minX; x2 = maxX; }
    void GetMaxY( double &y1,double &y2) const { y1 = minY; y2 = maxY; }
    void GetMaxZ( double &z1,double &z2) const { z1 = minZ; z2 = maxZ; }
    void GetNodalValuesMinMax(const string &value, double &min, double &max, bool boundary = false );
    void GetFacialValuesMinMax( const string &value, double &min, double &max, bool boundary = false , unsigned dn = 2 );

    const CFENode& GetNode( unsigned i ) const { return *GetNodes()[ i ];  }
    const CFiniteElementInstance& GetElement( unsigned i ) const { return *GetElements()[ i ];  }

    const map< string, GraphData < double >* >& GetMaps() const { return graphdatas; }
    void ReadFEData( const string &ffe );

protected:

    string name;
    map< string, GraphData < double >* > graphdatas;
    CFEMProject FPR;
    vector<string> names;
    vector< vector<double> > fn_values;
    vector< bool > is_boundary;
    double minX, maxX, minY, maxY, minZ, maxZ;

    void ComputeSizes();
    void ReadGraphData( const string &fnd );

};

#endif	/* _CFEMAPPINTERFACE_H */

