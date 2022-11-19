#pragma once
#include "Expression.h"

class CExpressionCosinus :
	public CExpression
{
public:
	CExpressionCosinus(CExpression *expr);
	CExpressionCosinus( const CExpressionCosinus &exp ):CExpression(exp) { }
	virtual CExpressionCosinus* Clone() const { return new CExpressionCosinus( *this ); }
	virtual ~CExpressionCosinus(void);

	virtual double Compute() const { return cos( expr[0]->Compute() ); }
	virtual string ToString() const { return "cos("+expr[0]->ToString()+")"; }
	virtual CExpression* Differentiate( const string &wrt ) const;
};

