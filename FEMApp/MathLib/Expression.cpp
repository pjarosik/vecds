#include "Expression.h"
//#include "StandException.h"
#include "ExpressionSum.h"
#include "ExpressionSubtraction.h"
#include "ExpressionProduct.h"
#include "ExpressionQuotient.h"
#include "ExpressionConstant.h"
#include "ExpressionBracket.h"
#include "ExpressionNegative.h"
#include "ExpressionVariable.h"

#include "ExpressionCosinus.h"
#include "ExpressionSinus.h"
#include "ExpressionTangens.h"
#include "ExpressionLn.h"
#include "ExpressionLog.h"
#include "ExpressionHeavisite.h"
#include "ExpressionAbs.h"
#include "ExpressionSquareroot.h"
#include "ExpressionPower.h"

#include "ExpressionPI.h"
#include "ExpressionE.h"

#include "ExpressionExp.h"
#include "ExpressionSinh.h"
#include "ExpressionCosh.h"
#include "ExpressionTanh.h"
#include "ExpressionAcos.h"
#include "ExpressionAsin.h"
#include "ExpressionAtan.h"
#include "ExpressionSgn.h"
#include "ExpressionRand.h"

CExpression::CExpression( const string &ex )
{
	Parse( ex );
}

CExpression::CExpression( const string &ex, const map< string, CVariable* > &vars  )
{
    Parse( ex );
    map< string, CVariable* >::const_iterator i;
    for (i=vars.begin(); i!=vars.end(); i++)
    {
        VariableDouble *vd = dynamic_cast< VariableDouble* >( i->second );
        if ( vd ) SetVariable( i->first, vd );
    }
}

CExpression::~CExpression( void )
{
	unsigned k;
	for (k=0; k<expr.size(); k++)
		delete expr[k];
}

CExpression::CExpression( const CExpression &exp )
{
	unsigned k;
	for (k=0; k<exp.expr.size(); k++)
		expr.push_back( exp.expr[ k ]->Clone() );
}

void CExpression::Parse( const string &ex )
{
	string expr( PassWhite( ex ) );
	if ( !expr.size() ) throw CMathException("Empty expression string");
		
	if ( FoundAddition( expr ) ) return;
	if ( FoundSubtraction( expr ) ) return;
	if ( FoundProduct( expr ) ) return;
	if ( FoundQuotient( expr ) ) return;
	if ( FoundPower( expr ) ) return;
	if ( FoundConstant( expr ) ) return;
	if ( FoundSign( expr ) ) return;
	if ( FoundBracket( expr ) ) return;
	if ( FoundVariable( expr ) ) return;

	throw CMathException( "Unknown token in expression :" + ex );
}

bool CExpression::FoundBracket( const string &ex )
{
	if ( ex[0] != '(' )	return false;
	int level = 0, k, b=-1;
	for (k=0; k<(int)ex.size(); k++)
	{
		if ( ex[k]=='(') 
		{
			if ( level == 0 ) b = k;
			level++;
		}
		if ( ex[k]==')') 
		{
			level--;
			if ( level == 0 )
			{
				if ( b==-1 ) 
					throw CMathException( "Close bracked without open in expression :" + ex );
				expr.push_back( new CExpressionBracket( ex.substr( b + 1, k - b - 1 ) ) );
				return true;
			}
		}
	}
	if ( b != -1 || level != 0 ) throw CMathException( "Unmatched bracket in expression :" + ex );
	return true;
}

bool CExpression::FoundSign( const string &ex )
{
	if ( ex[ 0 ] == '-' ) 
	{
		if ( ex.size() == 1 ) throw CMathException( "Nothing to negative in expression :" + ex );
		expr.push_back( new CExpressionNegative( ex.substr( 1, ex.size() - 1  ) ) );
		return true;
	}
	return false;
}

bool CExpression::FoundAddition( const string &ex )
{
	int pos = FindOperation( ex, '+' );
	if ( pos == 0 ) throw CMathException( "No first member in addition :" + ex );
	if ( pos == ex.size() - 1 ) throw CMathException( "No second member in addition :" + ex );
	if ( pos == -1 ) return false;
	expr.push_back( new CExpressionSum( ex.substr(0,pos), ex.substr(pos+1,ex.size()-pos-1) ) );
	return true;
}

bool CExpression::FoundSubtraction( const string &ex )
{
	int pos = FindOperation( ex, '-' );
	if ( pos == 0 ) 
	{
		throw CMathException( "No first member in subtraction :" + ex );
	}
	if ( pos == ex.size() - 1 ) throw CMathException( "No second member in subtraction :" + ex );
	if ( pos == -1 ) return false;
	vector< string > exprs;
	exprs.push_back( ex.substr(0,pos) );
	string rest = ex.substr(pos+1,ex.size()-pos-1);
	while( true )
	{
		pos = FindOperation( rest, '-' );
		if (pos==-1) 
		{ 
			exprs.push_back(rest);
			break;
		}
		exprs.push_back( rest.substr(0,pos) );
		rest = rest.substr(pos+1,rest.size()-pos-1);
	}

	expr.push_back( new CExpressionSubtraction( exprs ) );
	return true;
}

bool CExpression::FoundProduct( const string &ex )
{
	int pos = FindOperation( ex, '*' );
	if ( pos == 0 ) throw CMathException( "No first member in multiplication :" + ex );
	if ( pos == ex.size() - 1 ) throw CMathException( "No second member in multiplication :" + ex );
	if ( pos == -1 ) return false;
	expr.push_back( new CExpressionProduct( ex.substr(0,pos), ex.substr(pos+1,ex.size()-pos-1) ) );
	return true;
}

bool CExpression::FoundPower( const string &ex )
{
	int pos = FindOperation( ex, '^' );
	if ( pos == 0 ) throw CMathException( "No first member in power :" + ex );
	if ( pos == ex.size() - 1 ) throw CMathException( "No second member in power :" + ex );
	if ( pos == -1 ) return false;
	expr.push_back( new CExpressionPower( ex.substr(0,pos), ex.substr(pos+1,ex.size()-pos-1) ) );
	return true;
}

bool CExpression::FoundQuotient( const string &ex )
{
	int pos = FindOperation( ex, '/' );
	if ( pos == 0 ) 
	{
		throw CMathException( "No first member in quotient :" + ex );
	}
	if ( pos == ex.size() - 1 ) throw CMathException( "No second member in quotient :" + ex );
	if ( pos == -1 ) return false;
	vector< string > exprs;
	exprs.push_back( ex.substr(0,pos) );
	string rest = ex.substr(pos+1,ex.size()-pos-1);
	while( true )
	{
		pos = FindOperation( rest, '/' );
		if ( pos == -1 ) 
		{ 
			exprs.push_back(rest);
			break;
		}
		exprs.push_back( rest.substr(0,pos) );
		rest = rest.substr(pos+1,rest.size()-pos-1);
	}

	expr.push_back( new CExpressionQuotient( exprs ) );
	return true;
}


bool CExpression::FoundConstant( const string &ex )
{
	if ( isdigit(ex[0]) )
	{
		expr.push_back( new CExpressionConstant( ex ) );
		return true;
	}
	return false;
}

string CExpression::PassWhite( const string &ex )
{
	if ( ex[0] <= 32 && ex[ ex.size() -1 ] <= 23 ) return ex;
	unsigned b=0,e=ex.size()-1;
	while ( ex[ b ] <= 32 && b < e ) b++;
	while ( e && ex[ e ] <= 32 ) e--;
	return ex.substr( b, e - b + 1 );
}

int CExpression::FindOperation( const string &ex, char o )
{
	unsigned k = 0;
	int blev = 0;
	for( k = 0; k < ex.size(); k++)
	{
		if ( ex[ k ] == '(' ) blev++;
		else if ( ex[ k ] == ')' ) blev--;
			if ( ex[ k ] == o  && blev == 0 && !IsExp( ex, k ) && !IsSign(ex,k) ) return k;
	}
	return -1;
}

bool CExpression::FoundVariable( const string &ex )
{
	if ( isalpha ( ex[ 0 ] ) ) 
	{
		unsigned k=1;
		for (k=0; k<ex.size(); k++)
		{
			if ( ex[ k ] == '(' )
			{
				string name = ex.substr(0, k);
				vector< CExpression* > params;
				unsigned pp=k + 1, l = k, blev = 1;
				for ( l = k + 1; l < ex.size(); l++ )
				{
					if ( ex[l] == '(' ) blev++;
					if ( ex[l] == ')' ) blev--;
					
					if ( blev==0 && ( ex[l]==',' || ex[l]==')') ) 
					{
						params.push_back( new CExpression( ex.substr( pp, l - pp ) ) );
						pp = l + 1;
					}
				}
				switch( params.size() )
				{
					case 1 :	if ( name == "sin" ) expr.push_back( new CExpressionSinus( params[0] ) );
								else if ( name == "cos" ) expr.push_back( new CExpressionCosinus( params[0] ) );
								else if ( name == "tan"	) expr.push_back( new CExpressionTangens( params[0] ) );
								else if ( name == "ln"	) expr.push_back( new CExpressionLn( params[0] ) );
								else if ( name == "log" ) expr.push_back( new CExpressionLog( params[0] ) );
								else if ( name == "H" )	 expr.push_back( new CExpressionHeavisite( params[0] ) );
								else if ( name == "abs" ) expr.push_back( new CExpressionAbs( params[0] ) );
								else if ( name == "sqrt" ) expr.push_back( new CExpressionSquareroot( params[0] ) );
								else if ( name == "exp" )	expr.push_back( new CExpressionExp( params[0] ) );
								else if ( name == "sinh" ) 	expr.push_back( new CExpressionSinh( params[0] ) );
								else if ( name == "cosh" ) 	expr.push_back( new CExpressionCosh( params[0] ) );
								else if ( name == "tanh" ) 	expr.push_back( new CExpressionTanh( params[0] ) );
								else if ( name == "acos" ) 	expr.push_back( new CExpressionAcos( params[0] ) );
								else if ( name == "asin" ) 	expr.push_back( new CExpressionAsin( params[0] ) );
								else if ( name == "atan" ) 	expr.push_back( new CExpressionAtan( params[0] ) );
								else if ( name == "sgn" ) 	expr.push_back( new CExpressionSgn( params[0] ) );
								else if ( name == "rand" ) 	expr.push_back( new CExpressionRand( params[0] ) );
								else throw CMathException( "Unknown function :" + name ); 
								break;
					    
				}
				return true;

			}
			if ( ! ( isalpha( ex[ k ] ) || isdigit( ex[ k ] ) || ex[ k ]=='_' ) ) throw CMathException( "Variable name should consist of letters or digits :" + ex );
		}
		if ( ex == "pi" ) expr.push_back( new CExpressionPI );
		else if ( ex == "e" ) expr.push_back( new CExpressionE );
		else expr.push_back( new CExpressionVariable( ex ) );
		return true;
	}

	return false;
}

bool CExpression::DependsOn( const string &wrt ) const
{
	unsigned k;
	for (k=0; k<expr.size(); k++)
		if ( expr[ k ]->DependsOn( wrt ) ) return true;
	return false;
}

void CExpression::SetVariable( const string &name, VariableDouble *v )
{
	unsigned k;
	for (k=0; k<expr.size(); k++)
        expr[ k ]->SetVariable( name, v );
}

void CExpression::GetIdentifiers( vector< string > &ids ) const
{
	unsigned k;
	for (k=0; k<expr.size(); k++)
		expr[ k ]->GetIdentifiers( ids );
}
