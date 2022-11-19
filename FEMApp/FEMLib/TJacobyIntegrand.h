#pragma once


template< class v, class m, class I,class J >
class TJacobyIntegrand
{
public:

	TJacobyIntegrand( I &i, J &j )  {	}
	~TJacobyIntegrand(void)	{ 	}
	void operator( const C &c )

protected:
	I i;
	J j;
};


template< class v, class m, class I, class J >
m TJacobyIntegrand<J,I,C>::operator( const v &c )
{
	const m &jm = J( c );
	return jm.Det() * i( c );
}