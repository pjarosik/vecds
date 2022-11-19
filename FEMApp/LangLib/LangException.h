#pragma once

#include <string>
#include <istream>
using namespace std;

class CLangException
{
public:
	CLangException( istream *i, const string &m):line( GetLine( i ) ),error_msg( m ) {  }
	~CLangException(void);
	const string GetErrorMessage( );
	string GetLine( istream *i );
protected:
	istream *ist;
    string line, error_msg;
};


inline string CLangException::GetLine( istream *i ) 
{ 
	char buf[ 50000 ];
	buf[0]=0;
	i->getline( buf, 200, ';' );
	return buf; 
}

inline const string CLangException::GetErrorMessage() 
{ 
	return (string)"ERROR : " + error_msg + " \nIN LINE :\n" + line; 
}
