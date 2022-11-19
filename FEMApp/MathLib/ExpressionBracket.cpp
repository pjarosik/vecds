#include "ExpressionBracket.h"
#include "ExpressionConstant.h"

CExpressionBracket::CExpressionBracket( const string &exp ):CExpression( exp )
{

}


CExpressionBracket::~CExpressionBracket( void )
{
}

CExpression* CExpressionBracket::Differentiate( const string &wrt ) const
{
	return new CExpressionBracket( expr[0]->Differentiate(wrt) );
}

