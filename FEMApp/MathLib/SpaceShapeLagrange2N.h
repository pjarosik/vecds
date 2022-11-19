#pragma once

#include "SpaceShapeLagrange.h"

class CSpaceShapeLagrange2N :
    public CSpaceShapeLagrange
{
public:
	//! CShapeFunctions constructor
    /*!
	  \param n - number of nodes in element
	  \param dim - dimension of the problem 2-2D, 3 - 3D etc.
    */

    CSpaceShapeLagrange2N(unsigned dim);

	//! CShapeFunctions destructor
    /*!
	  
    */
	virtual ~CSpaceShapeLagrange2N(void);

    CSpaceShapeLagrange2N( const CSpaceShapeLagrange2N &s ):CSpaceShapeLagrange( s ) {  }

protected:
	//! one dimensional Lagrange aproximation shape function calculations
    /*!
	  \param l - array of shape functions
	  \param x - coordinate [-1,1]
    */
    void  Calc_L( double x, double  (&l)[2] ) const;

	//! one dimensional Lagrange aproximation shape function and it's gradients calculations
    /*!
	  \param dl - array of shape functions and its gradients
	  \param x - coordinate [-1,1]
    */
	void Calc_dL(  double x, double  (&dl)[2][4] ) const;
};
