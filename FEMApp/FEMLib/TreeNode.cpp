
#include "TreeNode.h"
#include <algorithm>
#include <iostream>
using namespace std;

void DeleteTree( CTreeNode *nd )
{
	vector< CTreeNode* > v;
	nd->GetStructure( v );
	unsigned k;
	for (k=0; k<v.size(); k++)
		delete v[k];
	delete nd;
}

CTreeNode::CTreeNode( const string &nm ):printdiag(false), objectname( nm ), unlocked( true ), notvalid( true ), cloned_node( 0 )
{
}

CTreeNode::~CTreeNode(void)
{
}

CTreeNode* CTreeNode::AddNode( CTreeNode *ob )
{
	if ( find( depends.begin(), depends.end(), ob ) == depends.end() ) depends.push_back( ob );
	return ob;
}

void CTreeNode::GetStructure( vector< CTreeNode* > &v ) const
{
	if ( !unlocked ) return;
	vector< CTreeNode* >::const_iterator si;
	for (si=depends.begin(); si!=depends.end(); si++)
	{
		(*si)->GetStructure( v );
		if ( find( v.begin(), v.end(), *si ) == v.end() )
			v.push_back( *si );
	}

}

void CTreeNode::CreateUsedByVector( )
{
	usedby.clear();
	vector< CTreeNode* > ub;
	UpdateUsedByVector( ub );
}

void CTreeNode::UpdateUsedByVector( vector< CTreeNode* > us )
{
	vector< CTreeNode* >::const_iterator si;
	for (si=us.begin(); si!=us.end(); si++)
		if ( find( usedby.begin(), usedby.end(), (*si) ) == usedby.end() )
			usedby.push_back( (*si) );

	if ( find( us.begin( ), us.end( ), this ) == us.end( ) )
			us.push_back( this );

	for ( si = depends.begin(); si != depends.end(); si++ )
		(*si)->UpdateUsedByVector( us );
}


void CTreeNode::UnlockStructure()
{
	UnlockThis();
	vector< CTreeNode* >::const_iterator si;
	for (si=depends.begin(); si!=depends.end(); si++)
		(*si)->UnlockStructure();
}


void CTreeNode::InvalidateStructure()
{
	InvalidateThis();
	vector< CTreeNode* >::const_iterator si;
	for (si=depends.begin(); si!=depends.end(); si++)
		(*si)->InvalidateStructure();
}

void CTreeNode::InvalidateUsedByObjects()
{
	InvalidateThis();
	vector< CTreeNode* >::const_iterator si;
	for (si=usedby.begin(); si!=usedby.end(); si++)
		(*si)->InvalidateThis();
}

void CTreeNode::PrintStructure( ostream &out, unsigned level )
{
	unsigned k;
	for (k=0; k<level; k++) out << '\t';
	out << objectname << endl;
	for (k=0; k<depends.size(); k++)
        depends[ k ]->PrintStructure( out, level + 1 );
}

 CTreeNode* CTreeNode::CloneTree( bool clear )
{
    if ( clear ) CloneReset();
    if ( cloned_node ) return cloned_node;
    CTreeNode *cn = Clone();
    cn->CreateUsedByVector();
    return cn;
}

void  CTreeNode::CloneReset()
{
    //if ( !cloned_node ) return;
	cloned_node = 0;
	vector< CTreeNode* >::iterator i;
    for (i=depends.begin(); i!=depends.end(); i++)
        (*i)->CloneReset();
}
