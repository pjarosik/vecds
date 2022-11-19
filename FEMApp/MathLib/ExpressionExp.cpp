#include "ExpressionExp.h"
#include "ExpressionVariable.h"
#include "ExpressionProduct.h"

CExpressionExp::CExpressionExp(CExpression *ex)
{
	expr.push_back( ex );
}

CExpressionExp::~CExpressionExp(void)
{
}

CExpression* CExpressionExp::Differentiate( const string &wrt ) const 
{ 
	if ( !expr[ 0 ]->DependsOn( wrt ) ) return new CExpressionConstant("0");
	if (  expr[ 0 ]->ToString() == wrt ) return new CExpressionExp( new CExpressionVariable( wrt ) ); 
	CExpression *dexpr = expr[ 0 ]->Differentiate( wrt );
	return new CExpressionProduct( dexpr, new CExpressionExp( expr[ 0 ]->Clone() ) ); 
}