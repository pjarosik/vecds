#pragma once
#include "FiniteElementInstance.h"
#include "TriangularSubelement.h"

class CFiniteElementInstance2D :
	public CFiniteElementInstance
{

public:

    CFiniteElementInstance2D( CGeometricDomain &gd, CSpaceShapeApproxPtr< CFENode > &s );
	virtual ~CFiniteElementInstance2D( void );

	virtual CFiniteElementInstance2D* Clone( void ) const { return new CFiniteElementInstance2D( *this ); }

    //! Approximates all nodal results values if some are missing using linear shape functions and vertex values.
    /*!
    */
    virtual void ApproximateAllNodalResults( );

	//! Calculates  area of finite elements (if applicable)
    /*!
	*/
	virtual double GetArea( ) const;
    //! Calculates  volume of the element
    /*!
    */
    virtual double GetVolume( ) const;

	void AddTriangle( CTriangularSubelement &se ) { subelems.push_back( se ); }
	const vector< CTriangularSubelement >& GetSubElems() const { return subelems; }
	void SubDivide(unsigned level);

protected:

	vector< CTriangularSubelement > subelems;

};
