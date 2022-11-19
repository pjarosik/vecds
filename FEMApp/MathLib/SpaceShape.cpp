#include "SpaceShape.h"
#include "MathException.h"
#include "SubspaceIndexer.h"

vector< unsigned >& operator << ( vector< unsigned > &ov, unsigned i) { ov.push_back( i ); return ov; }

CSpaceShape::CSpaceShape( SHAPE_TYPES t, unsigned a, unsigned b, unsigned ne, unsigned nf ):
    CSpace( a, upow( b, a ) ),
    dim(a),
    dg(b),
    shape_type( t ),
    edgeinds( ne ),
    faceinds( nf )
{

}

CSpaceShape::CSpaceShape( SHAPE_TYPES t, unsigned a, unsigned b ):CSpace(a,b),dim(a),shape_type( t )
{

}

CSpaceShape::CSpaceShape(const CSpaceShape &s ):CSpace( s ),nodes(s.nodes),edgeshapes(s.edgeshapes), faceshapes(s.faceshapes),
    edgeinds( s.edgeinds ), faceinds(s.faceinds),dim(s.dim),dg(s.dg),shape_type(s.shape_type)
{
    unsigned k;
    for (k=0; k<edgeshapes.size(); k++)
        edgeshapes[k] = edgeshapes[k]->Clone();

    for (k=0; k<faceshapes.size(); k++)
        faceshapes[k] = faceshapes[k]->Clone();
}


/* CSpaceShape::CSpaceShape( CCopyer &cpr, const CSpaceShape &s ):CSpace( s ),nodes(s.nodes),edgeshapes(s.edgeshapes), faceshapes(s.faceshapes),
    edgeinds( s.edgeinds ), faceinds(s.faceinds),dim(s.dim),dg(s.dg),shape_type(s.shape_type)
{
    unsigned k;

    cpr.Copy( edgeshapes );
    cpr.Copy( faceshapes );

}*/


CSpaceShape::~CSpaceShape()
{
 // vector< CSpaceShape* > edgeshapes, faceshapes;
    vector< CSpaceShape* >::iterator i;
    for (i=edgeshapes.begin(); i!=edgeshapes.end(); i++ )
        delete *i;

    for (i=faceshapes.begin(); i!=faceshapes.end(); i++ )
        delete *i;
}

void CSpaceShape::SelfTest( ostream &out ) const
{
    out << "*** SHAPE FUNCTIONS TEST n= " << nodes.size() << " ***" << endl << endl;
    out << "*** NODAL VALUE FUNCTIONS ***" << endl;
    mvector N( nodes.size() );
    for ( unsigned k = 0; k < nodes.size( ); k++)
    {
        transform( nodes[ k ], N );

        for ( unsigned l = 0; l < nodes.size( ); l++ )
        {
            double val = fabs( N[ l ] ) < 1e-10 ? 0.0 : fabs( N[ l ] );
            out << val;
        }
        out << endl;
    }
}

void CSpaceShape::CreateInds(  )
{
    switch( shape_type )
    {
        case ST_RECTANGLE :     CreateInds( edgeshapes, edgeinds );
                                CreateInds( faceshapes, faceinds );
                        break;
        case ST_TRIANGLE :
                                edgeinds.resize(3);
                                edgeinds[0].push_back(0);
                                edgeinds[0].push_back(1);

                                edgeinds[1].push_back(1);
                                edgeinds[1].push_back(2);

                                edgeinds[2].push_back(2);
                                edgeinds[2].push_back(0);
                        break;

    }
}


int CSpaceShape::FindIndex( const mvector &ksi ) const
{
    if ( ksi.GetDim() != dim ) throw CMathException("CSpaceShape::FindIndexe( const mvector &ksi ) - different dimension");
    unsigned k, l;
    for ( k=0; k<nodes.size(); k++ )
    {
        bool found = true;
        for ( l = 0; l < dim; l++ )
            if ( fabs( nodes[ k ][ l ] - ksi[ l ] ) > 1.0e-6 ) { found = false; break; }
        if ( found ) return k;
    }
    return -1;
}

void CSpaceShape::GetCornerNodes( vector< unsigned > &cn ) const
{
    unsigned k;
    vector<double> cc(2);
    cc[0] = -1.0;
    cc[1] =  1.0;
    mvector ksi(dim);
    CNCounter nc(dim,2);
    nc.Reset();
    do
    {
        for (k=0; k<dim; k++)
            ksi[k] = cc[ nc.GetIndex( k ) ];
        int cind = FindIndex( ksi );
        if ( cind != -1 ) cn.push_back( cind );
    } while( nc++ );
}


void CSpaceShape::CreateInds( const vector< CSpaceShape* > &ssc, vector< vector< unsigned > > &ic )
{
    if ( ssc.size() == 0 ) return;
    //if ( ksi.GetDim() != dim ) throw CMathException("CSpaceShape::FindIndexe( const mvector &ksi ) - different dimension");
    ic.resize( ssc.size() );
    vector<double> cv(2);
    cv[0] = -1.0;
    cv[1] =  1.0;
    unsigned k;
    CSubspaceIndexer isub( dim, ssc[ 0 ]->Getxdim(), cv );
    do {
            CSpaceShape &ss = *ssc[ isub ];
            const vector< mvector > &ln = ss.GetLocalNodes();
            mvector X;
            for ( k=0; k<ln.size(); k++)
            {
                isub.Map( ln[ k ], X );
                int ind = FindIndex( X );
                if ( ind != -1 ) ic[ isub ].push_back( (unsigned)ind );
                else throw CMathException("CSpaceShape::CreateInds(  ) -index not found");
            }
    } while( isub.Next() );
}


void CSpaceShape::ExtractIndices( const CSpaceShape &os, vector< vector< unsigned > > &inds )
{
    /*
    unsigned sdim = os.Getxdim(), idim = dim - sdim;
    inds.clear();
    unsigned k, l, ind = 0;
    vector< bool > ia( dim );
    mvector xi( dim );
    for (k=0; k<idim; k++) ia[ ind++ ] = false;
    for (k=0; k<sdim; k++) ia[ ind++ ] = true;
    do
    {
        for (k=0; k<os.GetLocalNodes().size(); k++ )
        {
            ind = 0;
            for (l = 0; l<dim; l++)
                if ( ia[ l ] ) xi[ l ] = os[ k ][ ind++ ];
            int ind = FindIndex( xi );
            if ( ind != -1 ) inds[ i ].push_back( (unsigned)ind );
            else throw CMathException("CSpaceShape::FindIndex(  ) -index not found");
        }
        done = true;

    } while( done );
*/
}
