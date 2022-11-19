#pragma once
#include "Expression.h"

class CExpressionLn : public CExpression
{
public:
	CExpressionLn(CExpression *expr);
	CExpressionLn( const CExpressionLn &exp ):CExpression(exp) { }
	virtual CExpressionLn* Clone() const { return new CExpressionLn( *this ); }
	virtual ~CExpressionLn(void);

	virtual double Compute() const { return log( expr[0]->Compute() ); }
	virtual string ToString() const { return "ln("+expr[0]->ToString()+")"; }
	virtual CExpression* Differentiate( const string &wrt ) const;

};

