#pragma once
#include "Expression.h"

class CExpressionE :
	public CExpression
{
public:
	CExpressionE( void );
	virtual ~CExpressionE( void );
	virtual CExpressionE* Clone( ) const { return new CExpressionE( *this ); }
	virtual CExpression* Differentiate( const string &wrt ) const { return new CExpression("0"); }
	virtual double Compute() const { return M_E; }
	virtual string ToString() const { return "e"; }
};

