#ifndef SETUP_H
#define SETUP_H
#include "SimpleVariables.h"
#include "DBNode.h"
#include "MathException.h"

/*

typedef VariableTemplate< bool,     VT_BOOL     > VariableBool;
typedef VariableTemplate< int,      VT_INT      > VariableInt;
typedef VariableTemplate< unsigned, VT_UNSIGNED > VariableUnsigned;
typedef VariableTemplate< double,   VT_DOUBLE   > VariableDouble;
typedef VariableTemplate< mvector,  VT_VECTOR   > VariableVector;
typedef VariableTemplate< matrix,   VT_MATRIX   > VariableMatrix;
typedef VariableTemplate< string,   VT_STRING   > VariableString;

 */


class CSetup : public CDBNode
{
public:
    CSetup( );

    void Add( const string &nm, const string &st )  { Add( nm, VariableString( st ) );  }
    void Add( const string &nm, int v )             { Add( nm, VariableInt( v ) );      }
    void Add( const string &nm, unsigned v )        { Add( nm, VariableUnsigned( v ) ); }
    void Add( const string &nm, double v )          { Add( nm, VariableDouble( v ) );   }
    void Add( const string &nm, bool v )            { Add( nm, VariableBool( v ) );     }
    void Add( const string &nm, const mvector &v )  { Add( nm, VariableVector( v ) );   }
    void Add( const string &nm, const matrix &v )   { Add( nm, VariableMatrix( v ) );   }

    string& GetString( const string &nm )       { return *dynamic_cast< VariableString*     >( GetVariable( nm ) ); }
    int& GetInt( const string &nm )             { return *dynamic_cast< VariableInt*        >( GetVariable( nm ) ); }
    unsigned& GetUnsigned( const string &nm )   { return *dynamic_cast< VariableUnsigned*   >( GetVariable( nm ) ); }
    double& GetDouble( const string &nm )       { return *dynamic_cast< VariableDouble*     >( GetVariable( nm ) ); }
    bool& GetBool( const string &nm )           { return *dynamic_cast< VariableBool*       >( GetVariable( nm ) ); }
    mvector& GetVector( const string &nm )      { return *dynamic_cast< VariableVector*     >( GetVariable( nm ) ); }
    matrix&  GetMatrix( const string &nm )      { return *dynamic_cast< VariableMatrix*     >( GetVariable( nm ) ); }

    const string& GetString( const string &nm ) const       { return dynamic_cast< const VariableString*   >( GetVariable( nm ) )->GetValue(); }
    const int& GetInt( const string &nm ) const             { return dynamic_cast< const VariableInt*      >( GetVariable( nm ) )->GetValue(); }
    const unsigned& GetUnsigned( const string &nm ) const   { return dynamic_cast< const VariableUnsigned* >( GetVariable( nm ) )->GetValue(); }
    const double& GetDouble( const string &nm ) const       { return dynamic_cast< const VariableDouble*   >( GetVariable( nm ) )->GetValue(); }
    const bool& GetBool( const string &nm ) const           { return dynamic_cast< const VariableBool*     >( GetVariable( nm ) )->GetValue(); }
    const mvector& GetVector( const string &nm ) const      { return dynamic_cast< const VariableVector*   >( GetVariable( nm ) )->GetValue(); }
    const matrix&  GetMatrix( const string &nm ) const      { return dynamic_cast< const VariableMatrix*   >( GetVariable( nm ) )->GetValue(); }

};




#endif // SETUP_H
