#pragma once


class DoubleId
{
	double &x;
public :
	DoubleId( double &a ):x(a) { }
	operator double() { return x; }
};

inline const double my_pow(const double x, const unsigned n) { return n == 0 ? 1 : x * my_pow( x, n - 1 ); }

template<int N>
class ExFact {
public:
    enum { value = N * ExFact<N-2>::value };
};

template<>
class ExFact<1> {
public:
    enum { value = 1 };
};

template<>
class ExFact<0> {
public:
    enum { value = 1 };
};

class CERF
{
public:
	CERF( void );

public:
	~CERF( void );
	double ERF( double x ) { return 0; }
	static double integrate( double x );
	static double approx1( double x );
	static double iter1( double x );
	static double iter2( double x );
	static double iter3( double x );
	static double erf(double x);
	static double erfc(double x);
};


