// LargeMatrixSkyLine.h: interface for the CLargeMatrixSkyLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LargeMatrixSkyLine_H__91DA3AC2_66A9_4221_B7D5_DC0EC154D3E3__INCLUDED_)
#define AFX_LargeMatrixSkyLine_H__91DA3AC2_66A9_4221_B7D5_DC0EC154D3E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Timer.h"
#include "LargeMatrix.h"

class CLargeMatrixSkyLine  : public CLargeMatrix
{
	
public:

	CLargeMatrixSkyLine( );
	CLargeMatrixSkyLine( const CLargeMatrixSkyLine &K );
	virtual ~CLargeMatrixSkyLine( ) { Reset(); }

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

	
	void operator =( CLargeMatrixSkyLine &A );
	double* GetData( )            {  return data; }
	unsigned*    GetProfile( )    {  return  prf; }

protected:
	unsigned   *prf,  *imax,  minp,  maxp,  srp,  Mc_ind,  **bprf,  n_threads;
	double    **mat,  *diag, *data;

	void ina( int wr,  int kol,  double w );
	void SetRowExtremas( );
};


inline void CLargeMatrixSkyLine::ina( int wr, int kol, double w )
{
  	    if ( wr < kol )                      return;
        if ( ( wr == -1 ) || ( kol == -1 ) ) return;
        unsigned ind = wr - kol;

#ifdef _DEBUG
	    assert( ind < prf[ wr ] );
#endif
        mat[ wr ][ ind ] += w;
}


#endif // !defined(AFX_LargeMatrixSkyLine_H__91DA3AC2_66A9_4221_B7D5_DC0EC154D3E3__INCLUDED_)
