#pragma once
#include "FiniteElementInstance.h"

class CFiniteElementInstance1D :
	public CFiniteElementInstance
{
public:
    CFiniteElementInstance1D( CGeometricDomain &gd, CSpaceShapeApproxPtr< CFENode > &s );
	virtual ~CFiniteElementInstance1D(void);

		//! Clone functions
    /*!
		This member allows to create a copy of the object. Useful in mesh generation.	
    */
	virtual CFiniteElementInstance1D* Clone( void ) const { return new CFiniteElementInstance1D( *this ); }

    //! Approximates all nodal results values if some are missing using linear shape functions and vertex values.
    /*!
    */
    virtual void ApproximateAllNodalResults( );

	//! Calculates  volume of the element
    /*!
	*/
	virtual double GetVolume( ) const { return GetArea() * GetLength(); }

	//! Calculates  area of finite elements (if applicable)
    /*!
	*/
	virtual double GetArea( ) const { return material->GetValue("A"); }

	//! Calculates  length of the element (if applicable)
    /*!
	*/
	virtual double GetLength( ) const;
};
