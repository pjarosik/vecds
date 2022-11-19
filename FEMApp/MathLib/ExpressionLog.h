#pragma once
#include "Expression.h"

class CExpressionLog :
	public CExpression
{
public:
	CExpressionLog(CExpression *expr);
	CExpressionLog( const CExpressionLog &exp ):CExpression(exp) { }
	virtual CExpressionLog* Clone() const { return new CExpressionLog( *this ); }
	virtual ~CExpressionLog(void);

	virtual double Compute() const { return log10( expr[0]->Compute() ); }
	virtual string ToString() const { return "log("+expr[0]->ToString()+")"; }
	virtual CExpression* Differentiate( const string &wrt ) const;
};

