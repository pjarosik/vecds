/* 
 * File:   CProfileReductionRCM.cpp
 * Author: piotrek
 * 
 * Created on November 20, 2011, 12:01 PM
 */

#include "ProfileReductionRCM.h"
#include <queue>
#include <list>


CProfileReductionRCM::CProfileReductionRCM(const CProfileReductionRCM& orig):CProfileReduction(orig) {
}

CProfileReductionRCM::~CProfileReductionRCM() {
}

void CProfileReductionRCM::Optimize()
{
    
    /*
        CGraphVerticesContainer Q,R,S,x;
	int k,i=0;
	CGraphVertexOwnerIterator j( G );
	while( ++j ) Q.Add( j );

	x.queue_put( Q[ 0 ] );
	Q.Remove( 0 );
	R.AddContainer( x[0]->ED );
	Q.RemoveContainer( R );
	CGraphVertex *z;
	bool done=false;
	do {

		i += R.N();
		x.AddContainer( R );
		S.AddContainer( R );
		R.Reset();
		if ( Q.N() )
		{
			z = S.queue_get();
			CGraphVertexIterator j( z->ED );
			while( ++j ) if ( Q.CheckExistence( j ) ) R.queue_put( j );
			Q.RemoveContainer( R );
		}
		else done = true;
	} while( !done );
	for ( k = x.N() - 1;  k>=0;  k--)	x[ k ]->new_number = x.N() - 1 - k;
     
     */
    

    
    unsigned k;
    vector< CFENode* > G( nodes );
    list< CFENode* > Q, R, S, x;

    
    if ( G[0]->GetGraphNodes().size() == 0 )
        throw CFEException("CProfileReductionRCM::Optimize() - graph not created");
    
    vector< CFENode* >::const_iterator i;
    for ( i = G.begin() ; i!= G.end(); i++)
        Q.push_back( *i );
    x.push_back( G[ 0 ] );
    Q.pop_front();
    for (k=0; k<x.front()->GetGraphNodes().size(); k++ ) R.push_back( x.front()->GetGraphNodes()[ k ] );
    RemoveList( R, Q );
    CFENode *z=0;
    bool done=false;
    do {
//        i += R.size();
        MergeLists( R, x );
        S.merge( R );
        if (Q.size())
        {
            if ( S.size() == 0 ) 
            {
                throw CFEException("CProfileReductionRCM::Optimize() optimization error");
            }
            z = S.front(); S.pop_front();
            vector< CFENode* >::const_iterator j;
             for( j = z->GetGraphNodes().begin(); j!=z->GetGraphNodes().end();  j++ )
                 if ( find( Q.begin(),Q.end(), *j ) != Q.end()  ) R.push_back( *j );
             RemoveList( R, Q );
        }
        else done = true;
    } while( !done );
    k=0;
    list< CFENode* >::const_iterator j;
    numerator.resize( x.size() );
    for ( j = x.begin(); j!=x.end();  j++, k++ )
    {
        (*j)->SetNumber( x.size() - k - 1 );
        numerator[ x.size() - k - 1 ] = k;
    }
            
    
}

void CProfileReductionRCM::RemoveList ( const list<CFENode*> &source, list<CFENode*> &target )
{
    list< CFENode* >::const_iterator i;
    for (i=source.begin(); i!=source.end(); i++)
        target.remove( *i );
}


void CProfileReductionRCM::MergeLists( const list<CFENode*> &source, list<CFENode*> &target )
{
    list< CFENode* >::const_iterator i;
    for (i=source.begin(); i!=source.end(); i++)
        target.push_back( *i );
}
