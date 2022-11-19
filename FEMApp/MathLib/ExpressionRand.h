#pragma once
#include "Expression.h"
//#include "../CommonLib/VRND.h"

class CExpressionRand :
	public CExpression
{
public:
	CExpressionRand(CExpression *expr);
	CExpressionRand( const CExpressionRand &exp ):CExpression(exp) { }
	virtual CExpressionRand* Clone() const { return new CExpressionRand( *this ); }
	virtual ~CExpressionRand(void);

	CExpression* Differentiate( const string &wrt ) const;
	virtual double Compute() const;
	virtual string ToString() const { return "rand("+expr[0]->ToString()+")"; }
};
