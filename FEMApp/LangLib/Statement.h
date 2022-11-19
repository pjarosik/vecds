#pragma once

#include <sstream>
#include <iostream>
#include <string>
using namespace std;


#include "LangException.h"
#include "CharValidators.h"
#include "LangToken.h"
#include "NamedParameter.h"
#include "ListParameter.h"
#include "statdef.h"

class CStatement
{
public:
	CStatement();
	virtual ~CStatement(void);
	virtual void Execute( istream &i ) = 0;

    template<class T> void toStringVector( const vector<T> &s, vector<string> &t )  { unsigned k; t.clear(); for ( k=0; k<s.size(); k++) t.push_back( s[k].toString() ); }
    template<class T> void toIntVector( const vector<T> &s, vector<string> &t )     { unsigned k; t.clear(); for ( k=0; k<s.size(); k++) t.push_back( s[k].toInt() ); }
    template<class T> void toDoubleVector( const vector<T> &s, vector<string> &t )  { unsigned k; t.clear(); for ( k=0; k<s.size(); k++) t.push_back( s[k].toDouble() ); }

protected:
	bool CheckChar( istream &i, char c, bool call_exception = true ) const;
};



inline bool CStatement::CheckChar( istream &in, char c, bool call_exception ) const
 { 
	 char rc;
	 in >> rc; 
	 if ( c != '{' ) 
	 {
		 if ( call_exception ) throw CLangException( &in, (string)(c + " expected") );
		 else return false;
	 }
	 return true;
 }

inline istream& operator >> ( istream &i, CStatement *st )
{
	if ( st ) st->Execute( i );
	return i;
}

inline istream& operator >> ( istream &i, CStatement &st )
{
	st.Execute( i );
	return i;
}
