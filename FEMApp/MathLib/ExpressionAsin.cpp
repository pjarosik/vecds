#include "ExpressionAsin.h"
#include "ExpressionProduct.h"
#include "ExpressionQuotient.h"
#include "ExpressionConstant.h"
#include "ExpressionSquareroot.h"
#include "ExpressionSubtraction.h"
#include "ExpressionPower.h"


CExpressionAsin::CExpressionAsin(CExpression *ex)
{
	expr.push_back( ex );
}

CExpressionAsin::~CExpressionAsin( void )
{
}

CExpression* CExpressionAsin::Differentiate( const string &wrt ) const 
{ 
	if ( !expr[0]->DependsOn( wrt ) ) return new CExpressionConstant("0");
	CExpression *dexpr = expr[0]->Differentiate( wrt );
	return new CExpressionProduct( dexpr, 
											new CExpressionQuotient( 
																		new CExpressionConstant("1"),
																		new CExpressionSquareroot( new CExpressionSubtraction( 
																																new CExpressionConstant("1"),
																																new CExpressionPower( 
																																					expr[ 0 ]->Clone(),
																																					new CExpressionConstant("2")
																																		)
																															)
																								 )
																		) 
								); 
}
