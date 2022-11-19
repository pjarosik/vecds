#pragma once
#include "Expression.h"

class CExpressionSubtraction :
	public CExpression
{
public:
	CExpressionSubtraction( const string &ex1, const string &ex2 );
	CExpressionSubtraction( const vector<string> &exs );
	CExpressionSubtraction( const vector<CExpression*> &exs ) { expr = exs; }
	CExpressionSubtraction( CExpression *ex1, CExpression *ex2 ) {  expr.push_back( ex1 ); expr.push_back(ex2); }
	CExpressionSubtraction( const CExpressionSubtraction &exp ):CExpression(exp) { }
	virtual CExpressionSubtraction* Clone() const { return new CExpressionSubtraction( *this ); }
	virtual ~CExpressionSubtraction( void );
	virtual double Compute() const;
	virtual string ToString() const;
	virtual CExpression* Differentiate( const string &wrt ) const;
	virtual CExpression* Simplify( ) const;
};

