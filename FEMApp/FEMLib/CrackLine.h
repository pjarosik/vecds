#pragma once
#include "Crack.h"
#include "../MathLib/matrix.h"

class CCrackLine :
	public CCrack
{
public:
	
	CCrackLine( const mvector &a1, const mvector &a2 );
	virtual ~CCrackLine(void);

	virtual bool  XYZ( double t, mvector &X ) const;
	virtual bool dXYZ( double t, mvector dX ) const;
	virtual double angle( double t ) const;
	virtual double distance( const mvector &x ) const;
	virtual bool IsNear( const mvector &x, double eps=1e-6 ) const;

protected:

	mvector x1, x2;

};
