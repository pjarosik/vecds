#include "ExpressionPower.h"
#include "ExpressionProduct.h"
#include "ExpressionQuotient.h"
#include "ExpressionConstant.h"
#include "ExpressionSum.h"
#include "ExpressionLn.h"

CExpressionPower::CExpressionPower( const string &ex1, const string &ex2 )
{
	Parse( ex1 );
	Parse( ex2 );
}


CExpressionPower::~CExpressionPower(void)
{

}


CExpression* CExpressionPower::Differentiate( const string &wrt ) const
{
	return new CExpressionProduct(
									Clone() ,
									new CExpressionSum( 
														new CExpressionQuotient(
																					new CExpressionProduct( expr[0]->Differentiate( wrt ), expr[1]->Clone() ),
																					expr[0]->Clone()
																				),
														new CExpressionProduct( 
																					expr[1]->Differentiate( wrt ), 
																					new CExpressionLn( expr[0]->Clone() )
																					)
														)
								);

}