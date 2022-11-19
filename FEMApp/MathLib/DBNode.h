#ifndef DBNODE_H
#define DBNODE_H

#include "Variable.h"
#include "VariableGroup.h"
#include "MathException.h"
#include <map>
#include <vector>
#include <iostream>
using namespace std;

class CDBNode
{

public:

    CDBNode();
    CDBNode( const CDBNode &dn );
    virtual CDBNode* Clone() const { return new CDBNode( *this ); }
    const CDBNode& operator=( const CDBNode &nd );
    virtual ~CDBNode();

    void Add( const string &name, const CDBNode &dn );
    void Add( const string &name, const CVariable &dn );
    void Add( const string &name, CDBNode *dn );
    void Add( const string &name, CVariable *dn );

    void print( ostream &of, unsigned level = 0  );
    void print_tree( ostream &of, unsigned level = 0  );
    void GetNodesNamed( const string &name, CVariableGroup &found, int level = -1 );
    const CVariable* GetVariable( const string &name, bool ex = true ) const;
    CVariable* GetVariable( const string &name, bool ex = true );
    const map< string, CVariable* >& GetVariables() const { return vars; }

protected:

    map< string, CDBNode* > subnodes;
    map< string, CVariable* > vars;

};

inline const CVariable* CDBNode::GetVariable( const string &nm, bool ex ) const
{
    map< string, CVariable* >::const_iterator found = vars.find( nm );
    if ( found == vars.end( ) )
    {
        if ( ex ) throw CMathException( "Variable named \'" + nm + "\'not found");
        else return 0;
    }
    return found->second;
}


inline CVariable* CDBNode::GetVariable( const string &nm, bool ex )
{
    map< string, CVariable* >::const_iterator found = vars.find( nm );
    if ( found == vars.end() )
    {
        if ( ex ) throw CMathException( "Variable named \'" + nm + "\'not found");
        else return 0;
    }
    return found->second;
}

#endif // DBNODE_H
