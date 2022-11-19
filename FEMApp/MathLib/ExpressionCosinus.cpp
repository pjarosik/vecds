#include "ExpressionCosinus.h"
#include "ExpressionVariable.h"
#include "ExpressionConstant.h"
#include "ExpressionNegative.h"
#include "ExpressionSinus.h"
#include "ExpressionProduct.h"

CExpressionCosinus::CExpressionCosinus( CExpression *ex )
{
	expr.push_back( ex );
}

CExpressionCosinus::~CExpressionCosinus( void )
{

}

CExpression* CExpressionCosinus::Differentiate( const string &wrt ) const 
{ 
	if ( !expr[0]->DependsOn( wrt ) ) return new CExpressionConstant("0");
	if ( expr[0]->ToString() == wrt ) return new CExpressionNegative( new CExpressionSinus( new CExpressionVariable( wrt ) ) ); 
	CExpression *dexpr = expr[0]->Differentiate( wrt );
	return new CExpressionNegative( new CExpressionProduct( dexpr, new CExpressionSinus( expr[0]->Clone() ) ) ); 
}