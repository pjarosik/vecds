#include "ExpressionAbs.h"


CExpressionAbs::CExpressionAbs(CExpression *ex)
{
	expr.push_back( ex );
}


CExpressionAbs::~CExpressionAbs(void)
{
}
