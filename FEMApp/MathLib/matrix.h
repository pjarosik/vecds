#pragma once

#include "MatrixMemoryManager.h"
#include "pointer.h"
#include <assert.h>

#define _USE_MATH_DEFINES
#include	<cmath>
#include <vector>
using namespace std;
//#include <cstdio>
//#include <stdarg.h>
#include "threads.h"


/*

*********************************************************************************************************************

   MATRIX CLACULUS CLASSES, basic version

   author P. Tauzowski (C) 2006

*********************************************************************************************************************

*/

class mvector;

class matrix
{
protected:

	pointer< double > data;
	unsigned rows, cols;
//    static Section memsection;

	void Create( unsigned r, unsigned c )
	{
        if ( ( rows == r ) && ( cols == c ) ) { SetZero();  return; }
		#ifdef _DEBUG
				assert( r * c > 0 );
		#endif
		if ( r * c <= rows * cols )
		{
			rows = r;  cols = c;
			SetZero();
			return;
		}
		Reset();
		rows = r;  cols = c;

//      enterSection( memsection );
//		CMatrixMemoryManager::Allocate( data.GetLocation(), r * c );
//      leaveSection(memsection);

        data.SetPointer( new double[ r * c ] );
		SetZero();
	}
	void Reset()
	{
//        enterSection( memsection );
  //          CMatrixMemoryManager::Deallocate( data.GetPointer() );
    //    leaveSection(memsection);
        delete [] data.GetPointer();
		data.SetPointer( 0 );
		rows  = 0;
		cols  = 0;
    }
public:

	 matrix( void ):data( 0 ), rows( 0 ), cols( 0 ) {  Create( 1, 1 ); }
	 matrix( const matrix &m );
	 matrix( unsigned r, unsigned c ):data( 0 ), rows( 0 ), cols( 0 ) {  Create(r,c); }
	 ~matrix( void );

	void GetDim( unsigned &r, unsigned &c ) const {  r = rows; c = cols;  }
	bool CheckDim( unsigned r, unsigned c ) const {  return ( r == rows ) && ( c == cols );  }
	void SetDim( unsigned r, unsigned c=1 ) { Create(r,c); }
	unsigned GetCols()  const  { return cols;         }
	unsigned GetRows()  const  { return rows;         }
	unsigned GetSize()  const  { return rows * cols;  }
	unsigned GetDataSize() { return rows * cols * sizeof( double );  }

	matrix& operator=( const matrix &m ) { Create(m.GetRows(),m.GetCols()); data.SetMem( m.GetPointer(), m.GetRows() * m.GetCols() ); return *this; }
	double operator=( double v )  { for(unsigned k=0; k<rows*cols; k++) data[k] = v; return v; }
	void operator+=( const matrix &m ) { for(unsigned k=0; k<rows*cols; k++) data[k] += m.GetPointer()[k]; }
	void operator-=( const matrix &m ) { for(unsigned k=0; k<rows*cols; k++)  data[ k ] -= m.GetPointer()[ k ];    }

	void operator*=( const double v ) { for(unsigned k=0; k<rows*cols; k++) data[ k ] *= v; }

	const pointer<double>& GetPointer() const { return data; }
    const double* GetDataPointer() const { return data.GetPointer(); }
    double* GetDataPointer() { return data.GetPointer(); }
    double operator () ( unsigned r, unsigned c ) const
	{
			 #ifdef _DEBUG
				assert( ( r < rows ) && ( c < cols ) );
			 #endif

			return data[ c * rows  + r ];
	}

    double& operator () ( unsigned r, unsigned c )
	{
			 #ifdef _DEBUG
				assert( ( r < rows ) && (c < cols ) );
			 #endif

			return data[ c * rows + r ];
	}

	void ExtractColumn( unsigned  col, matrix &vc ) const 
	{
		#ifdef _DEBUG
				assert( col < cols );
		#endif
		vc.SetDim(rows,1);
		for(unsigned k=0; k<rows; k++) vc(k,0) = data[ col * rows + k ];
	}

	void ExtractRow( unsigned row, matrix &vc ) const
	{
		#ifdef _DEBUG
				assert( row < rows );
		#endif
		vc.SetDim(cols,1);
		for(unsigned k=0; k<cols; k++) vc(k,0) = data[ k * rows + row ];
	}

	void SetZero() {  memset( data.GetPointer(), (char)0,  rows * cols  * sizeof(double) );  }

	double Det() const;
	void Inverse( ) ;
    void polard3d( matrix &r, matrix &u ) const;
    void eig3d( mvector &d, int &rot );
    void inv3d( matrix &b )const;
    void adj3d( matrix &b )const;
    void sym33to66( matrix &v ) const;
	void print( const string &label ) const;
	void Allocate( unsigned r, unsigned c, const matrix &m );

};


inline matrix::matrix( const matrix &m ):data( 0 ),rows(0),cols(0)
{
	Create( m.GetRows(), m.GetCols() );
	data.SetMem( m.GetPointer(), m.GetRows() * m.GetCols() );
}

inline ostream& operator << ( ostream &out, const matrix &m  )
{
    unsigned k, l, r = m.GetRows(), c = m.GetCols();
    out << "[";
    for (k=0; k<r; k++)
    {
        out << (k ? "," : "") << "[";
               for (l=0; l<c; l++) out << (l ? "," : "")  << m(k,l);
        out << "]";
    }
    out << "]";
    return out;
}

/*

*********************************************************************************************************************
  VECTOR CLASS
*********************************************************************************************************************
*/

class mvector :public matrix
{
	public:
		mvector( unsigned n ):matrix(n,1) { }
//		mvector( unsigned n, const double ):matrix(n,1) { }
		mvector( const matrix &mv ):matrix( mv ) { }
		mvector( const vector<double> &mv ):matrix( (unsigned)mv.size(), 1 ) { unsigned size = rows * cols,k; for( k=0; k<size; k++) data[ k ] = mv[ k ];  }
		mvector( ):matrix() { }
		~mvector( );

		double operator=( double v ) { unsigned size = rows * cols,k; for( k=0; k<size; k++) data[ k ] = v; return v; }
		matrix& operator=( const matrix &m ) { Create(m.GetRows(),m.GetCols()); data.SetMem( m.GetPointer(), m.GetRows() * m.GetCols() ); return *this; }
		void operator+=( const matrix &m ) { matrix::operator+=(m); }

//		void SetDim( unsigned d ) { Create(d,1); }
		unsigned GetDim() const { return GetRows() > GetCols() ? GetRows() : GetCols(); }
		void Normalize();
        string toString() const;
		double GetNorm() const;

        double operator[](unsigned i) const
		{
			#ifdef _DEBUG
				assert( i < rows );
			#endif

			return data[ i ];
		}

		double& operator[](unsigned i)
		{
			#ifdef _DEBUG
				assert( i < rows );
			#endif

			return data[ i ];
		}

};

inline double mvector::GetNorm() const
{
	double norm = 0.0;
	unsigned k;
	for (k=0; k<GetDim(); k++) norm += data[ k ] * data[ k ];
	return sqrt( norm );
}

inline void mvector::Normalize()
{
	double norm = GetNorm();
	unsigned k;
	for (k=0; k<GetDim(); k++) data[ k ] /= norm;
}

/*

*********************************************************************************************************************
  VECTOR CLASS
*********************************************************************************************************************
*/


inline matrix operator+( const matrix& m1, const matrix& m2 )
{
        matrix result(m1);
        result += m2;
        return result;
}

inline matrix operator-( const matrix& m1, const matrix& m2 )
{
        matrix result(m1);
        result -= m2;
        return result;
}

inline matrix operator*( const double v, const matrix& m2 )
{
        matrix result(m2);
        result *= v;
        return result;
}

inline matrix operator*( const matrix& m1, const matrix& m2 )
{
	unsigned dl = m1.GetCols(), m, k, l;
	assert( m1.GetCols() == m2.GetRows() );
	matrix odp( m1.GetRows(), m2.GetCols() );
	for (k=0; k<odp.GetRows(); k++)
		for (l=0; l<odp.GetCols(); l++)
			for (m=0; m<dl; m++) odp(k,l) += m1(k,m) * m2(m,l);
	return odp;
}


inline matrix operator~( const matrix &m )
{
	unsigned rows,cols,k,l;
        m.GetDim( rows, cols );
	matrix result( cols, rows );
	for (k=0; k<rows; k++)
		for (l=0; l<cols; l++) result(l,k) = m( k, l );
	return result;
}


inline mvector operator^( const mvector &w1, const mvector &w2 )
{
        mvector odp(3);

	odp[ 0 ] = w1[ 1 ] * w2[ 2 ] - w2[ 1 ] * w1[ 2 ];
	odp[ 1 ] = w2[ 0 ] * w1[ 2 ] - w1[ 0 ] * w2[ 2 ];
	odp[ 2 ] = w1[ 0 ] * w2[ 1 ] - w2[ 0 ] * w1[ 1 ];

        return odp;
}

inline double operator%( const mvector &w1, const mvector &w2 )
{
        #ifdef _DEBUG
		assert( w1.GetDim() == w2.GetDim() );
        #endif
        double res=0.0;
        unsigned k;
        for (k=0; k<w1.GetDim(); k++ )
            res += w1[ k ] * w2[ k ];
        return res;
}


inline ostream& operator << ( ostream &out, const mvector &v  )
{
    unsigned k, dim = v.GetDim();
    out << "[";
    for (k=0; k<dim; k++)
         out << (k ? "," : "")  << v[ k ];
    out << "]";
    return out;
}

