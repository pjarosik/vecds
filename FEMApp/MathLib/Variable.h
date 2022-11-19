#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include <iostream>
using namespace std;
#include "var_types.h"


class CVariable
{
    vartype type;
public:
    CVariable( const vartype &v ):type( v ) { }
    virtual ~CVariable() { }
    virtual CVariable* Clone() const =0;
    const vartype& getType() const { return type; }

    virtual void Write( ostream &out ) const =0;
};

inline ostream& operator << ( ostream &out, const CVariable &v  ) { v.Write(out); return out; }

#endif // VARIABLE_H


