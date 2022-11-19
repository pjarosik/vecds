
#include "FEFunctorVectorStraindU.h"

CFEFunctorVectorStraindU::CFEFunctorVectorStraindU( CFEFunctorMatrix *du ):CFEFunctorVector("strain vector dU"), dU( du )
{
	AddNode( dU );
	const matrix &mdu = (*dU);
	unsigned dim = mdu.GetRows();
	
	if ( dim == 2 )
	{
		mnames.push_back("exx");
		mnames.push_back("eyy");
		mnames.push_back("gxy");
	}
	if ( dim == 3 )
	{
		mnames.push_back("exx");
		mnames.push_back("eyy");
		mnames.push_back("ezz");
		mnames.push_back("gxy");
		mnames.push_back("gzy");
		mnames.push_back("gzx");
	}

}

CFEFunctorVectorStraindU::CFEFunctorVectorStraindU( const CFEFunctorVectorStraindU &vf ):CFEFunctorVector(vf)
{
	CLONE_MEMBER( vf, dU )

}

CFEFunctorVectorStraindU::~CFEFunctorVectorStraindU(void)
{

}


void CFEFunctorVectorStraindU::Compute(CFiniteElementInstance *fe1 )
{
	const matrix &mdu = (*dU);
	unsigned dim = mdu.GetRows();
	const matrix &gradU = (*dU);
	if ( dim == 2 )
	{
		fv.SetDim(3);
		fv[0] = gradU(0,0);
		fv[1] = gradU(1,1);
		fv[2] = gradU(0,1) + gradU(1,0);
		return;
	}
	if ( dim == 3 )
	{
		fv.SetDim(6);
		fv[0] = gradU(0,0);
		fv[1] = gradU(1,1);
		fv[2] = gradU(2,2);
		fv[3] = gradU(0,1) + gradU(1,0);
		fv[4] = gradU(1,2) + gradU(2,1);
		fv[5] = gradU(0,2) + gradU(2,0);
		return;
	}
	assert(false);
}