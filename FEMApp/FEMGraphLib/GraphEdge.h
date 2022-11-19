#ifndef CGRAPHEDGE_H
#define CGRAPHEDGE_H

#include <vector>
#include "GraphNode.h"
#include "../MathLib/AlgorithmsMath.h"
#include "GraphObject.h"

class CGraphEdge : public CGraphObject
{
public:

    CGraphEdge();
    bool operator==(const CGraphEdge &ge) const { return CompareContainers( ge.GetNodes(), nodes ); }
    const vector< CGraphNode* >& GetNodes() const { return nodes; }

protected:

    vector< CGraphNode* > nodes;

};

#endif // CGRAPHEDGE_H
