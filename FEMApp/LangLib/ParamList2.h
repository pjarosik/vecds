#pragma once

#include "LangException.h"
#include <string>
#include <vector>
#include <istream>
using namespace std;

template< char b, char e, class T1 , class T2 >
class CParamList2
{
	string name;
	T1 p1;
	T2 p2;
public:
	CParamList2(void)	{ 	}
	~CParamList2(void)	{	}
	istream& Read( istream &ist );
	const string& toString() const { return name; }
	const T1& GetP1() const { return p1; }
	const T2& GetP2() const { return p2; }
};

template< char b, char e, class T1 , class T2 >
istream& CParamList2< b, e, T1, T2 >::Read ( istream &ist )
{                                 
	char c;
	ist >> c;
	if ( c != b ) throw CLangException( &ist, (string)"List opening character expected: " + b + (string)" instead of: " + c );
	ist >> p1;
	ist >> c;
	if ( c != ',' ) throw CLangException( &ist, (string)"list separator <,> expected instead of: " + c );
	ist >> p2;
	ist >> c;
	if ( c != e ) throw CLangException( &ist, (string)"List closing character expected: " + e + (string)" instead of: " + c );
	return ist;
}

template< char b, char e, class T1, class T2 >
istream& operator >> ( istream &ist, CParamList2< b, e, T1, T2 > &npar )
{                                 
	npar.Read( ist );
	return ist;
}
