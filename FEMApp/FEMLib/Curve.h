#pragma once

#include "../MathLib/matrix.h"
#include "../MathLib/SpaceLine.h"

class CCurve
{
public:
	CCurve(double a,double b);
	virtual ~CCurve(void);

	virtual bool  XYZ( double t, mvector &X ) const=0;
	virtual bool dXYZ( double t, mvector dX ) const=0;
	void linearize( vector<mvector> &lc );

	bool Intersection( const CCurve &c, mvector &xs );

	double Xs() const { return 0.5 * ( t2 - t1 ); }
protected:
	double t1, t2;
	unsigned ndiv;
};
