#pragma once
#include "Expression.h"
class CExpressionBracket :
	public CExpression
{
public:
	CExpressionBracket( const string &exp );
	CExpressionBracket( CExpression *exp ) { expr.push_back( exp ); }
	CExpressionBracket( const CExpressionBracket &exp ):CExpression(exp) { }
	virtual CExpressionBracket* Clone() const { return new CExpressionBracket( *this ); }
	virtual ~CExpressionBracket(void);
	virtual string ToString() const { return "(" + expr[ 0 ]->ToString() + ")"; }
	virtual CExpression* Differentiate( const string &wrt ) const;
};

