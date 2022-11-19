#pragma once
#include "SpaceShapeLagrange4N.h"

class CSpaceShapeLagrange4N_2D_16 :
    public CSpaceShapeLagrange4N
{
public:
        //! CSpaceShape constructor
    /*!
	  
    */

    CSpaceShapeLagrange4N_2D_16();

    //! CSpaceShape destructor
    /*!
	  
    */
    virtual ~CSpaceShapeLagrange4N_2D_16(void);

    CSpaceShapeLagrange4N_2D_16( const CSpaceShapeLagrange4N_2D_16 &s ):CSpaceShapeLagrange4N( s ) {  }
    //CSpaceShapeLagrange4N_2D_16( CCopyer &cpr, const CSpaceShapeLagrange4N_2D_16 &s ):CSpaceShapeLagrange4N( s ) {  }
    CLONE_OBJECT(CSpaceShapeLagrange4N_2D_16)
//    COPY_OBJECT(CSpaceShapeLagrange4N_2D_16)

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
