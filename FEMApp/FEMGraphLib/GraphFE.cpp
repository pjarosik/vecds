#include "GraphFE.h"
#include "../MathLib/GeometricObjectShape.h"
#include "../MathLib/SpaceShapeApproxPtr.h"

CGraphFE::CGraphFE( CFiniteElement &f ):fe(f)
{
    const vector< FEDomain* > &fedomains = fe.GetDomains();
    vector< FEDomain* >::const_iterator i;
    for ( i = fedomains.begin(); i != fedomains.end(); i++ )
    {
        FEDomain::iterator j;
        for ( j= (*i)->begin(); j!=(*i)->end(); j++ )
        {
            //CSpaceShapeApproxPtr< CGeometricObjectPoint >  se();
            //CGeometricObject* g = new CGeometricObjectShape( owner, points );
            //gfe.push_back( );
        }

    }
}

CGraphFE::~CGraphFE()
{
    vector< CGeometricObject* >::iterator i;
    for (i= owner.begin(); i!=owner.end(); i++)
        delete *i;
}
