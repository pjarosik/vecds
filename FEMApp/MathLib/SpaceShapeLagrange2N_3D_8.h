#pragma once
#include "SpaceShapeLagrange2N.h"

class CSpaceShapeLagrange2N_3D_8 :
	public CSpaceShapeLagrange2N
{
public:
        //! CSpaceShape constructor
    /*!
	  
    */

    CSpaceShapeLagrange2N_3D_8();

    //! CSpaceShape destructor
    /*!
	  
    */
    virtual ~CSpaceShapeLagrange2N_3D_8(void);

    //CSpaceShapeLagrange2N_3D_8( CCopyer &cpr, const CSpaceShapeLagrange2N_3D_8 &s ):CSpaceShapeLagrange2N( s ) { }
    CLONE_OBJECT(CSpaceShapeLagrange2N_3D_8)
//    COPY_OBJECT(CSpaceShapeLagrange2N_3D_8)

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
