#pragma once
#include "SpaceShapeLagrange3N.h"

class CSpaceShapeLagrange3N_2D_9 :
    public CSpaceShapeLagrange3N
{
public:
		//! CShapeFunctions constructor
    /*!
	  
    */

    CSpaceShapeLagrange3N_2D_9();

	//! CShapeFunctions destructor
    /*!
	  
    */
    virtual ~CSpaceShapeLagrange3N_2D_9(void);

    CSpaceShapeLagrange3N_2D_9( const CSpaceShapeLagrange3N_2D_9 &s ):CSpaceShapeLagrange3N( s ) {  }
    //CSpaceShapeLagrange3N_2D_9( CCopyer &cpr, const CSpaceShapeLagrange3N_2D_9 &s ):CSpaceShapeLagrange3N( s ) {  }
    CLONE_OBJECT(CSpaceShapeLagrange3N_2D_9)
    //COPY_OBJECT(CSpaceShapeLagrange3N_2D_9)

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

