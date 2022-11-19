#pragma once
#include "Expression.h"

class CExpressionHeavisite :
	public CExpression
{
public:
	CExpressionHeavisite(CExpression *expr);
	CExpressionHeavisite( const CExpressionHeavisite &exp ):CExpression( exp ) { }
	virtual CExpressionHeavisite* Clone() const { return new CExpressionHeavisite( *this ); }
	virtual ~CExpressionHeavisite(void);

	virtual double Compute() const { return expr[0]->Compute() > 0 ? 1.0 : 0.0; }
	virtual string ToString() const { return "H("+expr[0]->ToString()+")"; }
};

