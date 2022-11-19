#pragma once
#include "Expression.h"

class CExpressionTanh :
	public CExpression
{
public:
	CExpressionTanh(CExpression *expr);
	CExpressionTanh( const CExpressionTanh &exp ):CExpression(exp) { }
	virtual CExpressionTanh* Clone() const { return new CExpressionTanh( *this ); }
	virtual ~CExpressionTanh(void);

	CExpression* Differentiate( const string &wrt ) const;
	virtual double Compute() const { return tanh( expr[0]->Compute() ); }
	virtual string ToString() const { return "tanh("+expr[0]->ToString()+")"; }
};

