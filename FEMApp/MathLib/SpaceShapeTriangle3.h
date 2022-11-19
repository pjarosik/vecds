#pragma once
#include "SpaceShape.h"

class CSpaceShapeTriangle3 :
    public CSpaceShape
{
public:
    //! CSpaceShapeTriangle3 constructor
    /*!
	  
    */
    CSpaceShapeTriangle3(void);
    CSpaceShapeTriangle3( const CSpaceShapeTriangle3 &s ):CSpaceShape( s ) { }
    //CSpaceShapeTriangle3( CCopyer &cpr, const CSpaceShapeTriangle3 &s ):CSpaceShape( s ) { }

    //! CSpaceShapeTriangle3 destructor
    /*!
	  
    */
    virtual ~CSpaceShapeTriangle3(void);

//    COPY_OBJECT( CSpaceShapeTriangle3 )
    CLONE_OBJECT( CSpaceShapeTriangle3 )

    //! Shape function values
    /*!
	  \param xi a local coordinates of a point
	  \param sfn vector of shape function values at given point. (output)
    */
    virtual void transform( const mvector &xi,mvector &sfn ) const;

	//! Shape function gradient
    /*!
	  \param xi a local coordinates of a point
	  \param gsfn matrix of shape functions gradients at given point. (output)
    */
    virtual void jacobian( const mvector &xi,matrix &gsfn ) const;
protected:
    virtual void CreateShapes() { }
};
