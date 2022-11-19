// LargeMatrix.h: interface for the CLargeMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LARGEMATRIX_H__2173CEC7_B84C_4D8E_BD1C_92518A865287__INCLUDED_)
#define AFX_LARGEMATRIX_H__2173CEC7_B84C_4D8E_BD1C_92518A865287__INCLUDED_

#include "matrix.h"
#include "Timer.h" 	// Added by ClassView


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "matrix.h"
#include "Progress.h"
#include <valarray>
#include "threads.h"

using namespace std;

//!  FEM quation matrices
/*!
  Base class for large matrices used for global matrices of FEM equation.
*/
class CLargeMatrix
{

public:

	//!  CLargeMatrix constructor
	/*!

	*/
	CLargeMatrix( );

	//!  CLargeMatrix destructor
	/*!

	*/
	CLargeMatrix( const CLargeMatrix &lm );

	//!  CLargeMatrix destructor
	/*!

	*/
	virtual ~CLargeMatrix( );
	virtual CLargeMatrix* Clone() const = 0;

    virtual void Read( const char *nm )=0;
    virtual void Save( const char *nm )=0;

	CTimer dec_time, solv_time, agr_time;

	//!  Matrix accumulation
	/*!
		Adds other Largem matrix multiplied by a factor.
	*/
	virtual void Add( double w, CLargeMatrix *M )=0;

	//!  Matrix decomposition
	/*!
		Decomposes a matrix.
	*/
	virtual bool Decompose( CProgress *pr = 0 )=0;

	//!  Matrix solve
	/*!
		Calculates solution basedon right-hand vector and decomposed matrix.
	*/
	virtual bool Solve( const mvector &b, mvector &x )=0;

	//!  LargeMatrix assignment
	/*!
		Assigns LargeMatrix
	*/
	virtual void Set( CLargeMatrix &M )=0;

	//!  Matrix DOF erasing
	/*!
		Erases a DOF by nulyfying row and columns.
	*/
    virtual void EraseDof( unsigned dof )=0;

	//!  Allocation of a matrix
	/*!
		\param al - allocation vector
		\param m - small (element) matrix
		Allocates the matrix based on given allocation vector.
	*/
	virtual void AllocateMatrix( valarray<unsigned> &alr, valarray<unsigned> &alc, const matrix &m )=0;

	//!  Allocation of a matrix
	/*!
		Finds the greatest diagonal member an multiply it by 10e5;
	*/
	virtual void FindFactorC( )=0;

	//!  Creation a matrix
	/*!
		Allocates memory for the matrix.
	*/
	virtual bool CreateMatrix( )=0;

	//!  Copy structure
	/*!
		\param K CLargeMatrix
		Copies structure of other CLargeMatrix.
	*/
	virtual bool CreateStructure( CLargeMatrix *K )=0;

	//!  Initialises the matrix with given dimension
	/*!
		\param ndof dimension of the CLargeMatrix
		Initialises dimension of the matrix.
	*/
	virtual bool Create( unsigned ndof )=0;

	//!  Copy matrix
	/*!
		\param K CLargeMatrix
		Creates a copy..
	*/
	virtual void Copy( CLargeMatrix &K ) =0;

	//!  Reseting matrix
	/*!
		Deallocation memory.
	*/
	virtual void Reset()=0;

	//!  Assigns 0 to all members
	/*!
		Assigns 0 to all members.
	*/
	virtual void SetZero()=0;

	//!  Updates profile.
	/*!
		 Updates profile of a matrix using element allocation vector.
	*/
	virtual void UpdateProfile( const valarray<unsigned> &alr, const valarray<unsigned> &alc )=0;

	//!  Accumulates CLargeMatrix
	/*!
		Accumulates CLargeMatrix multiplied by a factor;
	*/
	virtual void Sum( double, CLargeMatrix* )=0;

	virtual void MultiplyColumn( double* W, unsigned ndof, double wr )=0;
	virtual void MultiplyVector( const mvector &in, mvector &out ) const =0;
	virtual void print()=0;
//	virtual void operator=( const matrix &m ) { }
    double DataSize() const { return sizeof( double ) * total_elem_number; }
    unsigned GetDim( ) const { return dim; }
    unsigned GetTotalElemNumber( ) const { return total_elem_number; }
	double  GetFactorC( ) { return factor_c; }
//    virtual void Save( const char *nm );

protected:
	unsigned  total_elem_number; /*!< total number of elements in the matrix */
	unsigned  dim; /*!< dimension of the matrix */
	double    factor_c; /*!< The bigest diagonal member multiplied by 10E5 */
    Section section;
};

/*

      ********************************************************************************
	***                                                                              ***
   ***      L A R G E    M A T R I X   O P E R A T O R S                              ***
	***                                                                              ***
	  ********************************************************************************

*/

/*inline CLargeExpressionMult< CMatrixSummation > operator*( CLargeMatrix &m1,  CMatrixSummation &ms )
{
    return CLargeExpressionMult< CMatrixSummation >( m1, ms );
}*/

matrix operator  *( const CLargeMatrix &, const mvector&);

#endif // !defined(AFX_LARGEMATRIX_H__2173CEC7_B84C_4D8E_BD1C_92518A865287__INCLUDED_)

