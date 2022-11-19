
#include "TriangularSubelement.h"
#include "FiniteElementInstance2D.h"

CTriangularSubelement::CTriangularSubelement( CFiniteElementInstance2D *ms, const vector< mvector > &n  ):nodes(n), master(ms)
{
}

CTriangularSubelement::~CTriangularSubelement(void)
{
}

void CTriangularSubelement::Subdivide( unsigned level )
{
	if (Area()<1e-10) return;
	if ( level == 0 )
	{
//		double a = Area();
		master->AddTriangle( *this );
		return;
	}

	mvector	e1=0.5*( nodes[0] + nodes[1] ),
			e2=0.5*( nodes[1] + nodes[2] ),
		    e3=0.5*( nodes[2] + nodes[0] );

	vector< mvector > nsub(3);

	nsub[0] = nodes[ 0 ];
	nsub[1] = e1;
	nsub[2] = e3;
	CTriangularSubelement st0( master, nsub );

	nsub[0] = nodes[ 1 ];
	nsub[1] = e2;
	nsub[2] = e1;
	CTriangularSubelement st1( master, nsub );

	nsub[0] = nodes[ 2 ];
	nsub[1] = e3;
	nsub[2] = e2;
	CTriangularSubelement st2( master, nsub );

	nsub[0] = e1;
	nsub[1] = e2;
	nsub[2] = e3;
	CTriangularSubelement st3( master, nsub );

	st0.Subdivide( level - 1 );
	st1.Subdivide( level - 1 );
	st2.Subdivide( level - 1 );
	st3.Subdivide( level - 1 );

}

void CTriangularSubelement::GetJacobyMatrix( const mvector &xi, matrix &J  ) const
{
	J.SetDim( 2, 2 );
    matrix dN( sf.GetXdim(), 2 );
    sf.jacobian( xi, dN );
	unsigned k;
    for (k=0; k<sf.GetXdim(); k++)
	{
				mvector x( 2 );
//				master->GetX( nodes[ k ], x );
				x = nodes[ k ];

				J(0,0) += dN(k,0) * x[ 0 ];   J(0,1) += dN(k,0) * x[ 1 ];
				J(1,0) += dN(k,1) * x[ 0 ];   J(1,1) += dN(k,1) * x[ 1 ];
    }
}

void CTriangularSubelement::GetMasterCoords( const mvector &xi, mvector &mX  ) const
{
	unsigned k;
	mvector N(3);
	mX.SetDim(2);
    sf.transform( xi, N );
    for (k=0; k<sf.GetXdim(); k++)
		mX += N[ k ] * nodes[ k ];
}

double CTriangularSubelement::Area() const
{

	mvector x0(2), x1(2), x2(2);

    master->GetShapeSpace()->Transform( nodes[ 0 ], x0 );
    master->GetShapeSpace()->Transform( nodes[ 1 ], x1 );
    master->GetShapeSpace()->Transform( nodes[ 2 ], x2 );

	return 0.5 * fabs( (x2[0]-x0[0]) * ( x1[1]-x0[1] ) - (x1[0]-x0[0]) * ( x2[1]-x0[1] ) );
}

void CTriangularSubelement::GetVertex( vector<mvector> &coords )const
{
    master->GetShapeSpace()->Transform( nodes[ 0 ], coords[ 0 ] );
    master->GetShapeSpace()->Transform( nodes[ 1 ], coords[ 1 ] );
    master->GetShapeSpace()->Transform( nodes[ 2 ], coords[ 2 ] );

}

void CTriangularSubelement::GetKsi( const mvector &xi, mvector &Xi ) const
{
	mvector N;
    sf.transform( xi, N );
	Xi = N[0] * nodes[0] + N[1] * nodes[1] + N[2] * nodes[2];

}
