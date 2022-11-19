#include "ExpressionSum.h"
#include "ExpressionProduct.h"
#include "ExpressionConstant.h"

CExpressionSum::CExpressionSum( const string &ex1, const string &ex2 )
{
	Parse( ex1 );
	Parse( ex2 );
}


CExpressionSum::~CExpressionSum( void )
{
}

CExpression* CExpressionSum::Differentiate( const string &wrt ) const 
{ 
	return new CExpressionSum(  expr[0]->Differentiate( wrt ), expr[1]->Differentiate( wrt )  ); 
}

CExpression* CExpressionSum::Simplify( ) const
{
	
	CExpression *expr0 = expr[0]->Simplify();
	CExpression *expr1 = expr[1]->Simplify();

	if ( expr0->ToString() == expr1->ToString() )
	{
		delete expr1;
		return new CExpressionProduct( new CExpressionConstant( 2.0 ), expr0 );
	}

	if ( expr0->ToString() == "0") { delete expr0; return expr0; }
	if ( expr1->ToString() == "0") { delete expr1; return expr1; }

	return new CExpressionSum( expr0, expr1 );

}