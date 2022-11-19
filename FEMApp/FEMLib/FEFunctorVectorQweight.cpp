
#include "FEFunctorVectorQweight.h"

CFEFunctorVectorQweight::CFEFunctorVectorQweight( CFEFunctorMatrix *dn, const mvector &x, double d ):CFEFunctorVector("CFEFunctorVectorQweight"),dN(dn),dist(d),xs(x)
{
	AddNode( dN );
}

CFEFunctorVectorQweight::CFEFunctorVectorQweight( const CFEFunctorVectorQweight &vf ):CFEFunctorVector( vf ),dist(vf.dist),xs(vf.xs)
{
	CLONE_MEMBER( vf, dN )
}

CFEFunctorVectorQweight::~CFEFunctorVectorQweight( void )
{

}

bool CFEFunctorVectorQweight::AcceptFE(CFiniteElementInstance *fe1 )
{
	unsigned k;
	bool inside=false, outside=false;
	for (k=0; k<fe1->GetNodes().size(); k++)
	{
		if ( IsInside( fe1->GetNodes()[k] ) ) inside = true;
		else outside = true;
		if ( inside && outside ) return true;
	}
	
	return false;

}

void CFEFunctorVectorQweight::Compute( CFiniteElementInstance *fe1 )
{
	unsigned k;
	fv.SetDim(2);
	mvector &dq=fv;
	const matrix &dN1 = (*dN);
	for (k=0; k<fe1->GetNodes().size(); k++)
	{
		if ( IsInside( fe1->GetNodes()[k] ) ) 
		{
			dq[ 0 ] += dN1(k,0);
			dq[ 1 ] += dN1(k,1);
		}
	}


}