#pragma once
#include <string>
#include <istream>
#include "LangStream.h"
using namespace std;

template< class N, char ac, class T >
class CNamedParameter
{
	N name, validname;
	T param;
    bool validate, isread, ex;

public:

    CNamedParameter( void ):validname( "" ), validate( false ), isread(false)	{	}
    CNamedParameter( const N &vn, bool vd = true, bool e = true ):validname( vn ), ex( e ), validate( vd )	{	}
	~CNamedParameter( void )	{	}
	istream& Read( istream &ist );
    const string& GetStringName() const { return name.toString(); }
	const N& GetName() const { return name; }
	const T& GetParam() const { return param; }
    T& GetParam() { return param; }
    operator const T&() const { return param; }
    bool IsRead() const { return isread; }

};


template< class N, char ac, class T >
istream& CNamedParameter< N, ac, T >::Read ( istream &ist )
{                                 
    ist >> name;
    if ( name != validname )
    {
        isread = false;
        if ( validate  )
        {
            if ( ex )
            {
                throw CLangException( &ist, "Wrong parameter name '" + name.toString() + "'.' Expected parameter name is '" + validname.toString() +"'");
            }
            else
            {
                string nb = name.toString();
                string::reverse_iterator i;
                for (i = nb.rbegin(); i!=nb.rend(); i++)
                    ist.putback(*i);
                return ist;
            }
        }
    }
    char c=' ';
    ist >> c;
	if ( c == ac ) ist >> param; 
	else 
	{
		stringstream ss;
        ss << " assign char \'" << ac << "\' is expected instead of \'" << c << "\' in named parameter " << name.toString();
		throw CLangException( &ist, ss.str() );
	}
    isread = true;
	return ist;
}

template< class N, char ac, class T >
istream& operator >> ( istream &ist, CNamedParameter< N, ac, T > &npar )
{                                 
	npar.Read(ist);
	return ist;
}
