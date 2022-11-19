#include "FEFunctorScalarCompliance.h"
#include "../MathLib/matrix.h"

CFEFunctorScalarCompliance::CFEFunctorScalarCompliance( const matrix &ms, CFEFunctorMatrix *fs, CFEFunctorVector *d ):CFEFunctorScalar("Compliance"),k(ms), K(fs),q(d)
{
    if ( K ) AddNode( K );
    AddNode( q );
}

CFEFunctorScalarCompliance::CFEFunctorScalarCompliance( const CFEFunctorScalarCompliance &vf ):CFEFunctorScalar( vf ),K(vf.K),k(vf.k)
{
    if ( vf.K )
    {
        CLONE_MEMBER( vf, K )
    }
    CLONE_MEMBER( vf, q )
}

void CFEFunctorScalarCompliance::Compute( CFiniteElementInstance *param  )
{
    const mvector &qe = *q;
    if (K) value = ((~qe)*((*K)*qe))(0,0);
    else value = ((~qe)*(k*qe))(0,0);
}
