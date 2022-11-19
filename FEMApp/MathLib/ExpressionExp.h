#pragma once
#include "Expression.h"

class CExpressionExp :
	public CExpression
{
public:
	CExpressionExp( CExpression *expr );
	CExpressionExp( const CExpressionExp &exp ):CExpression( exp ) { }
	virtual CExpressionExp* Clone() const { return new CExpressionExp( *this ); }
	virtual ~CExpressionExp(void);

	CExpression* Differentiate( const string &wrt ) const;
	virtual double Compute() const { return exp( expr[ 0 ]->Compute() ); }
	virtual string ToString() const { return "exp("+expr[ 0 ]->ToString()+")"; }
};

