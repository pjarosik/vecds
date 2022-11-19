#pragma once
#include "../FEMLib/IntegrationPoints.h"

class CIntegrationPointsMultiTriangular :
	public CIntegrationPoints
{
public:
	CIntegrationPointsMultiTriangular(int dm, int dg );
	virtual ~CIntegrationPointsMultiTriangular(void);
	virtual void SetFE( CFiniteElementInstance *f );
protected:
	vector< mvector >  t_points;
	vector< double >  t_weights;
};
