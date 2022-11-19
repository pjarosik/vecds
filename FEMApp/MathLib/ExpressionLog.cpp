#include "ExpressionLog.h"
#include "ExpressionLn.h"
#include "ExpressionQuotient.h"
#include "ExpressionProduct.h"
#include "ExpressionBracket.h"
#include "ExpressionConstant.h"

CExpressionLog::CExpressionLog(CExpression *ex)
{
	expr.push_back( ex );
}


CExpressionLog::~CExpressionLog(void)
{
}

CExpression* CExpressionLog::Differentiate( const string &wrt ) const
{
	if ( expr[0]->DependsOn( wrt ) ) 
		return new CExpressionQuotient( expr[0]->Differentiate(wrt) , new CExpressionBracket( 
																			new CExpressionProduct( expr[0]->Clone(), new CExpressionLn(
																																		new CExpressionConstant(10) 
																																		) 
																								)	
																		)
										);
	return new CExpressionConstant("0");
}