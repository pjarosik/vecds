#include "VariableGroup.h"

CVariableGroup::CVariableGroup():CVariable(VT_GROUP) { }

CVariableGroup::CVariableGroup( const CVariable &v, unsigned n ):CVariable(VT_GROUP)
{
    Add(v,n);
}

CVariableGroup::~CVariableGroup()
{
    clear();
}

void CVariableGroup::clear(  )
{
    unsigned k;
    for (k=0; k<variables.size(); k++)
        delete variables[ k ];
    variables.clear();
}


CVariableGroup::CVariableGroup( const CVariableGroup &vg ):CVariable( vg ),variables( vg.variables )
{
    unsigned k;
    for (k=0; k<variables.size(); k++)
        variables[ k ] = variables[k]->Clone();
}

void CVariableGroup::Write( ostream &out ) const
{
     unsigned k;
     out << "[";
     if ( variables.size() > 20 )
        for (k=0; k<variables.size(); k++) out << *variables[ k ] << endl;
     else
        for (k=0; k<variables.size(); k++) out << (k ? ", " : "") << *variables[ k ];
     out << "]";
}
