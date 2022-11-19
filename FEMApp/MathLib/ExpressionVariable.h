#pragma once
#include "ExpressionConstant.h"
#include "Variable.h"
#include "SimpleVariables.h"
#include "MathException.h"

class CExpressionVariable :
	public CExpression
{

	const string name;
    VariableDouble *variable;

public:

	CExpressionVariable( const string &nm );
	CExpressionVariable( const CExpressionVariable &exp ):CExpression( exp ),variable( exp.variable ), name( exp.name ) { }
	virtual CExpressionVariable* Clone() const { return new CExpressionVariable( *this ); }
	virtual ~CExpressionVariable( void );

	virtual double Compute() const 
	{ 
        if ( !variable ) throw CMathException( "Undefined variable :" + name );
        return variable->GetValue();
	}
	virtual string ToString() const { return name; }
    virtual void SetVariable ( const string &vname, VariableDouble *v )
	{ 
        if ( name == vname ) variable = v;
	}
	virtual CExpression* Differentiate( const string &wrt ) const { return wrt == name ? new CExpressionConstant( 1.0 ) : new CExpressionConstant( 0.0 ); }
	virtual CExpression* Simplify( ) const { return Clone(); }
	virtual bool DependsOn( const string &wrt ) const 
	{ 
        return name == wrt;
	}
//	virtual void GetIdentifiers( vector< string > &ids )  const { if ( find( ids.begin(),ids.end(), name )==ids.end() ) ids.push_back( name ); }

};

