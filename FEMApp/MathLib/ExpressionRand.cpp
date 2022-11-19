#include "ExpressionRand.h"


CExpressionRand::CExpressionRand(CExpression *ex)
{
	expr.push_back( ex );
}

CExpressionRand::~CExpressionRand(void)
{
}

CExpression* CExpressionRand::Differentiate( const string &wrt ) const 
{ 
	return 0;
}

double CExpressionRand::Compute() const
{

    /*
	double r = (double)VRND::rnd( ) + ( (double)RAND_MAX - 1.0 ) * (double)VRND::rnd( ) ;
	if ( r == 0.0 ) 
		r += 1.0;
	if ( r == (double)RAND_MAX * (double)RAND_MAX ) 
		r -= 1.0 ;
	
    return expr[0]->Compute() * ( (double)RAND_MAX * (double)RAND_MAX - r ) / (double)RAND_MAX / (double)RAND_MAX;
*/
}
