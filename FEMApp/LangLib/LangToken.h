#pragma once

#include <string>
#include <istream>
#include <cstdlib>
#include <sstream>
using namespace std;

template<class T>
class CLangToken
{
	string token;
    bool isread;
public:
    CLangToken( const string &t= "" ):token(t),isread(false)	{ 	}
	~CLangToken(void)	{	}
	istream& Read( istream &ist );
    bool ReadIfValid( istream &ist, string valid );
    bool IsRead() const { return isread; }
	const string& toString() const { return token; }
    double toDouble() const;
	unsigned toUnsigned() const { return (unsigned)atoi(token.c_str()); }
    bool operator == ( const string &t ) const { return t == token; }
	bool operator == ( const CLangToken<T> &t ) const; 
	bool operator != ( const CLangToken<T> &t ) const { return t.toString() != token; }
    const string& operator =( const string &st ) { token=st; return st; }
};

template< class T >
inline double CLangToken< T >::toDouble() const
{
    std::istringstream sstr(token);
    double val;
    sstr >> val;
    return val;
}

template< class T >
inline bool CLangToken< T >::operator == ( const CLangToken<T> &t ) const
{ 
	return t.toString() == token; 
}

template< class T >
istream& CLangToken< T >::Read ( istream &ist )
{
    if (isread) token="";
    isread=true;
    if ( token != "" )
    {
        ReadIfValid( ist, token );
        return ist;
    }
	char c;
	ist >> c;
    if ( ist.eof() ) return ist;
	token.clear();
	while( T::Validate( c ) ) 
	{ 
		token += c;
		ist.get(c); 
	}
	ist.putback( c );
	return ist;
}

template< class T >
bool CLangToken< T >::ReadIfValid( istream &ist, string valid )
{
    unsigned ind = 0;
    int k;
	char c;
	ist >> c;
    isread=false;
    if ( ist.eof() ) return false;
	while( T::Validate( c ) ) 
	{ 
		if ( c != valid[ ind++ ] )
		{
			ist.putback( c );
            for ( k = ind - 2; k >= 0; k-- )
				ist.putback( valid[ k ] );
			return false;

		}
		ist.get(c); 
	}
	ist.putback( c );
	token = valid;
    isread=true;
	return true;
}

template< class T >
istream& operator >> ( istream &ist, CLangToken< T > &npar )
{                                 
	npar.Read( ist );
	return ist;
}
