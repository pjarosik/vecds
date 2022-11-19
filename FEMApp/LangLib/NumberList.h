#pragma once

#include <vector>
#include <istream>
#include "LangToken.h"
#include "CharValidators.h"
#include "LangException.h"
using namespace std;

template <char b, char s, char e>
class CNumberList :
	public vector<unsigned>
{
public:
	CNumberList():max(0) { }
	CNumberList(unsigned m):max(m) { }
	~CNumberList(void) { }
    void SetMaxRange( unsigned m )  { max = m; }
	istream& Read( istream &ist );
protected:
	unsigned max;
};

template <char b, char s, char e>
istream& CNumberList<b,s,e>::Read( istream  &ist )
{
    CLangToken<IsValidDigit>  i1, i2;
	char c;
    unsigned k = 0;
	ist >> c;

        if ( c  !=  b )  throw CLangException( &ist, (string)"Begin list expected :" + b );

        do
        {
            unsigned ib = 0, ie = 0;

            ist >>  c;

            if ( c == 'a' )
            {
                CLangToken< IsValidSmallLetter > all;
                ist  >>  all;
                if ( all.toString() == "ll" )
                {
                    ie = max - 1;
                    ist >>  c;
                    for (k=ib; k <= ie; k++)
                        push_back( k );
                    break ;
                }
		else throw CLangException( &ist, (string)"Unknown token: " + all.toString() + (string)" in numbers list" );
            }
            else  
            {
                 ist.putback( c );
                ist  >>  i1;
                ib = i1.toUnsigned();
            }

            ist >> c; 
            
            if ( c == '-' )
            {
                ist >> c;
                if ( c == 'a' )
                {
                    ist.putback( c );
                    CLangToken< IsValidSmallLetter > all;
                    ist  >>  all;
                    if ( all.toString() == "ll" )
                    {
                        ie = max - 1;
                        ist >> c;
                        for (k=ib; k <= ie; k++)
                            push_back( k );
                        break;
                    }
                    else throw CLangException( &ist, (string)"Unknown token: " + all.toString() + (string)" in numbers list" );
                }
                else
                {
                     ist.putback( c );
                    ist  >>  i2 ;
                    ie = i2.toUnsigned();
                }
            }
            else  
            {
                 ist.putback( c );
                 ie = ib;
            }


            for (k=ib; k <= ie; k++)
                push_back( k );

            ist >> c;

	}   while ( c == ',' );

        if ( c != e ) throw CLangException( &ist, (string)"List closing character expected: " + e + (string)" instead of: " + c );
	return ist;
};

template <char b, char s, char e>
istream& operator >> ( istream &ist, CNumberList<b,s,e> &npar )
{
	npar.Read( ist );
	return ist;
}
