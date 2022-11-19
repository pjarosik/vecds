#include "GraphModel.h"

CGraphModel::CGraphModel( const vector< CFENode* > &fen )
{
    unsigned k;
    for ( k=0; k<fen.size(); k++ )
    {
        CGraphNode *gn = new CGraphNode( fen[ k ] );
        nodes.push_back( gn );
        mapnodes[ fen[ k ] ] = gn;
    }

}



void CGraphModel::AddDomain( const FEDomain &fe )
{
    unsigned k, l;
    for( k = 0; k < fe.size(); k++ )
    {

    }
}



CGraphModel::~CGraphModel()
{
    vector< CGraphNode* >::const_iterator i;
    vector< CGraphEdge* >::const_iterator j;
    vector< CGraphFace* >::const_iterator k;

    for ( i = nodes.begin(); i != nodes.end(); i++) delete *i;
    for ( j = edges.begin(); j != edges.end(); j++) delete *j;
    for ( k = faces.begin(); k != faces.end(); k++) delete *k;

}
