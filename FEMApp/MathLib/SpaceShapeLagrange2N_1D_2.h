#pragma once
#include "SpaceShapeLagrange2N.h"

class CSpaceShapeLagrange2N_1D_2 :
	public CSpaceShapeLagrange2N
{
public:
    //! CShapeFunctions constructor
    /*!
	  
    */

    CSpaceShapeLagrange2N_1D_2();
    CSpaceShapeLagrange2N_1D_2( const CSpaceShapeLagrange2N_1D_2 &s ):CSpaceShapeLagrange2N(s) { }
    //CSpaceShapeLagrange2N_1D_2( CCopyer &cpr, const CSpaceShapeLagrange2N_1D_2 &s ):CSpaceShapeLagrange2N(s) { }

    //! CShapeFunctions destructor
    /*!
	  
    */
    virtual ~CSpaceShapeLagrange2N_1D_2(void);

    CLONE_OBJECT( CSpaceShapeLagrange2N_1D_2 )
//    COPY_OBJECT( CSpaceShapeLagrange2N_1D_2 )

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

};
