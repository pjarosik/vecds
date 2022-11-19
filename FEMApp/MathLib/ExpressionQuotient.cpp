#include "ExpressionQuotient.h"
#include "ExpressionConstant.h"
#include "ExpressionProduct.h"
#include "ExpressionBracket.h"
#include "ExpressionSum.h"
#include "ExpressionSubtraction.h"

CExpressionQuotient::CExpressionQuotient( const string &ex1, const string &ex2 )
{
	Parse(ex1);
	Parse(ex2);
}

CExpressionQuotient::CExpressionQuotient( const vector<string> &exs )
{
	unsigned k;
	for (k=0; k<exs.size(); k++)
		Parse( exs[ k ] );

}

CExpressionQuotient::~CExpressionQuotient(void)
{
}

double CExpressionQuotient::Compute() const
{
	double res = expr[ 0 ]->Compute();
	unsigned k;
	for (k=1; k<expr.size(); k++)
		res /= expr[k]->Compute();
	return  res; 
}

string CExpressionQuotient::ToString() const
{
	string res = expr[0]->ToString();
	unsigned k;
	for (k=1; k<expr.size(); k++)
		res += "/" +  expr[k]->ToString(); 
	return res;
}

CExpression* CExpressionQuotient::Differentiate( const string &wrt ) const
{
	return new CExpressionSubtraction( 
							new CExpressionQuotient( 
													expr[0]->Differentiate(wrt),
													expr[1]->Clone()
												 ),
												
							new CExpressionQuotient( 
													new CExpressionBracket( new CExpressionProduct(
																				expr[0]->Clone(), 
																				expr[1]->Differentiate(wrt)
																			)
																			),
													new CExpressionBracket( new CExpressionProduct( 
																			expr[1]->Clone(),
																			expr[1]->Clone()
																			)
																			)
													)
							);
																							
}

CExpression* CExpressionQuotient::Simplify( ) const
{
	
	CExpression *expr0 = expr[0]->Simplify();
	CExpression *expr1 = expr[1]->Simplify();

	if ( expr1->ToString() == "1") { delete expr1; return expr0; }
	if ( expr0->ToString() == "0") { delete expr0; delete expr1; return new CExpressionConstant(0.0); }

	if ( expr0->ToString() == expr1->ToString() ) { delete expr0; delete expr1; return new CExpressionConstant(1.0); }

	return new CExpressionQuotient( expr0, expr1 );

}