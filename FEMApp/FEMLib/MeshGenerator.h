/* 
 * File:   CMeshGenerator.h
 * Author: piotrek
 *
 * Created on November 21, 2011, 12:09 PM
 */

#ifndef CMESHGENERATOR_H
#define	CMESHGENERATOR_H

#include <vector>
#include "../MathLib/matrix.h"
using namespace std;


class CMeshGenerator {

public:
    
    CMeshGenerator();
    virtual ~CMeshGenerator();
    
    virtual bool Generate() = 0;
    
    void Clear() { points.clear(); elements.clear(); }
    
    unsigned GetNodesNumber() const { return nnode; }
    unsigned GetElemsNumber() const { return nelems; }
    
    const vector< mvector >& GetNodes() const { return points; }
    const vector< vector< unsigned > >& GetElements() const { return elements; }
    
    void Save( const string &fn );
    
protected:
    
    unsigned nnode, nelems;
    vector< mvector > points;
    vector< vector< unsigned > > elements, edges, faces;
};

#endif	/* CMESHGENERATOR_H */

