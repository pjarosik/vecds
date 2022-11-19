#pragma once
#include "Expression.h"

class CExpressionPower :
	public CExpression
{
public:

	CExpressionPower( const string &ex1, const string &ex2 );
	CExpressionPower( CExpression *ex1, CExpression *ex2 ) {  expr.push_back( ex1 ); expr.push_back( ex2 ); }
	CExpressionPower( const CExpressionPower &exp ):CExpression( exp ) { }
	virtual CExpressionPower* Clone() const { return new CExpressionPower( *this ); }
	virtual ~CExpressionPower(void);

	virtual double Compute() const { return pow( expr[0]->Compute() , expr[1]->Compute() ); }
	virtual string ToString() const { return expr[0]->ToString() + "^" +  expr[1]->ToString(); }
	virtual CExpression* Differentiate( const string &wrt ) const;
};

