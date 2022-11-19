#pragma once
#include "IntegrationPoints.h"

class CGaussPointsTriangular :
	public CIntegrationPoints
{
public:
	CGaussPointsTriangular(int dm, int dg );
	virtual ~CGaussPointsTriangular(void);

};
