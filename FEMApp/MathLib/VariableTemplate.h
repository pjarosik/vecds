#ifndef VARIABLETEMPLATE_H
#define VARIABLETEMPLATE_H
#include "Variable.h"

template< class T, vartype tp >
class VariableTemplate : public CVariable
{
    T value;
public:
    VariableTemplate( const T &v ):CVariable(tp),value(v) { }
    virtual ~VariableTemplate() { }
    virtual VariableTemplate* Clone() const { return new VariableTemplate< T, tp >( *this ); }
    T& GetValue() { return value; }
    const T& GetValue() const { return value; }
    operator T& () { return value; }
    void operator=( const T& v ) { value = v; }

    virtual void Write( ostream &out ) const { out << value; }
};


#endif // VARIABLETEMPLATE_H


