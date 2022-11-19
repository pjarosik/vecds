#include "ExpressionLn.h"
#include "ExpressionQuotient.h"
#include "ExpressionConstant.h"

CExpressionLn::CExpressionLn(CExpression *ex)
{
	expr.push_back( ex );
}

CExpressionLn::~CExpressionLn(void)
{
}

CExpression* CExpressionLn::Differentiate( const string &wrt ) const
{
	if ( expr[0]->DependsOn( wrt ) ) 
		return new CExpressionQuotient( expr[0]->Differentiate(wrt) , expr[0]->Clone() ) ;
	return new CExpressionConstant("0");
}
