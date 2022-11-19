#include "ExpressionNegative.h"
#include "ExpressionConstant.h"

CExpressionNegative::CExpressionNegative(const string &ex):CExpression( ex )
{
}


CExpressionNegative::~CExpressionNegative(void)
{
}


CExpression* CExpressionNegative::Differentiate( const string &wrt ) const
{
	if ( expr[0]->DependsOn( wrt ) ) return new CExpressionNegative( expr[0]->Differentiate(wrt) );
	return new CExpressionConstant("0");
}