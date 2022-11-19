#pragma once
#include "FiniteElementInstance.h"

class CFiniteElementInstance3D :
	public CFiniteElementInstance
{
public:
    CFiniteElementInstance3D( CGeometricDomain &gd, CSpaceShapeApproxPtr< CFENode > &s );
	virtual ~CFiniteElementInstance3D(void);

	virtual CFiniteElementInstance3D* Clone( void ) const { return new CFiniteElementInstance3D( *this ); }

    //! Approximates all nodal results values if some are missing using linear shape functions and vertex values.
    /*!
    */
    virtual void ApproximateAllNodalResults( );

	//! Calculates  volume of the element
    /*!
	*/
	virtual double GetVolume( ) const;
};
