#ifndef FEFUNCTORMATRIXEXPRESSIONS_H
#define FEFUNCTORMATRIXEXPRESSIONS_H

#include "FEFunctorMatrix.h"
#include "../MathLib/Expression.h"

class CFEFunctorMatrixExpressions : public CFEFunctorMatrix
{
    CFEFunctorMatrixExpressions( const vector< vector< string > > &stre, const string &nm = "FEFunctorMatrixExpressions" );
    virtual ~CFEFunctorMatrixExpressions( void );
    CFEFunctorMatrixExpressions( const CFEFunctorMatrixExpressions &vf );
    virtual void ManageDOFs( CFiniteElementInstance *fe1 ) {  }
    CLONE_FN( CFEFunctorMatrixExpressions )

protected:

    virtual void Compute( CFiniteElementInstance *fe1 );
    vector< vector< CExpression* > > exprs;

};


#endif // FEFUNCTORMATRIXEXPRESSIONS_H
