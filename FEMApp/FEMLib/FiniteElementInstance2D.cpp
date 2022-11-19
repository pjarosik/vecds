
#include "FiniteElementInstance2D.h"

CFiniteElementInstance2D::CFiniteElementInstance2D( CGeometricDomain &gd, CSpaceShapeApproxPtr< CFENode > &s ):CFiniteElementInstance(gd,s)
{
}

CFiniteElementInstance2D::~CFiniteElementInstance2D( void )
{

}

	
double CFiniteElementInstance2D::GetArea( ) const 
{ 
	return 0.0;
}

double CFiniteElementInstance2D::GetVolume( ) const
{
    return 0.0;
}

void CFiniteElementInstance2D::SubDivide(unsigned level)
{
	mvector x1(2),x2(2),x3(2),x4(2),xs(2);

	xs[0]= 0; xs[1]= 0;
	x1[0]=-1; x1[1]=-1;
	x2[0]= 1; x2[1]=-1;
	x3[0]= 1; x3[1]= 1;
	x4[0]=-1; x4[1]= 1;
	
	vector<mvector> tn;
	tn.push_back( x1 ); 
	tn.push_back( x2 ); 
	tn.push_back( xs );
	CTriangularSubelement te1( this,tn );

	tn[0] = x2; 
	tn[1] = x3; 
	tn[2] = xs;
	CTriangularSubelement te2( this,tn );

	tn[0] = x3; 
	tn[1] = x4; 
	tn[2] = xs;
	CTriangularSubelement te3( this,tn );

	tn[0] = x4; 
	tn[1] = x1; 
	tn[2] = xs;
	CTriangularSubelement te4( this,tn );

	te1.Subdivide(level);
	te2.Subdivide(level);
	te3.Subdivide(level);
	te4.Subdivide(level);
}

void CFiniteElementInstance2D::ApproximateAllNodalResults( )
{

}

