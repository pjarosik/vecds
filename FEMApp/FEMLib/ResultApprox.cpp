#include "ResultApprox.h"
#include "FEException.h"
#include <float.h>


CResultApprox::CResultApprox( const CSpaceShapeApproxPtr< CFENode > &s, const string &rn ):CSpaceShapeApproxPtr< CFENode >(s.GetShapeSpace(),s.GetPoints(),1),
    rname( rn ), ra( 0 ), sa( s )
{
    unsigned k,l;
    for ( k = 0; k < values.size(); k++ )
    {
        double resv = values[ k ]->GetResult( rname );
        if ( resv != DBL_MAX ) nv.push_back( resv );

        CDOFVector &dofs = values[ k ]->GetDofs( );
        CDOFVector::const_iterator i;
        for (i= dofs.begin(); i!=dofs.end(); i++)
            if ( (*i)->DofName() == rn ) nv.push_back( (*i)->GetSolution() );

    }

    if ( nv.size() == values.size() )
        ra = new CSpaceShapeApprox< vector< double > >( GetShapeSpace(), nv );
}


void CResultApprox::transform( const mvector &ksi, mvector &X ) const
{
    ra->Transform( ksi, X );
}

void CResultApprox::jacobian( const mvector &ksi, matrix &J ) const
{
    ra->Jacobian( ksi, J );
}
