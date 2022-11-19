#pragma once
#include "Expression.h"

class CExpressionSgn :
	public CExpression
{
public:
	CExpressionSgn(CExpression *expr);
	CExpressionSgn( const CExpressionSgn &exp ):CExpression(exp) { }
	virtual CExpressionSgn* Clone() const { return new CExpressionSgn( *this ); }
	virtual ~CExpressionSgn(void);

	virtual CExpression* Differentiate( const string &wrt ) const { return new CExpression("0"); }
	virtual double Compute() const { double v = expr[0]->Compute(); return v == 0.0 ? 0.0 : v / fabs( v ); }
	virtual string ToString() const { return "sgn("+expr[0]->ToString()+")"; }
};
