#pragma once
#include "Expression.h"

class CExpressionProduct :
	public CExpression
{
public:
	CExpressionProduct( const string &ex1, const string &ex2 );
	CExpressionProduct( CExpression *ex1, CExpression *ex2 ) {  expr.push_back( ex1 ); expr.push_back( ex2 ); }
	CExpressionProduct( const CExpressionProduct &exp ):CExpression( exp ) { }
	virtual CExpressionProduct* Clone() const { return new CExpressionProduct( *this ); }
	virtual ~CExpressionProduct(void);

	virtual double Compute() const { return expr[0]->Compute() * expr[1]->Compute(); }
	virtual string ToString() const { return expr[0]->ToString() + "*" +  expr[1]->ToString(); }
	virtual CExpression* Differentiate( const string &wrt ) const;
	virtual CExpression* Simplify( ) const;

};

