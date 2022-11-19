#pragma once
#include "Expression.h"

class CExpressionTangens :
	public CExpression
{
public:
	CExpressionTangens(CExpression *expr);
	CExpressionTangens( const CExpressionTangens &exp ):CExpression(exp) { }
	virtual CExpressionTangens* Clone() const { return new CExpressionTangens( *this ); }
	virtual ~CExpressionTangens(void);

	virtual double Compute() const { return tan( expr[0]->Compute() ); }
	virtual string ToString() const { return "tan("+expr[0]->ToString()+")"; }
	CExpression* Differentiate( const string &wrt ) const;
};

