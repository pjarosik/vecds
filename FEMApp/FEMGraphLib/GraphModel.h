#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H

#include "GraphNode.h"
#include "GraphEdge.h"
#include "GraphFace.h"
#include "../FEMLib/FiniteElementInstance.h"
#include <map>

using namespace std;

class CGraphModel
{
public:
    CGraphModel( const vector< CFENode* > &fen );
    void AddDomain( const FEDomain &fe );
    virtual ~CGraphModel();

protected:

    vector< CGraphNode* > nodes;
    vector< CGraphEdge* > edges;
    vector< CGraphFace* > faces;



    map< CFENode*, CGraphNode* > mapnodes;


};

#endif // GRAPHMODEL_H
