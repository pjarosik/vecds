#pragma once

#include <string>
#include <vector>
#include <map>
#include <cmath>
#include "SimpleVariables.h"

using namespace std;

class CExpression
{
public:
	
    CExpression( const string &ex );
    CExpression( const string &ex, const map< string, CVariable* > &vars );
	CExpression() { }
	CExpression( const CExpression &exp );
	virtual ~CExpression(void);
	virtual CExpression* Clone() const { return new CExpression( *this ); }
	virtual CExpression* Simplify( ) const { return expr[0]->Simplify(); }

    operator double() const { return  Compute(); }
	virtual double Compute() const { return expr[ 0 ]->Compute(); }
	virtual string ToString() const { return expr[ 0 ]->ToString(); }
    virtual void SetVariable ( const string &name, VariableDouble *v );
	virtual CExpression* Differentiate( const string &wrt ) const { return expr[0]->Differentiate(wrt); }
	virtual bool DependsOn( const string &wrt ) const;
	virtual void GetIdentifiers( vector< string > &ids ) const;

protected:

	vector< CExpression* > expr;

	void Parse( const string &ex );
	string PassWhite( const string &ex );
	int FindOperation( const string &ex, char o );
	bool IsExp( const string &ex, unsigned k ) const {  
		return ( 
				( k > 2 ) && 
				isdigit( ex[ k - 2 ] ) &&
				( ex[ k - 1 ] == 'E' || ex[ k - 1 ] == 'e' ) 
				); 
	}

	bool IsSign( const string &ex, unsigned k )const;
	
	bool FoundSign( const string &ex );
	bool FoundBracket( const string &ex );
	bool FoundAddition( const string &ex );
	bool FoundSubtraction( const string &ex );
	bool FoundProduct( const string &ex );
	bool FoundQuotient( const string &ex );
	bool FoundConstant( const string &ex );
	bool FoundVariable( const string &ex );
	bool FoundFunction( const string &ex );
	bool FoundPower( const string &ex );

};

inline bool CExpression::IsSign( const string &ex, unsigned k ) const 
{
	if ( k == 0 && ex[ k ] == '-' ) return true;
	while( --k )
		if ( ex[ k ] > 32 )
		{
			if ( ex[ k ]=='/' || ex[ k ]=='*' ) return true;
			else return false;
		}		
	return false;
}
