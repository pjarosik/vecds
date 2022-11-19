#include "StatementVariables.h"
#include "../LangLib/statdef.h"
#include "../MathLib/Expression.h"

CStatementVariables::CStatementVariables( CDBNode &s ):System( s )
{

}


void CStatementVariables::Execute( istream &i )
{
    NamedParameterListTokenExpressions variables;

    i >> variables;

    unsigned k;
    for (k=0; k<variables.GetList().size(); k++)
    {
        CExpression expr( variables.GetList()[ k ].GetParam().toString(), System.GetVariables() );
        string name = variables.GetList()[ k ].GetName().toString();
        VariableDouble var( expr.Compute() );
        System.Add( name, var );
    }
}
