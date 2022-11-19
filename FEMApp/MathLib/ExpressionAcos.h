#pragma once
#include "Expression.h"

class CExpressionAcos :
	public CExpression
{
public:
	CExpressionAcos(CExpression *expr);
	CExpressionAcos( const CExpressionAcos &exp ):CExpression(exp) { }
	virtual CExpressionAcos* Clone() const { return new CExpressionAcos( *this ); }
	virtual ~CExpressionAcos(void);

	CExpression* Differentiate( const string &wrt ) const;
	virtual double Compute() const { return acos( expr[0]->Compute() ); }
	virtual string ToString() const { return "acos("+expr[0]->ToString()+")"; }
};
