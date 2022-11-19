#include "ExpressionProduct.h"
#include "ExpressionQuotient.h"
#include "ExpressionConstant.h"
#include "ExpressionSum.h"

CExpressionProduct::CExpressionProduct(const string &ex1, const string &ex2)
{
	Parse(ex1);
	Parse(ex2);
}


CExpressionProduct::~CExpressionProduct(void)
{
}

CExpression* CExpressionProduct::Differentiate( const string &wrt ) const
{
	return new CExpressionSum( 
							new CExpressionProduct( expr[0]->Clone(), 
												expr[1]->Differentiate(wrt) ),
												
							new CExpressionProduct( expr[1]->Clone(), 
												expr[0]->Differentiate(wrt))
							);
																							
}

CExpression* CExpressionProduct::Simplify( ) const
{
	
	CExpression *expr0 = expr[0]->Simplify();
	CExpression *expr1 = expr[1]->Simplify();

	if ( expr1->ToString() == "1") { delete expr1; return expr0; }
	if ( expr0->ToString() == "1") { delete expr0; return expr1; }

	if ( expr0->ToString() == "0") { delete expr1; delete expr0; return new CExpressionConstant(0.0); }
	if ( expr1->ToString() == "0") { delete expr1; delete expr0; return new CExpressionConstant(0.0); }

	return new CExpressionProduct( expr0, expr1 );

}