#pragma once

template< class M >
class CMatrixMaxMin
{
public:
	CMatrixMaxMin( unsigned r, unsigned c, const M &m );
	~CMatrixMaxMin(void);

	void Update( const M &m );

protected:
	void reset( const M &m );
	M max, min;
	unsigned rows, cols; 
};
