#pragma once
#include "Expression.h"
class CExpressionNegative :
	public CExpression
{
public:
	CExpressionNegative( const string &ex );
	CExpressionNegative( CExpression *ex ) { expr.push_back( ex ); }
	CExpressionNegative( const CExpressionNegative &exp ):CExpression(exp) { }
	virtual CExpressionNegative* Clone() const { return new CExpressionNegative( *this ); }
	virtual ~CExpressionNegative(void);

	virtual double Compute() const { return -expr[0]->Compute(); }
	virtual string ToString() const { return " -" +  expr[0]->ToString(); }
	virtual CExpression* Differentiate( const string &wrt ) const;
};

