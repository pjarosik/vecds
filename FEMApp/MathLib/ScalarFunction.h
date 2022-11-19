#pragma once

#include "matrix.h"
#include "Space.h"

class CScalarFunction
{
public:
	CScalarFunction(unsigned d,const CSpace *s=0);
	~CScalarFunction(void);
//	operator double( const mvector &x ) const { return GetValue( x ); }
	unsigned GetDim() const { return dim; }
	virtual double value( const mvector &x, const CSpace *s=0 );
	virtual double Value( const mvector &X );
	virtual void grad( const mvector &x, mvector &g ) const;
	virtual void Grad( const mvector &x, mvector &G ) const;

protected:
	unsigned dim;
	const CSpace * space;
};
