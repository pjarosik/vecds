#include "ExpressionCosh.h"
#include "ExpressionSinh.h"
#include "ExpressionVariable.h"
#include "ExpressionProduct.h"

CExpressionCosh::CExpressionCosh(CExpression *ex)
{
	expr.push_back( ex );
}

CExpressionCosh::~CExpressionCosh(void)
{
}

CExpression* CExpressionCosh::Differentiate( const string &wrt ) const 
{ 
	if ( !expr[0]->DependsOn( wrt ) ) return new CExpressionConstant("0");
	if ( expr[0]->ToString() == wrt ) return new CExpressionSinh( new CExpressionVariable( wrt ) ); 
	CExpression *dexpr = expr[0]->Differentiate( wrt );
	return new CExpressionProduct( dexpr, new CExpressionSinh( expr[0]->Clone() ) ); 
}