#pragma once
#include "../MathLib/SpaceShapeTriangle3.h"
#include "../MathLib/matrix.h"
#include <vector>
using namespace std;

class CFiniteElementInstance2D;
class CTriangularSubelement
{
	vector< mvector > nodes;
	CFiniteElementInstance2D *master;
    CSpaceShapeTriangle3 sf;

public:

	CTriangularSubelement( CFiniteElementInstance2D *ms, const vector< mvector > &n );
	virtual ~CTriangularSubelement(void);
	void Subdivide( unsigned level );
	double Area() const;
	void GetVertex( vector<mvector> &cords )const;

	const vector< mvector >& GetNodes() const { return nodes; }

	void GetJacobyMatrix( const mvector &xi, matrix &J  ) const;
	void GetMasterCoords( const mvector &xi, mvector &mX  ) const;
	void GetKsi( const mvector &xi, mvector &Xi ) const;

};
