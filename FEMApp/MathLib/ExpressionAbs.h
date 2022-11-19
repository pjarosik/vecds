#pragma once
#include "Expression.h"

class CExpressionAbs :
	public CExpression
{
public:
	CExpressionAbs(CExpression *expr);
	CExpressionAbs( const CExpressionAbs &exp ):CExpression(exp) { }
	virtual CExpressionAbs* Clone() const { return new CExpressionAbs( *this ); }

	virtual ~CExpressionAbs(void);

	virtual double Compute() const { return fabs( expr[0]->Compute() ); }
	virtual string ToString() const { return "abs("+expr[0]->ToString()+")"; }
};

