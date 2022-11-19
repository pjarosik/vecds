/* 
 * File:   CLargeMatrixSkyLine2.h
 * Author: piotrek
 *
 * Created on December 4, 2011, 10:31 PM
 */

#ifndef CLARGEMATRIXSKYLINE2_H
#define	CLARGEMATRIXSKYLINE2_H

#include "Timer.h"
#include "LargeMatrix.h"

class CLargeMatrixSkyLine2  : public CLargeMatrix
{
	
public:

	CLargeMatrixSkyLine2( );
	CLargeMatrixSkyLine2( const CLargeMatrixSkyLine2 &K );
	virtual ~CLargeMatrixSkyLine2( ) { Reset(); }

//	virtual void MultiplyMatrix( CMatrixStruct in, CMatrixStruct out );
	virtual void print();
	virtual void Copy( CLargeMatrix &K );
	virtual void MultiplyVector( const mvector &in, mvector &out ) const;
	virtual void Add( double w, CLargeMatrix  *M );
	virtual void MultiplyColumn( double* W, unsigned ndof, double wr );
	virtual bool Solve( const mvector &b, mvector &x );
	virtual void Set( CLargeMatrix &M );
    virtual void EraseDof( unsigned dof );
    virtual void AllocateMatrix( valarray<unsigned> &alr, valarray<unsigned> &alc, const matrix &m );
    virtual void FindFactorC( );
	virtual void UpdateProfile( const valarray<unsigned> &alr, const valarray<unsigned> &alc );
	virtual bool CreateMatrix( );
	virtual bool Create( unsigned ndof );
        virtual bool CreateStructure( CLargeMatrix *K );
	virtual void Reset();
	virtual void SetZero();
        virtual void Sum(double,CLargeMatrix*);

	mvector operator*( const mvector &m );

	
	void Read( const char *filename );
	void Save( const char *name );

	
	void operator =( CLargeMatrixSkyLine2 &A );
	double* GetData( )            {  return data; }
	unsigned*    GetProfile( )    {  return  prf; }

protected:
	unsigned   *prf,  *imax,  minp,  maxp,  srp,  Mc_ind,  **bprf,  n_threads, *cprf;
	double    **mat,  *diag, *data;
        
        vector< unsigned > gprf;

	void ina( int wr,  int kol,  double w );
        double U(unsigned r,unsigned c) const { return mat[r][c-r]; }
        double L(unsigned row,unsigned col) const  { return mat[ row ][ row - col ];  }
        double& U(unsigned r,unsigned c) { return mat[r][c-r]; }
        double& L(unsigned r,unsigned c) { return mat[r][r-c]; }
        unsigned gPrf( unsigned k ) const { return k + prf[ k ]; }
	void SetRowExtremas( );
};


inline void CLargeMatrixSkyLine2::ina( int wr, int kol, double w )
{
        if ( wr < kol )                      return;
#ifdef _DEBUG
                unsigned ind = wr - kol;
                assert( ind < prf[ wr ] );
#endif
        L( wr, kol ) = w;
}


#endif	/* CLARGEMATRIXSKYLINE2_H */

