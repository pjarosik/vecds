#pragma once
#include "Expression.h"

class CExpressionSinh :
	public CExpression
{
public:
	CExpressionSinh(CExpression *expr);
	CExpressionSinh( const CExpressionSinh &exp ):CExpression( exp ) { }
	virtual CExpressionSinh* Clone() const { return new CExpressionSinh( *this ); }
	virtual ~CExpressionSinh(void);

	CExpression* Differentiate( const string &wrt ) const;
	virtual double Compute() const { return sinh( expr[0]->Compute() ); }
	virtual string ToString() const { return "sinh("+expr[0]->ToString()+")"; }
};
