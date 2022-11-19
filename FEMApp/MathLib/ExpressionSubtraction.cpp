#include "ExpressionSubtraction.h"
#include "ExpressionNegative.h"
#include "ExpressionConstant.h"

CExpressionSubtraction::CExpressionSubtraction(const string &ex1, const string &ex2)
{
	Parse( ex1 );
	Parse( ex2 );
}

CExpressionSubtraction::CExpressionSubtraction( const vector<string> &exs )
{
	unsigned k;
	for (k=0; k<exs.size(); k++)
		Parse( exs[ k ] );
}

CExpressionSubtraction::~CExpressionSubtraction(void)
{

}

double CExpressionSubtraction::Compute() const 
{ 
	double res = expr[ 0 ]->Compute();
	unsigned k;
	for (k=1; k<expr.size(); k++)
		res -= expr[k]->Compute();
	return  res; 
}

string CExpressionSubtraction::ToString() const 
{ 
	string res = expr[0]->ToString();
	unsigned k;
	for (k=1; k<expr.size(); k++)
		res += "-" +  expr[k]->ToString(); 
	return res;
}

CExpression* CExpressionSubtraction::Differentiate( const string &wrt ) const 
{ 
	vector< CExpression* > res;
	unsigned k;
	for (k=0; k<expr.size(); k++)
		res.push_back( expr[k]->Differentiate( wrt ) );
	return new CExpressionSubtraction( res ); 
}

CExpression* CExpressionSubtraction::Simplify( ) const
{
	
	CExpression *expr0 = expr[0]->Simplify();
	CExpression *expr1 = expr[1]->Simplify();

	if ( expr0->ToString() == "0") { delete expr0; return new CExpressionNegative( expr1 ); }
	if ( expr1->ToString() == "0") { delete expr1; return expr0; }

	if ( expr0->ToString() == expr1->ToString() ) { delete expr0; delete expr1; return new CExpressionConstant(0.0); }

	return new CExpressionSubtraction( expr0, expr1 );

}