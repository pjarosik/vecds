#pragma once

#include <vector>
#include <istream>
#include "statdef.h"
using namespace std;

template< char b, char e, class T >
class CSubSetList
{
	
public:

	CSubSetList( const vector<T> &a, vector<T> &ss ):all_elems( a ),sub_set(ss)	{	}
	~CSubSetList( void ) {	}
	istream& Read( istream &ist );

protected:

	class interval
	{
	public :
		interval():i1(0), i2(0) {  }
		bool IsOverlapped( const interval &in );
		unsigned i1, i2;
	};

	unsigned GetNumber( istream &ist ) const;
	const vector<T> &all_elems;
	vector<T> &sub_set;
	vector< interval > ints;

};

template< char b, char e, class T >
bool CSubSetList< b, e, T >::interval::IsOverlapped( const interval &in )
{
	if ( ( i1 <= in.i1 ) && ( in.i1 <= i2 ) ) return false;
	if ( ( i1 <= in.i2 ) && ( in.i2 <= i2 ) ) return false;

	if ( ( in.i1 <= i1 ) && ( i1 <= in.i2 ) ) return false;
	if ( ( in.i1 <= i2 ) && ( i2 <= in.i2 ) ) return false;

	return true;
}

template< char b, char e, class T >
unsigned CSubSetList< b, e, T >::GetNumber( istream &ist ) const
{
	char c;
	ist >> c;
	if ( c == 'a' ) return all_elems.size() - 1;

    if ( !IsValidDigit::Validate( c ) )
		throw CLangException( &ist, (string)"Malformed number list, 'a' or integer number was expected.");
	ist.putback(c);
	unsigned number;
	ist >> number;
	if ( ist.fail( ) )
		throw CLangException( &ist, (string)"Malformed number list, integer number was expected.");

	return number;
}


template< char b, char e, class T >
istream& CSubSetList< b, e, T >::Read ( istream &ist )
{          
	sub_set.clear();
	char c;
	ist >> c;
	if ( c != b ) throw CLangException( &ist, (string)"List opening character expected: " + b + (string)" instead of: " + c );
	ints.clear();
	interval in;
	unsigned k, l;
	do
	{ 
		
		in.i1 = GetNumber( ist );
		ist >> c;
		if ( c == '-' )	
		{
			in.i2 = GetNumber( ist );
		}
		else
		{
			in.i2 = in.i1;
		}

		if ( in.i2 < in.i1 )
			throw CLangException( &ist, "Unproper interval definition in list i1" );
		for (l=0; l<ints.size(); l++)
			if ( ints[l].IsOverlapped( in ) ) throw CLangException( &ist, "Overlapped intervals in index list" );
		ints.push_back( in );

	} while ( c == ',' );
	if ( c != e ) throw CLangException( &ist, (string)"List closing character expected: " + e + (string)" instead of: " + c );

	sub_set.clear();
	for (k=0; k<ints.size(); k++)
		for ( l=ints[ k ].i1; l<=ints[ k ].i2; l++)
			sub_set.push_back( all_elems[ l ] );

	return ist;
}

template< char b, char e, class T >
istream& operator >> ( istream &ist, CSubSetList< b, e, T > &npar )
{                                 
	npar.Read( ist );
	return ist;
}
