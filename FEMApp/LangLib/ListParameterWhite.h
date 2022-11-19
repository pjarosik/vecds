#pragma once

#include "LangException.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

template< char b, char e, class T >
class CListParameterWhite
{
	string name;
	vector<T> list;
public:
	CListParameterWhite(void)	{ 	}
	~CListParameterWhite(void)	{	}
	istream& Read( istream &ist );
	const string& toString() const { return name; }
	const vector<T>& GetList() const { return list; }
};

template< char b, char e, class T >
istream& CListParameterWhite< b, e, T >::Read ( istream &ist )
{                                 
	char c;
	ist >> c;
//	cout << "begin :" << c << endl;
	if ( c != b ) throw CLangException( &ist, (string)"List opening character expected: " + b + (string)" instead of: " + c );
	list.clear();
	streampos phere = ist.tellg();
	do
	{ 
		T item;
		ist >> item;
                if ( (ist.rdstate() & ifstream::failbit ) != 0 )
                       throw CLangException( &ist, "istream reading failed" );

		streampos here = ist.tellg();
		list.push_back( item );
		ist >> c;
		ist.putback( c );
//		cout << "item :" << item << " end:" << c << " pos :" << here << " size :" << here - phere << endl;
		phere=here;
	} while ( c != e );
	ist >> c;
	return ist;
}

template< char b, char e, class T >
istream& operator >> ( istream &ist, CListParameterWhite< b,e,T > &npar )
{                                 
	npar.Read( ist );
	return ist;
}
