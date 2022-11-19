
#define _USE_MATH_DEFINES
#include <cmath>
#include "FEFunctorMatrixGradUauxK2.h"


CFEFunctorMatrixGradUauxK2::CFEFunctorMatrixGradUauxK2( CCrackInInterface *c, CFEFunctorVectorLagrangeX *x ):CFEFunctorMatrix("CFEFunctorMatrixUauxK1"),crack(c),X( x )
{
	AddNode( X );
}

CFEFunctorMatrixGradUauxK2::CFEFunctorMatrixGradUauxK2( const CFEFunctorMatrixGradUauxK2 &vf ):CFEFunctorMatrix(vf),crack(vf.crack)
{
	CLONE_MEMBER( vf, X )
}

CFEFunctorMatrixGradUauxK2::~CFEFunctorMatrixGradUauxK2( void )
{

}

void CFEFunctorMatrixGradUauxK2::Compute( CFiniteElementInstance *fe1 )
{

	double lg10 = log(10.0), ep = crack->eps, k, mu, r =0.0, t=0.0;
	matrix &vr = *this;
	mvector g(2), G(2), rt(2);
	matrix J;

	fv.SetDim(2,2);
	crack->tip2->transform( (*X), rt );

	r = rt[0];
	t = rt[1];



	crack->tip2->GetPolarSpace().jacobian( rt, J );
	J.Inverse();

	if ( crack->H( (*X) ) > 0.0 )
	{
		k = crack->k1;
		mu = crack->mu1;
		g[0] = -exp(-ep*t-M_PI*ep)*(8*(ep*ep*ep)*exp(2*ep*t)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+2*ep*exp(2*ep*t)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))-4*lg10*(ep*ep)*exp(2*ep*t)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))-lg10*exp(2*ep*t)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))-2*lg10*ep*k*sin(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)+2*ep*k*sin(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)-4*(ep*ep)*k*cos(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)-lg10*k*cos(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)-2*lg10*ep*exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*sin(t/2)+2*ep*exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*sin(t/2)-4*(ep*ep)*exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*sin(t/2)-lg10*exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*sin(t/2)+4*(ep*ep)*k*sin(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)+lg10*k*sin(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)-2*lg10*ep*k*cos(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)+2*ep*k*cos(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)-4*(ep*ep)*exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*cos(t/2)-lg10*exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*cos(t/2)+2*lg10*ep*exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*cos(t/2)-2*ep*exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*cos(t/2))/(4*sqrt((double)2.0)*sqrt(M_PI)*lg10*(4*(ep*ep)+1)*cosh(M_PI*ep)*mu*sqrt(r));
		g[1] =  -sqrt(r)*exp(-ep*t-M_PI*ep)*(exp(2*ep*t)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))-2*ep*exp(2*ep*t)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))-2*exp(2*ep*t)*cos(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))-k*sin(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)+exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*sin(t/2)-k*cos(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)-exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*cos(t/2))/(4*sqrt((double)2.0)*sqrt(M_PI)*cosh(M_PI*ep)*mu);
		G = J * g;
		vr(0,0) = G[0];
		vr(0,1) = G[1];

		g[0] =  exp(-ep*t-M_PI*ep)*(4*lg10*(ep*ep)*exp(2*ep*t)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+lg10*exp(2*ep*t)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+8*(ep*ep*ep)*exp(2*ep*t)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))+2*ep*exp(2*ep*t)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))-4*(ep*ep)*k*sin(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)-lg10*k*sin(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)+2*lg10*ep*k*cos(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)-2*ep*k*cos(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)-4*(ep*ep)*exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*sin(t/2)-lg10*exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*sin(t/2)+2*lg10*ep*exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*sin(t/2)-2*ep*exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*sin(t/2)-2*lg10*ep*k*sin(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)+2*ep*k*sin(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)-4*(ep*ep)*k*cos(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)-lg10*k*cos(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)+2*lg10*ep*exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*cos(t/2)-2*ep*exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*cos(t/2)+4*(ep*ep)*exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*cos(t/2)+lg10*exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*cos(t/2))/(4*sqrt((double)2.0)*sqrt(M_PI)*lg10*(4*(ep*ep)+1)*cosh(M_PI*ep)*mu*sqrt(r));
		g[1] =  sqrt(r)*exp(-ep*t-M_PI*ep)*(2*ep*exp(2*ep*t)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+2*exp(2*ep*t)*cos(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+exp(2*ep*t)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))+k*cos(ep*log(r)/lg10)*exp(2*ep*t)*sin(t/2)-exp(2*M_PI*ep)*cos(ep*log(r)/lg10)*sin(t/2)-k*sin(ep*log(r)/lg10)*exp(2*ep*t)*cos(t/2)-exp(2*M_PI*ep)*sin(ep*log(r)/lg10)*cos(t/2))/(4*sqrt((double)2.0)*sqrt(M_PI)*cosh(M_PI*ep)*mu);
		G = J * g;
		vr(1,0) = G[0];
		vr(1,1) = G[1];
	}
	else
	{
		k = crack->k2;
		mu = crack->mu2;
		g[0] =  -exp(-ep*t-M_PI*ep)*(8*(ep*ep*ep)*exp(2*ep*t+2*M_PI*ep)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+2*ep*exp(2*ep*t+2*M_PI*ep)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))-4*lg10*(ep*ep)*exp(2*ep*t+2*M_PI*ep)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))-lg10*exp(2*ep*t+2*M_PI*ep)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))-2*lg10*ep*k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)+2*ep*k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)-4*(ep*ep)*k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)-lg10*k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)-2*lg10*ep*sin(ep*log(r)/lg10)*sin(t/2)+2*ep*sin(ep*log(r)/lg10)*sin(t/2)-4*(ep*ep)*cos(ep*log(r)/lg10)*sin(t/2)-lg10*cos(ep*log(r)/lg10)*sin(t/2)+4*(ep*ep)*k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)+lg10*k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)-2*lg10*ep*k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)+2*ep*k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)+4*(ep*ep)*sin(ep*log(r)/lg10)*cos(t/2)+lg10*sin(ep*log(r)/lg10)*cos(t/2)-2*lg10*ep*cos(ep*log(r)/lg10)*cos(t/2)+2*ep*cos(ep*log(r)/lg10)*cos(t/2))/(4*sqrt((double)2.0)*sqrt(M_PI)*lg10*(4*(ep*ep)+1)*cosh(M_PI*ep)*mu*sqrt(r));
		g[1] =  -sqrt(r)*exp(-ep*t-M_PI*ep)*(4*(ep*ep)*exp(2*ep*t+2*M_PI*ep)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+exp(2*ep*t+2*M_PI*ep)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))-8*(ep*ep*ep)*exp(2*ep*t+2*M_PI*ep)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))-2*ep*exp(2*ep*t+2*M_PI*ep)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))-8*(ep*ep)*exp(2*ep*t+2*M_PI*ep)*cos(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))-2*exp(2*ep*t+2*M_PI*ep)*cos(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))-4*(ep*ep)*k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)-k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)+4*(ep*ep)*sin(ep*log(r)/lg10)*sin(t/2)-sin(ep*log(r)/lg10)*sin(t/2)+4*ep*cos(ep*log(r)/lg10)*sin(t/2)-4*(ep*ep)*k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)-k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)-4*ep*sin(ep*log(r)/lg10)*cos(t/2)+4*(ep*ep)*cos(ep*log(r)/lg10)*cos(t/2)-cos(ep*log(r)/lg10)*cos(t/2))/(4*sqrt((double)2.0)*sqrt(M_PI)*(4*(ep*ep)+1)*cosh(M_PI*ep)*mu);
		G = J * g;
		vr(0,0) = G[0];
		vr(0,1) = G[1];

		g[0] =  exp(-ep*t-M_PI*ep)*(4*lg10*(ep*ep)*exp(2*ep*t+2*M_PI*ep)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+lg10*exp(2*ep*t+2*M_PI*ep)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+8*(ep*ep*ep)*exp(2*ep*t+2*M_PI*ep)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))+2*ep*exp(2*ep*t+2*M_PI*ep)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))-4*(ep*ep)*k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)-lg10*k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)+2*lg10*ep*k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)-2*ep*k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)-4*(ep*ep)*sin(ep*log(r)/lg10)*sin(t/2)-lg10*sin(ep*log(r)/lg10)*sin(t/2)+2*lg10*ep*cos(ep*log(r)/lg10)*sin(t/2)-2*ep*cos(ep*log(r)/lg10)*sin(t/2)-2*lg10*ep*k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)+2*ep*k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)-4*(ep*ep)*k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)-lg10*k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)-2*lg10*ep*sin(ep*log(r)/lg10)*cos(t/2)+2*ep*sin(ep*log(r)/lg10)*cos(t/2)-4*(ep*ep)*cos(ep*log(r)/lg10)*cos(t/2)-lg10*cos(ep*log(r)/lg10)*cos(t/2))/(4*sqrt((double)2.0)*sqrt(M_PI)*lg10*(4*(ep*ep)+1)*cosh(M_PI*ep)*mu*sqrt(r));
		g[1] =  sqrt(r)*exp(-ep*t-M_PI*ep)*(8*(ep*ep*ep)*exp(2*ep*t+2*M_PI*ep)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+2*ep*exp(2*ep*t+2*M_PI*ep)*sin(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+8*(ep*ep)*exp(2*ep*t+2*M_PI*ep)*cos(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+2*exp(2*ep*t+2*M_PI*ep)*cos(t)*sin((lg10*t+2*ep*log(r))/(2*lg10))+4*(ep*ep)*exp(2*ep*t+2*M_PI*ep)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))+exp(2*ep*t+2*M_PI*ep)*sin(t)*cos((lg10*t+2*ep*log(r))/(2*lg10))+4*(ep*ep)*k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)+k*cos(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*sin(t/2)+4*ep*sin(ep*log(r)/lg10)*sin(t/2)-4*(ep*ep)*cos(ep*log(r)/lg10)*sin(t/2)+cos(ep*log(r)/lg10)*sin(t/2)-4*(ep*ep)*k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)-k*sin(ep*log(r)/lg10)*exp(2*ep*t+2*M_PI*ep)*cos(t/2)+4*(ep*ep)*sin(ep*log(r)/lg10)*cos(t/2)-sin(ep*log(r)/lg10)*cos(t/2)+4*ep*cos(ep*log(r)/lg10)*cos(t/2))/(4*sqrt((double)2.0)*sqrt(M_PI)*(4*(ep*ep)+1)*cosh(M_PI*ep)*mu);
		G = J * g;
		vr(1,0) = G[0];
		vr(1,1) = G[1];

	}
}


