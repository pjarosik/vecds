#pragma once
#include "Expression.h"

class CExpressionConstant :
	public CExpression
{
	double value;
public:

	CExpressionConstant( const string &ex);
	CExpressionConstant( double v ) { value =v; }
	CExpressionConstant( const CExpressionConstant &exp ):CExpression(exp),value(exp.value) { }
	virtual CExpressionConstant* Clone() const { return new CExpressionConstant( *this ); }
	virtual ~CExpressionConstant(void);

	virtual double Compute() const { return value; }
	virtual string ToString() const;
	virtual CExpression* Differentiate( const string &wrt ) const { return new CExpression("0"); }
	virtual bool DependsOn( const string &wrt ) const { return false; }
	virtual CExpression* Simplify( ) const { return Clone(); }
};

