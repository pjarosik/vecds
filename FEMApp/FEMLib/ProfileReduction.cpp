/* 
 * File:   CProfileReduction.cpp
 * Author: piotrek
 * 
 * Created on November 19, 2011, 6:46 PM
 */

#include "ProfileReduction.h"

void CProfileReduction::Init( const vector<CFENode*> &nd, const vector< CFiniteElementInstance* > &es )
{
    nodes = nd;
    elements = es;
    CreateGraph();
}

void CProfileReduction::Init( const vector<CFENode*> &nd, const vector< vector< CFiniteElementInstance* > > &es )
{
    unsigned k,l;
    for (k=0; k<es.size(); k++)
        for(l=0; l<es[ k ].size(); l++ )
            elements.push_back( es[ k ][ l ] );
    
    CreateGraph();
    
}


CProfileReduction::CProfileReduction(const CProfileReduction& orig) {
}

CProfileReduction::~CProfileReduction() {
}

void CProfileReduction::CreateGraph()
{
    unsigned k, l, m;
       
    for (k=0; k<elements.size(); k++ )
        elements[ k ]->ResetNodalGraph();
    
    for (k=0; k<elements.size(); k++ )
        elements[ k ]->UpdateNodalGraph();
        
    for (k=0; k<nodes.size(); k++ ) 
    {
        nodes[ k ]->SetNumber( k );            
        nodes[ k ]->SortGraph( );
    }
        
    for (k=0; k< G.size(); k++)
        G.push_back( nodes[ k ] );
        
    
}
