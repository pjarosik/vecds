#pragma once
#include "SpaceShape.h"



class CSpaceShapeLagrange :
    public CSpaceShape
{
public:
	//! CShapeFunctions constructor
    /*!
	  \param dim - dimension of the problem 2-2D, 3 - 3D etc.
	  \param dg - polynominal degree + 1. ( e.g. 2 for linear shape functions )
    */
    CSpaceShapeLagrange( unsigned dim, unsigned dg );
    CSpaceShapeLagrange( const CSpaceShapeLagrange &s ):CSpaceShape( s ) { }

public:

    virtual ~CSpaceShapeLagrange( void );

};
