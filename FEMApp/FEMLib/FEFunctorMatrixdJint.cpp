
#include "FEFunctorMatrixdJint.h"



CFEFunctorMatrixdJint::CFEFunctorMatrixdJint( CFEFunctorVectorStress *sa, CFEFunctorVectorStraindU *ea, CFEFunctorMatrix *dua, CFEFunctorVectorStress *se, CFEFunctorMatrix *du, CFEFunctorVector *dq  ):CFEFunctorMatrix("FEFunctorMatrixdJint"),
                                                    s( se ), saux( sa ), dUaux( dua ), dU( du ), eaux( ea ), dqn( dq ), delta(2), dq(2)
{
	AddNode( saux );
	AddNode( eaux );
	AddNode( dUaux );
	AddNode( s );
	AddNode( dU );
	AddNode( dqn );

}

CFEFunctorMatrixdJint::CFEFunctorMatrixdJint(const CFEFunctorMatrixdJint &mf):CFEFunctorMatrix(mf),delta(2),dq(2)
{
	CLONE_MEMBER( mf, saux )
	CLONE_MEMBER( mf, eaux )
	CLONE_MEMBER( mf, dUaux )
	CLONE_MEMBER( mf, s )
	CLONE_MEMBER( mf, dU )
	CLONE_MEMBER( mf, dqn )
}

CFEFunctorMatrixdJint::~CFEFunctorMatrixdJint(void)
{

}

void CFEFunctorMatrixdJint::Compute(CFiniteElementInstance *fe1)
{

/*
	(*dUaux)();
	(*eaux)();
	(*saux)();
	(*s)();
	(*dU)();
	(*dqn)();
*/

	matrix sp;			//, &sum=*this, &dUaux1=*dUaux, &dU1=*dU;
	fv.SetDim(1,1);

	sp = ~(*s) * (*eaux);
	delta[0] = delta[1] = sp(0,0);

	fv += ~delta * (*dqn);

	delta[0] = (*s)[0] * (*dUaux)(0,0) ;
	delta[1] = 0.5 * (*s)[2] * (*dUaux)(1,0);

	fv -= ~delta * (*dqn);

	delta[0] = (*saux)[0] * (*dU)(0,0);
	delta[1] = 0.5 * (*saux)[2] * (*dU)(1,0);

	fv -= ~delta * (*dqn);

}
