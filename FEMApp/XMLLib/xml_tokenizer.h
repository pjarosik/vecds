#pragma once

#include <iostream>
#include <string>

using namespace std;

template <class T>
class XmlTokenizer
{
public:
	XmlTokenizer( istream &i ):inp( i ) { }
	XmlTokenizer& operator >> ( string &token );
	XmlTokenizer& operator >> ( char tc );
	char ReadChar( const char c );
	char ReadChar( const string & );
	void ReadString( const string & );
	void GetString( const string & );
	char GetChar( const char c );

protected:
	istream &inp;
};

template <class T>
char XmlTokenizer<T>::ReadChar( const char tc )
{
	char c;
	inp >> c;
	if ( c != tc ) throw CXML_Parser_Exception( inp, (string)"'" + tc + (string)"' character expected  instead of '" + c + (string)"'" );
	return c;
}

template <class T>
char XmlTokenizer<T>::ReadChar( const string &tc )
{
	char c;
	unsigned k;
	inp >> c;
	for (k=0; k<tc.size(); k++)
	{
		if ( c == tc[ k ] ) return c;
	}
	throw CXML_Parser_Exception( inp, (string)"'" + tc + (string)"' character expected  instead of '" + c + (string)"'" );
	
}

template <class T>
void XmlTokenizer<T>::ReadString( const string &tc )
{
	string reads;
	unsigned k;
	ReadChar( tc[ 0 ] );
	reads += tc[ 0 ];
	for ( k = 1; k < tc.size(); k++ ) 
		if ( !inp.eof() ) reads += inp.get();
	
	if ( tc != reads ) 
		throw CXML_Parser_Exception( inp, " string :" + tc + " expected  instead of :" + reads );
	
}

template <class T>
void XmlTokenizer<T>::GetString( const string &tc )
{
	char c;
	unsigned k;
	for (k=0; k<tc.size(); k++) GetChar( tc[ k ] );
}

template <class T>
char XmlTokenizer<T>::GetChar( const char tc )
{
	char c;
	c = inp.get();
	if ( c != tc ) throw CXML_Parser_Exception( inp, (string)"'" + tc + (string)"' character expected  instead of '" + c + (string)"'" );
	return c;
}

template <class T>
inline XmlTokenizer<T>& XmlTokenizer<T>::operator >> ( string &token )
{
	char c;
	token.clear();
	inp >> c;
	while( T::IsValid( c ) ) 
	{
		token += c;
		c = inp.get();
		if ( inp.eof( ) ) return *this;
	}
	inp.putback(c);
	return *this;
}