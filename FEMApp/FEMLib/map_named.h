#pragma once

#include <map>
#include <string>
using namespace std;

template<class M>
class map_named_ptr :
	public map<string,M*>
{
    
public:

    using map< string, M* >::iterator;
    using map< string, M* >::find;
    using map< string, M* >::begin;
    using map< string, M* >::end;
    using map< string, M* >::clear;

	map_named_ptr( void ) {	}
	~map_named_ptr( void ) { DeleteObjects(); }
	void Reset() { DeleteObjects(); }

	bool Check( const string &nm ) const { return map<string,M*>::find( nm ) != map<string,M*>::end(); }
	const M* Find( const string &nm ) const;
        M* Find( const string &nm );
	M* GetObject( const string &nm );

protected:

	void DeleteObjects();
};

template < class M >
const inline M* map_named_ptr< M >::Find( const  string &nm ) const
{
   	typename map< string, M* >::const_iterator i;
        i = find( nm );
	if ( i != end( ) ) return i->second;
	return 0;
} 


template < class M >
inline M* map_named_ptr< M >::Find( const  string &nm ) 
{
   	typename map< string, M* >::iterator i;
        i = find( nm );
	if ( i != end( ) ) return i->second;
	return 0;
} 

template < class M >
M* map_named_ptr< M >::GetObject( const string &nm )  
{ 
	M *fo = Find( nm );
	if ( !fo ) fo = new M;
	(*this)[nm]=fo;
	return fo;
} 

template < class M >
void map_named_ptr< M >::DeleteObjects( )  
{ 
	typename map< string, M* >::iterator i;
	for(i=begin(); i!=end(); i++)
		delete i->second;
	clear();
} 
