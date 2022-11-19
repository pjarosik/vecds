// MatrixAllocators.h: interface for the CDynamicAllocator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIXALLOCATORS_H__FFFA0ADC_C769_434F_BE5B_887EC0414CCB__INCLUDED_)
#define AFX_MATRIXALLOCATORS_H__FFFA0ADC_C769_434F_BE5B_887EC0414CCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <malloc.h>
#include <assert.h>

template <int rows=1,int col=1>
class CDynamicMatrixAllocator  
{
    double  *data;
    double **mat;
public:
    CDynamicMatrixAllocator():data(0),mat(0) { }
    double* allocate( int r, int c );
    double** GetTablePtr() { return mat; }
    void Reset() { if ( data ) free( data ); data =0; if ( mat ) free( mat ); mat =0; }
    virtual ~CDynamicMatrixAllocator() { Reset( ); }

};

template <int rows,int cols>
inline double* CDynamicMatrixAllocator<rows,cols>::allocate( int r, int c )
{ 
    if ( !data )  data = (double*)malloc(        sizeof(double)* r * c  );
    else          data = (double*)realloc( data, sizeof(double)* r * c  );

    if ( !mat )   mat  = (double**)malloc(        sizeof(double*)* c );
    else          mat  = (double**)realloc( mat,  sizeof(double*)* c );

    for(int k=0; k<c; k++) mat[ k ] = &data[ k * r ];

    return data; 
}


template <int rows=1,int cols=1>
class CStaticMatrixAllocator  
{
    double data[ rows * cols ], *mac[ cols ];
public:
    CStaticMatrixAllocator( ) {  }
    double* allocate( int r, int c );
    double** GetTablePtr() { return mac; }
    void Reset( ) { }
    virtual ~CStaticMatrixAllocator() { }

};

template <int rows,int cols>
inline double* CStaticMatrixAllocator<rows,cols>::allocate( int r, int c )
{
    #ifdef _DEBUG
			assert( rows * cols >= r * c ); 
	#endif
    for(int k=0; k<c; k++) mac[ k ] = &data[ k * r ];
    return data;
}

#endif // !defined(AFX_MATRIXALLOCATORS_H__FFFA0ADC_C769_434F_BE5B_887EC0414CCB__INCLUDED_)
