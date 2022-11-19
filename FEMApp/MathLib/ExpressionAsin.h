#pragma once
#include "Expression.h"


class CExpressionAsin :
	public CExpression
{
public:
	CExpressionAsin(CExpression *expr);
	CExpressionAsin( const CExpressionAsin &exp ):CExpression(exp) { }
	virtual CExpressionAsin* Clone() const { return new CExpressionAsin( *this ); }
	virtual ~CExpressionAsin(void);

	CExpression* Differentiate( const string &wrt ) const;
	virtual double Compute() const { return asin( expr[0]->Compute() ); }
	virtual string ToString() const { return "asin("+expr[0]->ToString()+")"; }
};


