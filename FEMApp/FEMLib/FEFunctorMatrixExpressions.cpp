#include "FEFunctorMatrixExpressions.h"


CFEFunctorMatrixExpressions::CFEFunctorMatrixExpressions( const vector< vector< string > > &se, const string &nm ):CFEFunctorMatrix( nm ),
    exprs( se.size(), vector< CExpression* >( se[0].size(), 0 ) )
{
    unsigned k,l;
    for (k=0; k<se.size(); k++)
        for (l=0; l<se[k].size(); l++)
            exprs[ k ][ l ] = new CExpression( se[ k ][ l ] );
}

CFEFunctorMatrixExpressions::CFEFunctorMatrixExpressions( const CFEFunctorMatrixExpressions &vf ):CFEFunctorMatrix( vf ),exprs(vf.exprs)
{
    unsigned k,l;
    for (k=0; k<exprs.size(); k++)
        for (l=0; l<exprs[l].size(); l++)
            exprs[ k ][ l ] = exprs[ k ][ l ]->Clone();
}

CFEFunctorMatrixExpressions::~CFEFunctorMatrixExpressions( void )
{
    unsigned k,l;
    for (k=0; k<exprs.size(); k++)
            for (l=0; l<exprs[l].size(); l++) delete exprs[ k ][ l ];
}

void CFEFunctorMatrixExpressions::Compute( CFiniteElementInstance *fe1 )
{
    fv.SetDim(exprs.size(),exprs[0].size());
    unsigned k,l;
    for (k=0; k<exprs.size(); k++)
        for (l=0; l<exprs[l].size(); l++)
            fv(k,l) = exprs[ k ][ l ]->Compute();
}
