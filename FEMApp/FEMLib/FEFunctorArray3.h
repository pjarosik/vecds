#ifndef FEFUNCTORARRAY3_H
#define FEFUNCTORARRAY3_H

#include "FEFunctor.h"
#include <vector>
using namespace std;

class CFEFunctorArray3 : public CFEFunctor< CFiniteElementInstance >
{

public:

    CFEFunctorArray3( unsigned d1, unsigned d2, unsigned d3, const string &nm="" );
    CFEFunctorArray3( const CFEFunctorArray3 &mf );
    CLONE_FN( CFEFunctorArray3 )
    using CFEFunctor< CFiniteElementInstance >::operator();
    const double& operator() (unsigned i, unsigned j, unsigned k ) const { return data[ i ][ j ][ k ]; }
    double& operator() (unsigned i, unsigned j, unsigned k ) { return data[ i ][ j ][ k ]; }

    virtual void ManageDOFs( CFiniteElementInstance *p  ) { }
    ostream& PrintFunctor( ostream &out ) const;
    virtual void DebugPrint( ) const;

protected:
    vector< vector< vector< double > > > data;

    virtual void Compute( CFiniteElementInstance *fe1 ) { }
};

#endif // FEFUNCTORARRAY3_H
