#ifndef CLONABLE_H
#define CLONABLE_H
#include <map>
#include <vector>
using namespace std;


template < class T >
class Cloner
{
    map< const T*, T* > objects;
    vector< T* > cloned;
public:
    Cloner() { }
    T* Clone( const T *o );
    void Reset() { objects.clear(); cloned.clear(); }
    const vector< T* > GetCloned() const { return cloned; }

};


template < class T >
T* Cloner<T>::Clone( const T *o )
{
    typename map< const T*, T* >::iterator i = objects.find( o );
    if ( i == objects.end() )
    {
        T *object = o->Clone();
        objects[ o ] = object;
        object->DeepClone( *this );
        cloned.push_back( object );
        return object;
    }
    return i->second;
}

/*
class CCopyer;

class Copyable
{
    public :
    virtual Copyable* Copy( CCopyer &cp ) const = 0;
};

class CCopyer {
    map< const Copyable*, Copyable* > cp;
public:
    template <class T> T* Copy( const T *ob )
    {
        map< const Copyable*, Copyable* >::iterator i = cp.find( ob );
        if ( i == cp.end() )
        {
            Copyable *co = ob->Copy( *this );
            cp[ ob ] = co;
            return dynamic_cast< T*>( co );
        }
    }

    template <class T> vector<T*> Copy( const vector<T*> &obs )
    {
        vector<T*> v( obs );
        unsigned k;
        for (k=0; k<obs.size(); k++ ) v[ k ] = Copy( v[ k ] );
    }

    const map< const Copyable*, Copyable* >& GetCopyed() const { return cp; }
};

*/
#define CLONE_OBJECT( CL ) virtual CL* Clone( ) const \
    { \
            return new CL( *this );  \
    }

/*
#define COPY_OBJECT( CL ) virtual CL* Copy( CCopyer &cpr ) const \
    { \
            return new CL( cpr, *this );  \
    }

*/
#endif // CLONABLE_H
