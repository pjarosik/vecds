#pragma once

#include "../MathLib/matrix.h"
#include "../MathLib/constants.h"
#include "FiniteElementInstance.h"
#include <vector> 

using namespace std;


class CIntegrationPoints
{
public:

	CIntegrationPoints( int t, int dm, int dg );
	virtual ~CIntegrationPoints( );
    bool Check( int t, int dm, int dg ) { return ( t == type ) && ( dim == dm ) && ( dg == degree );  }
	const mvector& GetKsi( int k )	const { return points[ k ];	}
	double GetWeight( int k )		const { return weights[ k ];	}
        int N()				const { return ip;			}
	int GetDegree()				const { return degree;		}
	int GetDim()					const { return dim;			}
	int GetType()					const { return type;		}
	const vector<mvector>& GetPoints() const { return points; }
	const vector<double>& GetWeights() const { return weights; }

protected:

	vector< mvector >  points;
	vector< double >  weights;
	int dim, degree, ip, type;

};
