#pragma once
#include "Expression.h"

class CExpressionAtan :
	public CExpression
{
public:
	CExpressionAtan(CExpression *expr);
	CExpressionAtan( const CExpressionAtan &exp ):CExpression(exp) { }
	virtual CExpressionAtan* Clone() const { return new CExpressionAtan( *this ); }
	virtual ~CExpressionAtan(void);

	CExpression* Differentiate( const string &wrt ) const;
	virtual double Compute() const { return sin( expr[0]->Compute() ); }
	virtual string ToString() const { return "sin("+expr[0]->ToString()+")"; }
};