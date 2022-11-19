#include "ExpressionTangens.h"
#include "ExpressionConstant.h"
#include "ExpressionProduct.h"
#include "ExpressionBracket.h"
#include "ExpressionQuotient.h"
#include "ExpressionCosinus.h"

CExpressionTangens::CExpressionTangens(CExpression *ex)
{
	expr.push_back( ex );
}

CExpressionTangens::~CExpressionTangens(void)
{
}

CExpression* CExpressionTangens::Differentiate( const string &wrt ) const
{
	return new CExpressionQuotient(
									expr[0]->Differentiate(wrt),
									new CExpressionBracket( 
															new CExpressionProduct(
																					new CExpressionCosinus( expr[0]->Clone() ), 
																					new CExpressionCosinus( expr[0]->Clone() )
																				)
															)
								);
}