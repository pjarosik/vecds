#pragma once

#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <vector>
#include "LangException.h"
using namespace std;




class CLangStream
{
public:
	CLangStream( void ):line(0),index(0),alen(0),nlines(0),eof(false), eof_allowed( false ) { }
	
	virtual void GetData() =0;
	
	template < class V > void PassString();
	bool IsEof() const { return eof; }

	void SetEOFAllowed( bool a ) { eof_allowed = a; }
	
	char ReadChar( ) ;
	char GetChar(  ) const { return lines[ line ][ index ]; }
	template < class V > void ReadString( string &s );

	bool ExtractCommand( string &cmd );

	stringstream& GetIls() { return ils; }
	
protected:

	bool MoveCursor();
	vector< string > lines;
	stringstream ils;
	unsigned index, line, alen, nlines;
	bool eof, eof_allowed;
};

inline char CLangStream::ReadChar( ) 
{ 
	char c = lines[ line ][ index ];
	MoveCursor();
	return c;
}

inline bool CLangStream::MoveCursor()
{
	index++;
	if ( index == alen ) { index=0; line++; }
	if ( line == nlines )
		if ( !eof_allowed ) throw CLangException( &ils, "Unexpected end of file " );
		else  return false;
	return true;
}

template < class V > 
void CLangStream::PassString( )
{
	while ( V::Validate( GetChar() ) ) MoveCursor();
}

template < class V > 
void CLangStream::ReadString( string &s )
{
	while ( V::Validate( GetChar() ) )	s += ReadChar(); 
}