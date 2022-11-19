#define _USE_MATH_DEFINES // for C++
#include <cmath>

#include "FEFunctorVectorUauxK2.h"

CFEFunctorVectorUauxK2::CFEFunctorVectorUauxK2( CCrackInInterface *c, CFEFunctorVectorLagrangeX *x ):CFEFunctorVector("CFEFunctorVectorUauxK1"),X( x )
{
	AddNode( X );
}

CFEFunctorVectorUauxK2::CFEFunctorVectorUauxK2( const CFEFunctorVectorUauxK2 &vf ):CFEFunctorVector(vf)
{
	CLONE_MEMBER( vf, X )
}

CFEFunctorVectorUauxK2::~CFEFunctorVectorUauxK2( void )
{

}

void CFEFunctorVectorUauxK2::Compute( CFiniteElementInstance *fe1)
{

	double lg10 = log(10.0), ep = crack->eps, k, mu, r , t;
	mvector rt(2);

	fv.SetDim(2);
	crack->tip2->GetPolarSpace().transform( (*X), rt );
	
	r = rt[0];
	t = rt[1];

	if ( crack->H( *X ) > 0.0 ) 
	{
		k = crack->k1;
		mu = crack->mu1;
		fv[0] =  sqrt(r)*exp(-ep*t-M_PI*ep)*(4*(ep*ep)*exp(2*ep*t)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))+exp(2*ep*t)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))+2*ep*k*sin(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)+k*cos(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)+2*ep*exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*sin(t/2)+exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*sin(t/2)-k*sin(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)+2*ep*k*cos(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)+exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*cos(t/2)-2*ep*exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*cos(t/2))/(2*sqrt((double)2.0)*sqrt(M_PI)*(4*(ep*ep)+1)*cosh(M_PI*ep)*mu);
		fv[1] =  sqrt(r)*exp(-ep*t-M_PI*ep)*(4*(ep*ep)*exp(2*ep*t)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+exp(2*ep*t)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))-k*sin(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)+2*ep*k*cos(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)-exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*sin(t/2)+2*ep*exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*sin(t/2)-2*ep*k*sin(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)-k*cos(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)+2*ep*exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*cos(t/2)+exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*cos(t/2))/(2*sqrt((double)2.0)*sqrt(M_PI)*(4*(ep*ep)+1)*cosh(M_PI*ep)*mu);
	}
	else
	{
		k = crack->k2;
		mu = crack->mu2;
		fv[0] =  sqrt(r)*exp(-ep*t-M_PI*ep)*(4*(ep*ep)*exp(2*ep*t+2*M_PI*ep)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))+exp(2*ep*t+2*M_PI*ep)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))+2*ep*k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)+k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)+2*ep*sin(ep*log(r)/lg10)*sin(t/2)+cos(ep*log(r)/lg10)*sin(t/2)-k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)+2*ep*k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)-sin(ep*log(r)/lg10)*cos(t/2)+2*ep*cos(ep*log(r)/lg10)*cos(t/2))/(2*sqrt((double)2.0)*sqrt(M_PI)*(4*(ep*ep)+1)*cosh(M_PI*ep)*mu);
		fv[1] =  sqrt(r)*exp(-ep*t-M_PI*ep)*(4*(ep*ep)*exp(2*ep*t+2*M_PI*ep)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+exp(2*ep*t+2*M_PI*ep)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))-k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)+2*ep*k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)-sin(ep*log(r)/lg10)*sin(t/2)+2*ep*cos(ep*log(r)/lg10)*sin(t/2)-2*ep*k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)-k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)-2*ep*sin(ep*log(r)/lg10)*cos(t/2)-cos(ep*log(r)/lg10)*cos(t/2))/(2*sqrt((double)2.0)*sqrt(M_PI)*(4*(ep*ep)+1)*cosh(M_PI*ep)*mu);

	}

}





