#include "ERF.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
using namespace std;

CERF::CERF(void)
{
}

CERF::~CERF(void)
{
}

double CERF::integrate( double x )
{
	double k,n=10000,dx=x/(double)n,w1=0,w2=dx,res=0.0;
	for (k=0;k<n; k++)
	{
		w1 = exp( - k*dx*k*dx );
		w2 = exp( - (k+1.0)*dx*(k+1.0)*dx );
		res +=  w1 + w2;
	}
	res *= dx / sqrt( M_PI );

	return res;
}

/*
2*x*exp(-pow(x,2))*(1024*pow(x,20)+10752*pow(x,18)+102144*pow(x,16)+868224*pow(x,14)+6511680*pow(x,12)+42325920*pow(x,10)+232792560*pow(x,8)+1047566520*pow(x,6)+3666482820*pow(x,4)+9166207050*pow(x,2)+13749310575)/(13749310575*sqrt(M_PI))

1-exp(-pow(x,2))*(1024*pow(x,20)-512*pow(x,18)+768*pow(x,16)-1920*pow(x,14)+6720*pow(x,12)-30240*pow(x,10)+166320*pow(x,8)-1081080*pow(x,6)+8108100*pow(x,4)-68918850*pow(x,2)+654729075)/(1024*sqrt(M_PI)*pow(x,21))

*/
double CERF::approx1( double x )
{
	return 0.0;// 2*x*exp(-pow(x,2))*(1024*pow(x,20)+10752*pow(x,18)+102144*pow(x,16)+868224*pow(x,14)+6511680*pow(x,12)+42325920*pow(x,10)+232792560*pow(x,8)+1047566520*pow(x,6)+3666482820*pow(x,4)+9166207050*pow(x,2)+13749310575)/(13749310575*sqrt(M_PI));
}

double CERF::iter1(double x)
//erf(x) = 2/sqrt(pi)*integral(exp(-t^2),t,0,x)
// = 2/sqrt(pi)*[x - x^3/3 + x^5/5*2! - x^7/7*3! + ...]
// = 1-erfc(x)
{
static const double two_sqrtpi= 1.128379167095512574; // 2/sqrt(pi)
static const double rel_error= 1e-10; // 2/sqrt(pi)
//if (fabs(x) > 2.2) {
//return 1.0 - erfc(x); //use continued fraction when fabs(x) > 2.2
//}
double sum= x, term= x, xsqr= x*x;
int j = 1;
do {
	term *= xsqr/j;
	sum  -= term/(2*j+1);
	++j;
	term *= xsqr/j;
	sum  += term/(2*j+1);
	++j;
} while ( fabs( term) / sum > rel_error );
 cout << "j=" << j << endl;
return two_sqrtpi * sum;
}

double CERF::iter2(double x)
{
	double sum = x, il = x, xx = x * x,mb; // fac = 1, mb;
	int n = 1;
//	unsigned n2 = 3;
	static const double eps = 1e-10;
	do {
		il  *=  -xx / n;
		mb   =   il / ( 2 * n + 1 ) ;
		sum +=   mb;
		n++;
//		n2 += 2;
	} while( fabs( mb /sum ) > eps );
	cout << "n=" << n << endl;
	return M_2_SQRTPI * sum;
}

double CERF::iter3(double x)
{
	double sum = 1.0 / x, il = 1.0 / x, xx =  x * x; //, fac = 1; //, cc=1.0;
	int n = 1;
	static const double eps = 1e-10;
	do {
//		fac *=
		il  *=  - ( 2 * n - 1 ) / xx / 2.0;
		sum +=   il;
		n++;
	} while( fabs( il / sum ) > eps );
	cout << "n=" << n << endl;
	return M_2_SQRTPI * sum;
}



 //calculate 12 significant figures
//you can adjust rel_error to trade off between accuracy and speed
//but don't ask for > 15 figures (assuming usual 52 bit mantissa in a double)


double CERF::erf(double x)
//erf(x) = 2/sqrt(pi)*integral(exp(-t^2),t,0,x)
// = 2/sqrt(pi)*[x - x^3/3 + x^5/5*2! - x^7/7*3! + ...]
// = 1-erfc(x)
{
	static const double rel_error= 1E-12;
	static const double two_sqrtpi= 1.128379167095512574; // 2/sqrt(pi)
	if (fabs(x) > 2.2) {
		return 1.0 - erfc(x); //use continued fraction when fabs(x) > 2.2
	}
	double sum= x, term= x, xsqr= x*x;
	int j= 1;
	do
	{
		term*= xsqr/j;
		sum-= term/(2*j+1);
		++j;
		term*= xsqr/j;
		sum+= term/(2*j+1);
		++j;
	} while (fabs(term)/sum > rel_error);
	return two_sqrtpi*sum;
}


double CERF::erfc(double x)
//erfc(x) = 2/sqrt(pi)*integral(exp(-t^2),t,x,inf)
// = exp(-x^2)/sqrt(pi) * [1/x+ (1/2)/x+ (2/2)/x+ (3/2)/x+ (4/2)/x+ ...]
// = 1-erf(x)
//expression inside [] is a continued fraction so '+' means add to denominator only
{
	static const double rel_error= 1E-12;
	static const double one_sqrtpi= 0.564189583547756287; // 1/sqrt(pi)
	if (fabs(x) < 2.2) {
		return 1.0 - erf(x); //use series when fabs(x) < 2.2
	}
	if ( x < 0.0 ) { //continued fraction only valid for x>0
		return 2.0 - erfc(-x);
	}
	double a=1, b=x; //last two convergent numerators
	double c=x, d=x*x+0.5; //last two convergent denominators
	double q1, q2= b/d; //last two convergents (a/c and b/d)
	double n= 1.0, t;
	do
	{
		t= a*n+b*x;
		a= b;
		b= t;
		t= c*n+d*x;
		c= d;
		d= t;
		n+= 0.5;
		q1= q2;
		q2= b/d;
	} while (fabs(q1-q2)/q2 > rel_error);
	return one_sqrtpi*exp(-x*x)*q2;
}


