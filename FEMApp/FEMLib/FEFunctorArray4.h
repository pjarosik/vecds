#ifndef FEFUNCTORARRAY4_H
#define FEFUNCTORARRAY4_H

#include "FEFunctor.h"
#include <vector>
using namespace std;

class CFEFunctorArray4 : public CFEFunctor< CFiniteElementInstance >
{

public:

    CFEFunctorArray4( unsigned d1, unsigned d2, unsigned d3, unsigned d4, const string &nm="" );
    CFEFunctorArray4( const CFEFunctorArray4 &mf );
    CLONE_FN( CFEFunctorArray4 )
    using CFEFunctor< CFiniteElementInstance >::operator();
    const double& operator() (unsigned i, unsigned j, unsigned k, unsigned l ) const { return data[ i ][ j ][ k ][ l ]; }
    double& operator() (unsigned i, unsigned j, unsigned k, unsigned l ) { return data[ i ][ j ][ k ][ l ]; }

    virtual void ManageDOFs( CFiniteElementInstance *p  ) { }
    ostream& PrintFunctor( ostream &out ) const;
    virtual void DebugPrint( ) const;

protected:
    vector< vector< vector< vector< double > > > > data;

    virtual void Compute( CFiniteElementInstance *fe1 ) { }
};

#endif // FEFUNCTORARRAY4_H
