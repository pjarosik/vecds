#pragma once

#include "LangException.h"
#include <string>
#include <vector>
#include <istream>
using namespace std;


template< char b, char e, class T, bool dex = true >
class CListParameter
{
    string name;
    vector< T > list;

public:

    CListParameter( bool x = dex ):ex( x ), isread( false ) { 	}
    ~CListParameter( void )	{	}
	istream& Read( istream &ist );
	const vector<T>& GetList() const { return list; }
    void AddItem( const T &i ) { list.push_back( i ); }
    bool IsRead() const { return isread; }
    const T& operator[](unsigned i) const { return list[i]; }
    T& operator[](unsigned i) { return list[i]; }
//    template< class C > operator const C();

protected:

    bool ex, isread;
};

/*
template< char b, char e, class T, bool dex >
template< class C>
CListParameter< b, e, T, dex >::operator const C()
{
    C rc( list.size() );
    unsigned k;
    for (k=0; k<list.size(); k++)
        rc[ k ] = list[ k ];
    return rc;
}
*/

template< char b, char e, class T, bool dex  >
istream& CListParameter< b, e, T, dex >::Read ( istream &ist )
{                                 
    isread = false;
	char c;
	ist >> c;
    if ( c != b )
    {
        if (ex) throw CLangException( &ist, (string)"List opening character expected \'" + b + (string)"\' instead of: " + c );
        else
        {
            ist.putback(c);
            return ist;
        }
    }
	list.clear();
	do
	{ 
		T item;
		ist >> item;
		list.push_back( item );
		ist >> c;
	} while ( c == ',' );
    if ( c != e ) throw CLangException( &ist, (string)"List closing character expected \'" + e + (string)"\' instead of: " + c );
    isread = true;
	return ist;
}


template< char b, char e, class T, bool dex >
istream& operator >> ( istream &ist, CListParameter< b,e,T, dex > &npar )
{                                 
	npar.Read( ist );
	return ist;
}


