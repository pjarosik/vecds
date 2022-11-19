#pragma once
#include "SpaceShapeLagrange2N.h"

class CSpaceShapeLagrange2N_2D_4 :
	public CSpaceShapeLagrange2N
{
public:
        //! CSpaceShape constructor
    /*!
	  
    */

    CSpaceShapeLagrange2N_2D_4();

    //! CSpaceShape destructor
    /*!
	  
    */
    virtual ~CSpaceShapeLagrange2N_2D_4(void);

    CSpaceShapeLagrange2N_2D_4( const CSpaceShapeLagrange2N_2D_4 &s ):CSpaceShapeLagrange2N( s ) { }
    //CSpaceShapeLagrange2N_2D_4( CCopyer &cpr, const CSpaceShapeLagrange2N_2D_4 &s ):CSpaceShapeLagrange2N( s ) { }
    CLONE_OBJECT(CSpaceShapeLagrange2N_2D_4)
//    COPY_OBJECT(CSpaceShapeLagrange2N_2D_4)

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
