#pragma once
#include "Expression.h"

class CExpressionSinus :
	public CExpression
{
public:
	CExpressionSinus(CExpression *expr);
	CExpressionSinus( const CExpressionSinus &exp ):CExpression(exp) { }
	virtual CExpressionSinus* Clone() const { return new CExpressionSinus( *this ); }
	virtual ~CExpressionSinus(void);

	CExpression* Differentiate( const string &wrt ) const;
	virtual double Compute() const { return sin( expr[0]->Compute() ); }
	virtual string ToString() const { return "sin("+expr[0]->ToString()+")"; }
};

