#pragma once
#include "Expression.h"

class CExpressionSum :
	public CExpression
{
public:
	CExpressionSum( const string &ex1, const string &ex2 );
	CExpressionSum( CExpression *ex1, CExpression *ex2 ) {  expr.push_back( ex1 ); expr.push_back( ex2 ); }
	CExpressionSum( const CExpressionSum &exp ):CExpression(exp) { }
	virtual CExpressionSum* Clone() const { return new CExpressionSum( *this ); }
	virtual ~CExpressionSum(void);
	virtual double Compute() const { return expr[0]->Compute() + expr[1]->Compute(); }
	virtual string ToString() const { return expr[0]->ToString() + "+" +  expr[1]->ToString(); }
	virtual CExpression* Differentiate( const string &wrt ) const;
	virtual CExpression* Simplify( ) const;
};

