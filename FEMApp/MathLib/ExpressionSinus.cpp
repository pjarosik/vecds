#include "ExpressionSinus.h"
#include "ExpressionConstant.h"
#include "ExpressionVariable.h"
#include "ExpressionCosinus.h"
#include "ExpressionProduct.h"

CExpressionSinus::CExpressionSinus(CExpression *ex)
{
	expr.push_back( ex );
}

CExpressionSinus::~CExpressionSinus(void)
{
}

CExpression* CExpressionSinus::Differentiate( const string &wrt ) const 
{ 
	if ( !expr[0]->DependsOn( wrt ) ) return new CExpressionConstant("0");
	if ( expr[0]->ToString() == wrt ) return new CExpressionCosinus( new CExpressionVariable( wrt ) ); 
	CExpression *dexpr = expr[0]->Differentiate( wrt );
	return new CExpressionProduct( dexpr, new CExpressionCosinus( expr[0]->Clone() ) ); 
}