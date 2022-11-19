#ifndef CGRAPHNODE_H
#define CGRAPHNODE_H

#include "../FEMLib/FENode.h"
#include "GraphObject.h"
#include <vector>
using namespace std;

class CGraphEdge;
class CGraphFace;

class CGraphNode : public CGraphObject
{
public:

    CGraphNode( const CFENode *fn ):fnd( fn ) { }
    bool operator==( const CGraphNode &gn ) const { return gn.GetFENode() == fnd; }
    const CFENode* GetFENode() const { return fnd; }
    //const mvector& GetX() const { return fnd->GetX(); }
   // double operator[](unsigned i) const { return fnd->GetCoord(i); }

protected:

    const CFENode *fnd;
    vector< CGraphEdge* > edges;
    vector< CGraphFace* > faces;
};

#endif CGRAPHNODE_H
