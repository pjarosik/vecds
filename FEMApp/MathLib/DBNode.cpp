#include "DBNode.h"

CDBNode::CDBNode()
{
}

CDBNode::CDBNode( const CDBNode &dn ):subnodes(dn.subnodes),vars(dn.vars)
{
    map< string, CVariable* >::const_iterator i;
    for (i=dn.vars.begin(); i!=dn.vars.end(); i++)
        vars[ (*i).first ] = (*i).second->Clone();

    map< string, CDBNode* >::const_iterator j;
    for (j=dn.subnodes.begin(); j!=dn.subnodes.end(); j++)
        subnodes[ (*j).first ] = (*j).second->Clone();
}

const CDBNode& CDBNode::operator=( const CDBNode &nd )
{
    map< string, CVariable* >::const_iterator i;
    for (i=nd.vars.begin(); i!=nd.vars.end(); i++)
        vars[ (*i).first ] = (*i).second->Clone();

    map< string, CDBNode* >::const_iterator j;
    for (j=nd.subnodes.begin(); j!=nd.subnodes.end(); j++)
        subnodes[ (*j).first ] = (*j).second->Clone();
}

CDBNode::~CDBNode( )
{
    map< string, CVariable* >::const_iterator i;
    for (i=vars.begin(); i!=vars.end(); i++)
        delete (*i).second;

    map< string, CDBNode* >::const_iterator j;
    for (j=subnodes.begin(); j!=subnodes.end(); j++)
        delete (*j).second;
}


void CDBNode::Add( const string &name, const CDBNode &dn )
{
   /* map< string, CDBNode* >::const_iterator i = subnodes.find( name );
    if ( i != subnodes.end() )
    {

    }*/

    subnodes[ name ] = dn.Clone();
}

void CDBNode::Add( const string &name, const CVariable &v )
{
    map< string, CVariable* >::const_iterator i = vars.find( name );
    if ( i != vars.end() )
    {

    }

    vars[ name ] =v.Clone();
}

void CDBNode::Add( const string &name, CDBNode *dn )
{
   /* map< string, CDBNode* >::const_iterator i = subnodes.find( name );
    if ( i != subnodes.end() )
    {

    }*/

    subnodes[ name ] = dn;
}

void CDBNode::Add( const string &name, CVariable *v )
{
    map< string, CVariable* >::const_iterator i = vars.find( name );
    if ( i != vars.end() )
    {

    }

    vars[ name ] = v;
}


void CDBNode::print( ostream &of, unsigned level )
{
    string tab;
    tab.resize(5*level,' ');
    map< string, CVariable* >::const_iterator i;
    for (i=vars.begin(); i!=vars.end(); i++)
        of << tab << (*i).first << "=" << *(*i).second << endl;

    map< string, CDBNode* >::const_iterator j;

    for (j=subnodes.begin(); j!=subnodes.end(); j++)
    {
        of << tab << "NODE :[" << (*j).first << "]" << endl;
        (*j).second->print(of,level+1);
        of << endl;
    }
}

void CDBNode::print_tree( ostream &of, unsigned level )
{
    string tab;
    tab.resize(5*level,' ');

    map< string, CDBNode* >::const_iterator j;

    for (j=subnodes.begin(); j!=subnodes.end(); j++)
    {
        of << tab << "NODE :[" << (*j).first << "]" << endl;
        (*j).second->print_tree(of,level+1);
    }
}

void CDBNode::GetNodesNamed( const string &name, CVariableGroup &found, int level )
{
    if ( level == 0 ) return;
    map< string, CVariable* >::const_iterator i;
    for (i=vars.begin(); i!=vars.end(); i++)
        if (name ==  (*i).first ) found.Add( *(*i).second );

     map< string, CDBNode* >::const_iterator j;
     for (j=subnodes.begin(); j!=subnodes.end(); j++)
           (*j).second->GetNodesNamed( name, found,  level - 1 );
}
