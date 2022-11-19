
#define _USE_MATH_DEFINES
#include <cmath>

#include "Crack.h"
#include "FiniteElementInstance2D.h"
#include <algorithm>
#include "TriangularSubelement.h"

CCrack::CCrack(void):CCurve( -1.0, 1.0 ),tip1(0),tip2(0),ndiv(100)
{

}

CCrack::~CCrack(void)
{
}


void CCrack::FindCrackLocation( const vector< CFiniteElementInstance* > &elems,  const vector< CFENode* > &nodes )
{
	unsigned k, l;
	mvector X(2), X1(2), X2(2);
	double dt = ( t2 - t1 ) / (double)ndiv;

	XYZ( t1, X1 );
	XYZ( t2, X2 );

//	double a1 = angle(t1) + M_PI, a2 = angle( t2 );


	for (l=0; l<elems.size(); l++)
	{
		if ( elems[ l ]->IsInBoundingBox( X1 ) )
		{
			if ( find(Te.begin(),Te.end(), elems[l] ) == Te.end() )
			{
				Te.push_back( elems[l] ) ;
				Te1.push_back( elems[l] ) ;
			}
		}
		if ( elems[ l ]->IsInBoundingBox( X2 ) )
			{
				if ( find(Te.begin(),Te.end(), elems[l] ) == Te.end() )
				{
					Te.push_back( elems[l] ) ;
					Te2.push_back( elems[l] ) ;
				}
			}

	}

	for (k=0; k<=ndiv; k++)
	{
		XYZ( t1 + k * dt, X );
		for (l=0; l<elems.size(); l++)
			if ( elems[ l ]->IsInBoundingBox( X ) )
			{
				if ( ( find(Te.begin(),Te.end(), elems[l] ) == Te.end() ) && ( find(He.begin(),He.end(), elems[l] ) == He.end() ) )
//				if (  ( find(He.begin(),He.end(), elems[l] ) == He.end() ) )
				{
			 		He.push_back( elems[l] ) ;
				}
			}
	}

	Tn = Te[0]->GetNodes();

	for (k=0; k<Te.size(); k++)
		for (l=0; l<Te[k]->GetNodes().size(); l++)
			if ( find( Tn.begin(), Tn.end(), Te[k]->GetNodes()[l] ) == Tn.end() ) Tn.push_back( Te[k]->GetNodes()[l] );

	for (k=0; k<nodes.size(); k++)
	{
        nodes[ k ]->GetX(X);
        if ( IsNear( X ) && find( Tn.begin(), Tn.end(), nodes[ k ] ) == Tn.end() )  Hn.push_back( nodes[k] );
	}

	EnrichNodes();

	for (k=0; k<elems.size(); k++)
		if ( (find(Te.begin(),Te.end(),elems[k])==Te.end()) && (find(He.begin(),He.end(),elems[k])==He.end())) Re.push_back( elems[k] );


}

double CCrack::H( const mvector &x ) const
{
	if ( distance(x) < 0.0 ) return -1.0;
	if ( distance(x) > 0.0 ) return  1.0;
	return 0.0;
}

void CCrack::EnrichNodes()
{

}

void CCrack::Subtriangularize()
{
	unsigned k, l, level=1;
//	CDOF *dof = 0;
	mvector x1(2),x2(2),x3(2),x4(2),xs(2);
	double area;


	xs[0]= 0; xs[1]= 0;
	x1[0]=-1; x1[1]=-1;
	x2[0]= 1; x2[1]=-1;
	x3[0]= 1; x3[1]= 1;
	x4[0]=-1; x4[1]= 1;

	for (k=0; k<He.size(); k++)
	{
		vector<mvector> tn;
		tn.push_back( x1 );
		tn.push_back( x2 );
		tn.push_back( xs );
		CTriangularSubelement te1( (CFiniteElementInstance2D*)He[k],tn );

		tn[0] = x2;
		tn[1] = x3;
		tn[2] = xs;
		CTriangularSubelement te2( (CFiniteElementInstance2D*)He[k],tn );

		tn[0] = x3;
		tn[1] = x4;
		tn[2] = xs;
		CTriangularSubelement te3( (CFiniteElementInstance2D*)He[k],tn );

		tn[0] = x4;
		tn[1] = x1;
		tn[2] = xs;
		CTriangularSubelement te4( (CFiniteElementInstance2D*)He[k],tn );

		te1.Subdivide(level);
		te2.Subdivide(level);
		te3.Subdivide(level);
		te4.Subdivide(level);

		CFiniteElementInstance2D* elem = (CFiniteElementInstance2D*)He[k];
		area=0;
		for(l=0; l<elem->GetSubElems().size(); l++)
			area += elem->GetSubElems()[l].Area();

	}

	mvector X(2),X1(2),X2(2);
//	double dt = ( t2 - t1 ) / (double)ndiv;

	XYZ( t1, X1 );
	XYZ( t2, X2 );

	for (k=0; k<Te1.size(); k++)
	{
		Te1[ k ]->GetXi( X1, xs );
		vector<mvector> tn;
		tn.push_back( x1 );
		tn.push_back( x2 );
		tn.push_back( xs );
		CTriangularSubelement te1( (CFiniteElementInstance2D*)Te1[k],tn );

		tn[0] = x2;
		tn[1] = x3;
		tn[2] = xs;
		CTriangularSubelement te2( (CFiniteElementInstance2D*)Te1[k],tn );

		tn[0] = x3;
		tn[1] = x4;
		tn[2] = xs;
		CTriangularSubelement te3( (CFiniteElementInstance2D*)Te1[k],tn );

		tn[0] = x4;
		tn[1] = x1;
		tn[2] = xs;
		CTriangularSubelement te4( (CFiniteElementInstance2D*)Te1[k],tn );

		te1.Subdivide(level);
		te2.Subdivide(level);
		te3.Subdivide(level);
		te4.Subdivide(level);

	}

	for (k=0; k<Te2.size(); k++)
	{
		Te2[ k ]->GetXi( X2, xs );
		vector<mvector> tn;
		tn.push_back( x1 );
		tn.push_back( x2 );
		tn.push_back( xs );
		CTriangularSubelement te1( (CFiniteElementInstance2D*)Te2[k],tn );

		tn[0] = x2;
		tn[1] = x3;
		tn[2] = xs;
		CTriangularSubelement te2( (CFiniteElementInstance2D*)Te2[k],tn );

		tn[0] = x3;
		tn[1] = x4;
		tn[2] = xs;
		CTriangularSubelement te3( (CFiniteElementInstance2D*)Te2[k],tn );

		tn[0] = x4;
		tn[1] = x1;
		tn[2] = xs;
		CTriangularSubelement te4( (CFiniteElementInstance2D*)Te2[k],tn );

		te1.Subdivide(level);
		te2.Subdivide(level);
		te3.Subdivide(level);
		te4.Subdivide(level);

	}
}
