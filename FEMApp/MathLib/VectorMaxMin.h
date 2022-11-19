#pragma once


template< class V >
class CVectorMaxMin
{
public:
	CVectorMaxMin( unsigned l, const V &v );
	~CVectorMaxMin(void);

	void Update( const V &m );

protected:
	void reset( const V &m );
	V vmax, vmin;
	unsigned len; 
};