#include "ExpressionSinh.h"
#include "ExpressionVariable.h"
#include "ExpressionProduct.h"
#include "ExpressionCosh.h"

CExpressionSinh::CExpressionSinh(CExpression *ex)
{
	expr.push_back( ex );
}

CExpressionSinh::~CExpressionSinh(void)
{
}

CExpression* CExpressionSinh::Differentiate( const string &wrt ) const 
{ 
	if ( !expr[0]->DependsOn( wrt ) ) return new CExpressionConstant("0");
	if ( expr[0]->ToString() == wrt ) return new CExpressionCosh( new CExpressionVariable( wrt ) ); 
	CExpression *dexpr = expr[0]->Differentiate( wrt );
	return new CExpressionProduct( dexpr, new CExpressionCosh( expr[0]->Clone() ) ); 
}