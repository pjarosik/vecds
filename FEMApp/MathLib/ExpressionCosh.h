#pragma once
#include "Expression.h"

class CExpressionCosh :
	public CExpression
{
public:
	CExpressionCosh(CExpression *expr);
	CExpressionCosh( const CExpressionCosh &exp ):CExpression( exp ) { }
	virtual CExpressionCosh* Clone() const { return new CExpressionCosh( *this ); }
	virtual ~CExpressionCosh(void);

	CExpression* Differentiate( const string &wrt ) const;
	virtual double Compute() const { return cosh( expr[0]->Compute() ); }
	virtual string ToString() const { return "cosh("+expr[0]->ToString()+")"; }
};
