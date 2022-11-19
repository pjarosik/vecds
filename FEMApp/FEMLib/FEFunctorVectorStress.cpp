
#include "FEFunctorVectorStress.h"


CFEFunctorVectorStress::CFEFunctorVectorStress( CFEFunctorMatrix *d, CFEFunctorVector *ee ):CFEFunctorVector( "stress" ), D( d ), e( ee )
{
	AddNode( D );
	AddNode( e );
	mnames = e->GetNames( );
	unsigned k;
	string ds;
	for (k=0; k<mnames.size(); k++)
	{
		if ( mnames[k][0] == 'e' ) mnames[k][0] = 's';
		if ( mnames[k][0] == 'g' ) mnames[k][0] = 't';
	}
}

CFEFunctorVectorStress::CFEFunctorVectorStress( CFEFunctorVectorStress &vf ):CFEFunctorVector( vf )
{
	CLONE_MEMBER( vf, D );
	CLONE_MEMBER( vf, e );
}

CFEFunctorVectorStress::~CFEFunctorVectorStress( void )
{

}

void CFEFunctorVectorStress::Compute(CFiniteElementInstance *fe1 )
{
	fv = (*D) * (*e) ;
}