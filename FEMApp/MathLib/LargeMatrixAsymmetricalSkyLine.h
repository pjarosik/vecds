// LargeMatrixAsymmetricalSkyLine.h: interface for the CLargeMatrixAsymmetricalSkyLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LargeMatrixAsymmetricalSkyLine_H__04D4AA98_C87E_455C_91FE_6B11EC232733__INCLUDED_)
#define AFX_LargeMatrixAsymmetricalSkyLine_H__04D4AA98_C87E_455C_91FE_6B11EC232733__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LargeMatrix.h"

class CLargeMatrixAsymmetricalSkyLine : public CLargeMatrix  
{
public:
    
        CLargeMatrixAsymmetricalSkyLine();
	CLargeMatrixAsymmetricalSkyLine( const CLargeMatrixAsymmetricalSkyLine &K );
	virtual ~CLargeMatrixAsymmetricalSkyLine();
    
        virtual void print();
        virtual void Copy( CLargeMatrix &K );
	virtual bool Create(unsigned ndof);
	virtual bool Decompose(CProgress *pr = 0) =0;
	mvector operator*( const mvector &m );
	virtual void Sum( double,CLargeMatrix* );
	virtual void SetZero();
	virtual void Reset();

        unsigned* GetLProfile() const { return Lprf; }
        unsigned* GetUProfile() const { return Uprf; }
	virtual bool CreateStructure( CLargeMatrix *K );
	virtual bool CreateMatrix( );
	virtual void UpdateProfile( const valarray<unsigned> &alr, const valarray<unsigned> &alc );
	virtual void FindFactorC( );
	virtual void AllocateMatrix( valarray<unsigned> &alr, valarray<unsigned> &alc, const matrix &m );
	virtual void EraseDof( unsigned dof );
	virtual void Set( CLargeMatrix &M );
	virtual bool Solve( const mvector &b, mvector &x );
	virtual void MultiplyColumn( double* W, unsigned ndof, double wr );
	virtual void Add( double w, CLargeMatrix  *M );
	virtual void MultiplyVector( const mvector &in, mvector &out ) const;
        double* GetData() const { return data; }
        virtual void operator=( const matrix &m ) { SetMatrix(m); }
        virtual void SetMatrix( const matrix &m );
	
        double Up(unsigned r,unsigned c) const 
        { 
            #ifdef _DEBUG
				assert( c >= r );
                                assert ( r >= Uprf[c] );
		#endif
            return U[c][c-r]; 
        }
        double Lr(unsigned r,unsigned c) const  
        { 
            #ifdef _DEBUG
				assert( r >= c );
                                assert( c >= Uprf[r] );
		#endif
            return L[r][r-c]; 
        }
        double& Up(unsigned r,unsigned c) 
        { 
             #ifdef _DEBUG
				assert( c >= r );
                                assert ( r >= Uprf[c] );
		#endif
            return U[c][c-r]; 
        }
        double& Lr(unsigned r,unsigned c) 
        { 
             #ifdef _DEBUG
				assert( r >= c );
                                assert( c >= Uprf[r] );
		#endif
            return L[r][r-c]; 
        }

protected:
    
	void ina( unsigned row,  unsigned col,  double w );
	unsigned *Uprf, *Lprf;
        double **U, **L, *data;
        bool decomposed;
};

#endif // !defined(AFX_LargeMatrixAsymmetricalSkyLine_H__04D4AA98_C87E_455C_91FE_6B11EC232733__INCLUDED_)
