#pragma once
#include "Expression.h"

class CExpressionQuotient :
	public CExpression
{
public:
	CExpressionQuotient( const string &ex1, const string &ex2);
	CExpressionQuotient( const vector< string > &exs );
	CExpressionQuotient( const vector< CExpression* > &exs ) { expr = exs; }
	CExpressionQuotient( CExpression *ex1, CExpression *ex2 ) {  expr.push_back( ex1 ); expr.push_back( ex2 ); }
	CExpressionQuotient( const CExpressionQuotient &exp ):CExpression(exp) { }
	virtual CExpressionQuotient* Clone() const { return new CExpressionQuotient( *this ); }
	virtual ~CExpressionQuotient( void );

	virtual double Compute() const;
	virtual string ToString() const;

	CExpression* Differentiate( const string &wrt ) const;
	CExpression* Simplify( ) const;

};

