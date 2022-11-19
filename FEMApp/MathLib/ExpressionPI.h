#pragma once
#include "Expression.h"


class CExpressionPI :
	public CExpression
{
public:
	CExpressionPI( void );
	virtual ~CExpressionPI( void );
	virtual CExpressionPI* Clone( ) const { return new CExpressionPI( *this ); }
	virtual CExpression* Differentiate( const string &wrt ) const { return new CExpression("0"); }
	virtual double Compute() const { return M_PI; }
	virtual string ToString() const { return "pi"; }
};

