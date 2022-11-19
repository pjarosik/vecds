/*
 * File:   CLargeMatrixSparseCR.h
 * Author: piotrek
 *
 * Created on November 17, 2011, 9:55 AM
 */

#ifndef CLARGEMATRIXSPARSECR_H
#define	CLARGEMATRIXSPARSECR_H

#include "LargeMatrix.h"
#include <list>
#include <vector>

using namespace std;

class CLargeMatrixSparseCR : public CLargeMatrix {
public:
    class CSlice
    {
    public:
        const CLargeMatrixSparseCR &LM;
        const mvector &in;
        mvector &out;
        unsigned begin, end;
        CSlice( const CLargeMatrixSparseCR &lm,const mvector &i,mvector &o, unsigned b, unsigned e ):LM(lm),in(i),out(o),begin(b),end(e) { }
    };
    CLargeMatrixSparseCR( bool ss );
    CLargeMatrixSparseCR(const CLargeMatrixSparseCR& orig);
    virtual ~CLargeMatrixSparseCR();
    const CLargeMatrixSparseCR& operator=( const CLargeMatrixSparseCR &o );
    
    //!  Copy matrix
	/*!
		\param K CLargeMatrix
		Creates a copy..
	*/
	virtual void Copy( CLargeMatrix &K );

	//!  Reseting matrix
	/*!
		Deallocation memory.
	*/
	virtual void Reset();

	//!  Assigns 0 to all members
	/*!
		Assigns 0 to all members.
	*/
	virtual void SetZero();
    
    //!  Matrix accumulation
	/*!
		Adds other Largem matrix multiplied by a factor.
	*/
        virtual void Add( double w, CLargeMatrix *M ) { }
    //!  Initialises the matrix with given dimension
	/*!
		\param ndof dimension of the CLargeMatrix
		Initialises dimension of the matrix.
	*/
        
        //!  LargeMatrix assignment
	/*!
		Assigns LargeMatrix
	*/
        virtual void Set( CLargeMatrix &M ) { }
        //!  Initialises the matrix with given dimension
	/*!
		\param ndof dimension of the CLargeMatrix
		Initialises dimension of the matrix.
	*/
        virtual bool Create( unsigned ndof );
     
        //!  Matrix DOF erasing
	/*!
		Erases a DOF by nulyfying row and columns.
	*/
         virtual void EraseDof( unsigned dof );

	//!  Allocation of a matrix
	/*!
		\param al - allocation vector
		\param m - small (element) matrix
		Allocates the matrix based on given allocation vector.
	*/
	virtual void AllocateMatrix( valarray<unsigned> &alr, valarray<unsigned> &alc, const matrix &m );

	//!  Allocation of a matrix
	/*!
		Finds the greatest diagonal member an multiply it by 10e5;
	*/
	virtual void FindFactorC( );

	//!  Creation a matrix
	/*!
		Allocates memory for the matrix.
	*/
	virtual bool CreateMatrix( );

	//!  Copy structure
	/*!
		\param K CLargeMatrix
		Copies structure of other CLargeMatrix.
	*/
	virtual bool CreateStructure( CLargeMatrix *K );
     
     
     //!  Updates profile.
	/*!
		 Updates profile of a matrix using element allocation vector.
	*/
     virtual void UpdateProfile( const valarray<unsigned> &alr, const valarray<unsigned> &alc );

    //!  Gets CR matrix data.
   /*!
        Gets CR matrix data suitable for solution using external solver (like Pardiso, Mumps).
        \param a **double - matrix data
        \param ia **int -  rows data
        \param ja **int - columns data
   */
    virtual void GetMatrixCRData( double **a, int ** ia, int **ja ) { *a = data; *ia = rdata; *ja=cdata; }

     //!  Matrix decomposition
	/*!
		Decomposes a matrix.
	*/
     virtual bool Decompose(CProgress *pr = 0) { return true; }
     
     virtual void Sum( double, CLargeMatrix* );

    virtual void MultiplyColumn( double* W, unsigned ndof, double wr );
	virtual void MultiplyVector( const mvector &in, mvector &out ) const;
    virtual void MultiplyVectorSlice( CSlice& ) const;
	virtual void print();
    virtual void Read( const char *nm );
    virtual void Save( const char *nm );


protected:
    
    void _print();
    void CreateMatrixCRData1( bool IsZeroBasedInd = false );
    void CreateMatrixCRData2( bool IsZeroBasedInd = false );
    void CreateMatrixCRDataTestSym( bool IsZeroBasedInd = false );
    void CreateMatrixCRData2TestUnsym( bool IsZeroBasedInd = false );
    void CheckSizesForIncompleteFactorisation();

    bool symmetric_storage;
    vector< list< unsigned > > lines;
    vector< unsigned > blines;

    double *data;
    unsigned *cols_raw;
    unsigned *blines_raw;
    unsigned *diag_raw;
    int *rdata,*cdata, ird;

    const vector< list< unsigned > > & GetLinesStruct() const { return lines; }
    const vector< unsigned > & GetBeginLinesStruct() const { return blines; }
    const double* GetData() const { return data; }
    const unsigned* GetColsRaw() const { return cols_raw; }
    const unsigned* GetBeginLinesRaw() const { return blines_raw; }
};

#endif	/* CLARGEMATRIXSPARSECR_H */

