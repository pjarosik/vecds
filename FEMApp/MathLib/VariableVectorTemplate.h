#ifndef VARIABLEVECTORTEMPLATE_H
#define VARIABLEVECTORTEMPLATE_H
#include "Variable.h"


template< class T >
class CVariableVectorTemplate : public CVariable
{
    vector< T > variables;

public:
    CVariableVectorTemplate( );
    CVariableVectorTemplate( const T &v, unsigned n=1 );
    CVariableVectorTemplate( const CVariableGroup &vg );
    virtual ~CVariableVectorTemplate() { }
    void Add( const T &v, unsigned n = 1 ) { unsigned k; for (k=0; k<n; k++) variables.push_back( v  ); }
    CVariableVectorTemplate& operator  <<( const T &v ) { Add(v); return *this; }
    virtual CVariableVectorTemplate* Clone() const { return new CVariableVectorTemplate( *this );  }
    T& operator[]( unsigned i ) {  return variables[ i ];  }
    const T& operator[]( unsigned i ) const {  return variables[ i ];  }
    T& last( ) {  return variables.back();  }
    T& first( ) {  return variables.front();  }
    size_t size() const { return variables.size(); }

    virtual void Write( ostream &out ) const;
};

template< class T >
CVariableVectorTemplate<T>::CVariableVectorTemplate():CVariable(VT_VECTOR_TEMPLATE) { }

template< class T >
CVariableVectorTemplate<T>::CVariableVectorTemplate( const T &v, unsigned n ):CVariable(VT_VECTOR_TEMPLATE)
{
    Add(v,n);
}

template< class T >
CVariableVectorTemplate<T>::CVariableVectorTemplate( const CVariableGroup &vg ):CVariable( vg ),variables( vg.variables )
{

}

template< class T >
void CVariableVectorTemplate<T>::Write( ostream &out ) const
{
     unsigned k;
     out << "[";
     if ( variables.size() > 20 )
        for (k=0; k<variables.size(); k++) out << variables[ k ] << endl;
     else
        for (k=0; k<variables.size(); k++) out << (k ? ", " : "") << variables[ k ];
     out << "]";
}




#endif // VARIABLEVECTORTEMPLATE_H
