#include "FiniteElementInstance3D.h"
#include "../MathLib/SpaceShapeLagrange2N_3D_8.h"
#include "../MathLib/SpaceShapeApprox.h"

CFiniteElementInstance3D::CFiniteElementInstance3D( CGeometricDomain &gd, CSpaceShapeApproxPtr< CFENode > &s ):CFiniteElementInstance(gd,s)
{
}

CFiniteElementInstance3D::~CFiniteElementInstance3D(void)
{
}

double CFiniteElementInstance3D::GetVolume( ) const
{
	return 0.0;
}

void CFiniteElementInstance3D::ApproximateAllNodalResults( )
{
    vector<string> allnames;
    const vector< CFENode* > &nodes =  main_shape->GetPoints();
    unsigned k;
    for (k=0; k<nodes.size(); k++ ) {
            const map< string, double> &nres = nodes[k]->GetResults();
            map< string, double>::const_iterator i;
            for ( i=nres.begin(); i!=nres.end(); i++ )
                if ( find( allnames.begin(), allnames.end(), i->first ) == allnames.end() ) allnames.push_back( i->first );
    }

    const vector< mvector >& lnodes = main_shape->GetShapeSpace().GetLocalNodes();
    vector< unsigned > cn;

    main_shape->GetShapeSpace().GetCornerNodes( cn );
    CSpaceShapeLagrange2N_3D_8 L8;
    vector< double > values(8);
    CSpaceShapeApprox< vector< double > > L8approx( L8, values );
    mvector V(1);

    for (k=0; k<nodes.size(); k++ )
    {
        const map< string, double> &nres = nodes[k]->GetResults();
        unsigned l;
        for (l=0; l<allnames.size(); l++)
            if ( nres.find( allnames[l] ) == nres.end() )
            {
                unsigned m;
                for (m=0; m<8; m++)
                {
                    values[ m ] = nodes[ cn[ m ] ]->GetResult( allnames[ l ] );
                }
                L8approx.Transform( lnodes[ k ], V );
                nodes[k]->AccumulateResult( allnames[l], V[0] );
            }
    }
}
