#include "ExpressionSquareroot.h"
#include "ExpressionQuotient.h"
#include "ExpressionConstant.h"
#include "ExpressionProduct.h"
#include "ExpressionSum.h"
#include "ExpressionSubtraction.h"
#include "ExpressionBracket.h"


CExpressionSquareroot::CExpressionSquareroot(CExpression *ex)
{
	expr.push_back( ex );
}

CExpressionSquareroot::~CExpressionSquareroot(void)
{
}

CExpression* CExpressionSquareroot::Differentiate( const string &wrt ) const
{
		return new CExpressionQuotient( 
										expr[0]->Differentiate( wrt ),
										new CExpressionBracket ( new CExpressionProduct(
																						new CExpressionConstant( "2" ), 
																						new CExpressionSquareroot( expr[0]->Clone( ) )
																						)
																)
										);
}