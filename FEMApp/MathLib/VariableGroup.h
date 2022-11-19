#ifndef VARIABLEGROUP_H
#define VARIABLEGROUP_H

#include "Variable.h"
#include <vector>
using namespace std;

class CVariableGroup : public CVariable
{
    vector< CVariable* > variables;

public:
    CVariableGroup( );
    CVariableGroup( const CVariable &v, unsigned n=1 );
    CVariableGroup( const CVariableGroup &vg );
    virtual ~CVariableGroup();
    void Add( const CVariable &v, unsigned n = 1 ) { unsigned k; for (k=0; k<n; k++) variables.push_back( v.Clone() ); }
    CVariableGroup& operator  <<( const CVariable &v ) { Add(v); return *this; }
    virtual CVariableGroup* Clone() const { return new CVariableGroup( *this );  }
    CVariable& operator[]( unsigned i ) {  return *variables[ i ];  }
    const CVariable& operator[]( unsigned i ) const {  return *variables[ i ];  }
    CVariable* last( ) {  return variables.back();  }
    CVariable* first( ) {  return variables.front();  }
    size_t size() const { return variables.size(); }
    void clear();

    virtual void Write( ostream &out ) const;
};

#endif // VARIABLEGROUP_H
