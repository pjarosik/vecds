#ifndef GRAPHFACE_H
#define GRAPHFACE_H

#include <vector>
#include "GraphNode.h"
#include "../MathLib/AlgorithmsMath.h"
#include "GraphObject.h"

using namespace std;

class CGraphFace : public CGraphObject
{
public:

    CGraphFace();
    bool operator==(const CGraphFace &gf) const { return CompareContainers( gf.GetNodes(), nodes ); }
    const vector< CGraphNode* >& GetNodes() const { return nodes; }

protected:

    vector< CGraphNode* > nodes;
};

#endif // GRAPHFACE_H
