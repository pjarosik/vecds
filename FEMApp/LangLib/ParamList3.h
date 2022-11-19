#pragma once

#include "LangException.h"
#include <string>
#include <vector>
#include <istream>
using namespace std;

template< char b, char e, class T1 , class T2, class T3 >
class CParamList3
{
	string name;
	T1 p1;
	T2 p2;
	T3 p3;
public:
	CParamList3(void)	{ 	}
	~CParamList3(void)	{	}
	istream& Read( istream &ist );
	const string& toString() const { return name; }
	const T1& GetP1() const { return p1; }
	const T2& GetP2() const { return p2; }
	const T3& GetP3() const { return p3; }
};

template< char b, char e, class T1 , class T2, class T3 >
istream& CParamList2< b, e, T1, T2, T3 >::Read ( istream &ist )
{                                 
	char c;
	ist >> c;
	if ( c != b ) throw CLangException( &ist, (string)"List opening character expected: " + b + (string)" instead of: " + c );
	ist >> p1;
	ist >> c;
	if ( c != ',' ) throw CLangException( &ist, (string)"list separator <,> expected instead of: " + c );
	ist >> p2;
	ist >> c;
	if ( c != ',' ) throw CLangException( &ist, (string)"list separator <,> expected instead of: " + c );
	ist >> p3;
	ist >> c;
	if ( c != e ) throw CLangException( &ist, (string)"List closing character expected: " + e + (string)" instead of: " + c );
	return ist;
}

template< char b, char e, class T1, class T2, class T3 >
istream& operator >> ( istream &ist, CParamList2< b, e, T1, T2, T3 > &npar )
{                                 
	npar.Read( ist );
	return ist;
}
