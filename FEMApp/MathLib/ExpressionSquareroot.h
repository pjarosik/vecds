#pragma once
#include "Expression.h"

class CExpressionSquareroot :
	public CExpression
{
public:
	CExpressionSquareroot(CExpression *expr);
	CExpressionSquareroot( const CExpressionSquareroot &exp ):CExpression(exp) { }
	virtual CExpressionSquareroot* Clone() const { return new CExpressionSquareroot( *this ); }
	virtual ~CExpressionSquareroot(void);

	virtual double Compute() const { return sqrt( expr[0]->Compute() ); }
	virtual string ToString() const { return "sqrt("+expr[0]->ToString()+")"; }
	CExpression* Differentiate( const string &wrt ) const;
};

