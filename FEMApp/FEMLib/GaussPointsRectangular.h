// SetOfGaussPoints.h: interface for the CSetOfGaussPoints class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETOFGAUSSPOINTS_H__3E3383E1_E9A4_4259_A970_B9F6AE3709A2__INCLUDED_)
#define AFX_SETOFGAUSSPOINTS_H__3E3383E1_E9A4_4259_A970_B9F6AE3709A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../MathLib/matrix.h"
#include "../MathLib/constants.h"
#include "IntegrationPoints.h"

#include <vector> 
using namespace std;


class CGaussPointsRectangular : public CIntegrationPoints
{

public:

	CGaussPointsRectangular( int dm, int dg );
	virtual ~CGaussPointsRectangular( );
};

#endif // !defined(AFX_SETOFGAUSSPOINTS_H__3E3383E1_E9A4_4259_A970_B9F6AE3709A2__INCLUDED_)
