#ifndef DOMAINCONTAINER_H
#define DOMAINCONTAINER_H

#include <vector>
#include <map>
#include <string>
#include "MathException.h"
#include <assert.h>

using namespace std;

template< class T >
class DomainContainer
{
public:
    DomainContainer(bool own=true);
    ~DomainContainer();

    void AddMember( T *o, const string &nm="" );
    void AddMembers( const vector< T*> &o, const string &nm="" );
    const vector< T* >* GetDomain( const string &nd ) const;
    vector< T* >* GetDomain( const string &nd );
    const vector< T* >& GetAll( ) const { return allmembers; }
    vector< T* >& GetAll( ) { return allmembers; }
    T* operator[](unsigned i) const { return allmembers[ i ]; }
    T* operator[](unsigned i) { return allmembers[ i ]; }
    size_t size() const { return allmembers.size(); }
    void clear();
    size_t DomainsNumber() const { return domains.size(); }
    void GetDomainNames( vector<string> &dn ) const;
    bool Find( const T *t ) const;
    string FindDomain( const T *t ) const;


protected:

    DomainContainer( const DomainContainer &dc ) { }

    vector< T* > allmembers;
    map< string, vector< T* > > domains;
    bool owner;
};

template< class T >
DomainContainer<T>::DomainContainer(bool own):owner(own) { }

template< class T >
void DomainContainer<T>::clear()
{
    if ( owner )
    {
        typename vector< T* >::const_iterator i;
        unsigned k;
        for( k=0; k<allmembers.size(); k++)
        {
            T *ptr = allmembers[k];
            delete ptr;
        }
    }
    allmembers.clear();
    domains.clear();
}

template< class T >
DomainContainer<T>::~DomainContainer()
{
    clear();
}

template< class T >
void DomainContainer<T>::AddMember( T *o, const string &nm )
{
#ifdef _DEBUG
    if ( find( allmembers.begin(), allmembers.end(), o ) != allmembers.end() )
    {
        assert(0);
    }
#endif

    allmembers.push_back( o );
    if ( nm != "" )
    {
        domains[ nm ].push_back( o );
//        inds[ nm ].push_back( allmembers.size() - 1 );
    }
}

template< class T >
bool DomainContainer<T>::Find( const T *t ) const
{
    unsigned k;
    for (k=0; k<allmembers.size(); k++)
        if ( allmembers[k] == t ) return true;
    return false;
}

template< class T >
string DomainContainer<T>::FindDomain( const T *t ) const
{
    typename map< string, vector< T* > >::const_iterator id;
    for (id = domains.begin(); id != domains.end(); id++ )
        if ( find( id->second.begin(), id->second.end(), t ) !=  id->second.end() ) return id->first;
    return "";
}

template< class T >
void DomainContainer<T>::AddMembers( const vector< T* > &o, const string &nm )
{
    if ( o.size() == 0 ) return;
    typename vector< T* >::const_iterator i;
    for (i=o.begin(); i!=o.end(); i++)  allmembers.push_back( *i );
    if ( nm == "" ) return;
    domains[ nm ].push_back( o.front()  );
    typename map< string, vector< T* > >::iterator mi =  domains.find( nm );
    vector< T* > &dm = mi->second;
    for ( i = o.begin(); i != o.end(); i++ ) dm.push_back( *i );
}

template< class T >
void DomainContainer<T>::GetDomainNames( vector<string> &dn ) const
{
    typename map< string, vector< T* > >::const_iterator i;
    dn.clear();
    for ( i = domains.begin();  i != domains.end(); i++ )
        dn.push_back( i->first );
}

template< class T >
const vector< T* >* DomainContainer<T>::GetDomain( const string &nd ) const
{
    typename map< string, vector< T* > >::const_iterator i = domains.find( nd );
    if ( i != domains.end() ) return &i->second;
    return 0;
}

template< class T >
vector< T* >* DomainContainer<T>::GetDomain( const string &nd )
{
    typename map< string, vector< T* > >::iterator i = domains.find( nd );
    if ( i != domains.end() ) return &i->second;
    return 0;
}


#endif // DOMAINCONTAINER_H
