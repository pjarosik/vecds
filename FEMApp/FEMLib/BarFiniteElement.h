#pragma once
#include "FiniteElementInstance.h"
#include "../MathLib/GeometricDomain.h"

class CFiniteElementInstanceBar :
	public CFiniteElementInstance
{
public:
    CFiniteElementInstanceBar(CGeometricDomain &gd , const CSpaceShapeApproxPtr< CFENode > &s );
	virtual ~CFiniteElementInstanceBar(void);

	 //! Global coordinate finder
    /*!
        \param xi global coordinates (input)
        \param X  local coordinates (output)
    */
    virtual void GetXi( const mvector &X, mvector &xi ) const;

    //! Local coordinate finder
    /*!
        \param X global coordinates (input)
        \param xi local coordinates (output)
    */
    virtual void GetX( const mvector &X, mvector &xi ) const;

	virtual void GetTransformMatrix( matrix &L ) const=0;
    double ComputeLength() const;
};
