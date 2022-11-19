#include "ExpressionTanh.h"
#include "ExpressionSinh.h"
#include "ExpressionVariable.h"
#include "ExpressionProduct.h"
#include "ExpressionPower.h"
#include "ExpressionSubtraction.h"


CExpressionTanh::CExpressionTanh( CExpression *ex )
{
	expr.push_back( ex );
}

CExpressionTanh::~CExpressionTanh(void)
{

}

CExpression* CExpressionTanh::Differentiate( const string &wrt ) const 
{ 
	if ( !expr[ 0 ]->DependsOn( wrt ) ) return new CExpressionConstant("0");
	
	CExpression *dexpr = expr[ 0 ]->Differentiate( wrt );
	return new CExpressionProduct( new CExpressionSubtraction(
																new CExpressionConstant("1"),
																new CExpressionPower( 
																						new CExpressionTanh( expr[0]->Clone() ), 
																						new CExpressionConstant("2") 
																					) 
															),
															dexpr
								);
}