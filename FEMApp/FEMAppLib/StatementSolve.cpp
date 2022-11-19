#include "StatementSolve.h"

CStatementSolve::CStatementSolve(CFEMProject &f ):FP( f )
{
    int a=1;
}

CStatementSolve::~CStatementSolve(void)
{
}

void CStatementSolve::Execute( istream &i )
{
    FP.Solve();

}
