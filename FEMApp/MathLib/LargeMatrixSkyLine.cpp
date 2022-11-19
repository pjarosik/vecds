// LargeMatrixSkyLine.cpp: implementation of the CLargeMatrixSkyLine class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "LargeMatrixSkyLine.h"
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLargeMatrixSkyLine::CLargeMatrixSkyLine():prf(0),        imax(0), 
                                 minp(0),       maxp(0),  srp(0),
								 Mc_ind(0),     bprf(0), 
								 n_threads(1),   mat(0),
								 diag(0),       data(0)
{
	
}

CLargeMatrixSkyLine::CLargeMatrixSkyLine( const CLargeMatrixSkyLine &K ):CLargeMatrix( K )
{
	
}

void CLargeMatrixSkyLine::Reset()
{
	unsigned k;
	if ( data ) { delete [] data; data = 0; }
	if ( prf  ) { delete [] prf;  prf  = 0; }
    if ( diag ) { delete [] diag; diag = 0; }
	if ( mat  ) { delete [] mat;  mat  = 0; }

	if ( imax ) { delete [] imax; imax=0; }
	if ( bprf )
	{
		for( k = 0; k < dim; k++ ) delete [ ] bprf;
		delete bprf; bprf = 0;
	}
	
	dim               = 0;
	total_elem_number = 0;

}

bool CLargeMatrixSkyLine::Create(unsigned ndof)
{
	Reset();

	dim = ndof;
	
	prf = new unsigned[ dim ]; 
	memset( prf,    0, dim * sizeof( unsigned ) );

	mat      = 0;
    diag     = 0;
	factor_c = 0.0;
	return true;

}

bool CLargeMatrixSkyLine::CreateMatrix()
{
	unsigned k, counter = 0;
	
	if ( data ) { delete [ ] data; data = 0; }
        if ( diag ) { delete [ ] diag; diag = 0; }
	if ( mat  ) { delete [ ] mat;  mat  = 0; }

	mat  = new  double*[ dim ];
        diag = new   double[ dim ];

	total_elem_number = 0;
	for ( k = 0; k < dim; k++ ) total_elem_number += prf[ k ];

	data = new double[ total_elem_number ];
        memset( data, 0, sizeof( double ) * total_elem_number );

	for ( k = 0; k < dim; k++ )
	{
		 mat[ k ] = &data[ counter ];
		 counter += prf[ k ];
	}
    
        SetRowExtremas( );
        
        cout << "*** MATRIX PROPERTIES ***" << endl;
        cout << "dim               :" << dim << endl;
        cout << "total_elem_number :" << total_elem_number << endl;
        cout << "average bandwidth :" << total_elem_number / dim << endl;
        cout << "size              :" << ( total_elem_number * 8 ) / 1024 / 1024 << " MB" << endl;

	return true;
}

void CLargeMatrixSkyLine::SetRowExtremas()
{
	if ( imax ) delete [ ] imax;
	imax = new unsigned[ dim ];
	for (unsigned k=0; k < dim; k++)
	{
		imax[ k ]  = dim - k;
		for (unsigned l = dim - 1; l > k; l--)
		if ( l - prf[ l ] + 1 <= k )
		{
			 imax[ k ] = l - k + 1;
			 break;
		}
	}
}


void CLargeMatrixSkyLine::UpdateProfile(const valarray<unsigned> &alr, const valarray<unsigned> &alc )
{
	int ind, wr;
	for (int k=0; k<(int)alr.size(); k++)
		for (int l=0; l<(int)alc.size(); l++)
		{
			wr  = (int)alr[ k ];
			ind = wr - (int)alc[ l ];
			if ( ind >= (int)prf[ wr ] )  prf[ wr ] = (ind + 1);

		}
}

void CLargeMatrixSkyLine::FindFactorC()
{
	    factor_c = 0.0;

		for ( unsigned k = 0; k < dim; k++ ) 
			if ( mat[ k ][ 0 ] > factor_c ) factor_c = mat[ k ][ 0 ];
		factor_c *= 1.0e5;
}

void CLargeMatrixSkyLine::AllocateMatrix( valarray<unsigned> &alr, valarray<unsigned> &alc, const matrix &m )
{
	   int k, l;
		  for ( k=0; k < (int)alr.size(); k++ ) 
			  for ( l=0; l < (int)alc.size(); l++ ) 
				  ina( (int)alr[ (unsigned)k ] , (int)alc[ (unsigned)l ] , m( (unsigned)k, (unsigned)l ) );
}



void CLargeMatrixSkyLine::operator =(CLargeMatrixSkyLine &A)
{
   Reset();
   Create( A.GetDim() );
   memcpy(  prf , A.GetProfile() , dim * sizeof( double ) );
   CreateMatrix();
   memcpy( data , A.GetData()    , dim * sizeof( double ) );
}


void CLargeMatrixSkyLine::Save(const char *filename)
{

    unsigned k,l;
    ofstream fout( filename );
    fout << dim << endl;
    fout << total_elem_number << endl;
    for( k = 0; k < dim; k++ )
    {
        for ( l = prf[ k ] - 1; l >= 0; l-- )
            fout  << k + 1 << "    " << dim - l << "    " << mat[ k ][ l ] << endl;
    }

/*
	    ofstream file( filename, ios::out | ios::binary );
		file.write( (const char*)&dim                     , sizeof(int) );
        file.write( (const char*)&total_elem_number       , sizeof(int) );
        file.write( (const char*)prf        , sizeof(int) * dim);
        file.write( (const char*)&factor_c  , sizeof(double) );
        file.write( (const char*)diag       , sizeof(double) * dim);
		file.write( (const char*)data       , sizeof(double) * total_elem_number );
		file.close(  ); 
*/

}

void CLargeMatrixSkyLine::Read(const char *filename)
{
        Reset();
		ifstream file( filename, ios::in | ios::binary );
		file.read(  (char*)&dim       , sizeof(unsigned) );    prf = new unsigned[ dim ];
        file.read(  (char*)&total_elem_number  , sizeof(int) );
        file.read(  (char*)prf         , sizeof(int) * dim );    

        CreateMatrix();

        file.read(  (char*)&factor_c  , sizeof(double) );
        file.read(  (char*)diag       , sizeof(double) * dim);
        file.read(  (char*)data       , sizeof(double) * total_elem_number );
        
		file.close( ); 
}

void CLargeMatrixSkyLine::EraseDof(unsigned dof)
{
	int k;
	for ( k = 1;       k < (int)prf[ dof ]; k++ ) mat[ dof ][ k ]     = 0.0;
    for ( k = dof + 1; k < (int)dim;        k++ ) if ( prf[ k ] > k - dof ) mat[ k ][ k - dof ] = 0.0;
	
	mat[ dof ][ 0 ] = factor_c;

}

void CLargeMatrixSkyLine::Set( CLargeMatrix &M )
{
	CLargeMatrixSkyLine &A = ( CLargeMatrixSkyLine& )M;
	memcpy( data , A.GetData( ), sizeof( double ) * total_elem_number );
}

bool CLargeMatrixSkyLine::Solve( const mvector &b, mvector &x )
{
	
	double r;
	int i, k;
	solv_time.Start( );
	x = b;

	  for ( i = 0; i < (int)dim; i++ ) 
	  {  
		  r = 0.0;
		  for ( k = 1; k < (int)prf[ i ]; k++ ) r += mat[ i ][ k ] * x[ i - k ];
	      x[ i ]  = ( x[ i ] - r ) / mat[ i ][ 0 ];
	  }

//    for (i=0; i<wym; i++) x(i) /= diag[i]; 
	  
	  for ( i = dim - 1; i >= 0; i--) 
	  {
		  r = 0.0;
          for ( k = 1; k < (int)imax[ i ]; k++ ) 
			  if ( k < (int)prf[ i + k ] ) r += mat[ k + i ][ k ] * x[ k + i ];
		 x[ i ] = ( x[ i ] - r ) / mat[ i ][ 0 ];
	  }

	  solv_time.Stop();

	  return true;
}

mvector CLargeMatrixSkyLine::operator*( const mvector &m )
{

#ifdef _DEBUG
		assert( m.GetRows( ) == dim );
#endif

/*
	mvector odp( dim, m.GetCols( ) );
	const double **macm   = m;
	double       **macodp = odp;
	unsigned k,l,a;
	for (l=0; l<odp.GetCols( ); l++)
		for (k=0; k<odp.GetRows( ); k++)
		{
			for (a = 0; a <  prf[ k ];  a++ )  macodp[ l ][ k ] += mat[ k ][ a ] * macm[ l ][ k - a ];
			for (a = 1; a < imax[ k ];  a++ )  if ( a  < prf[ k + a ] ) macodp[ l ][ k ] += mat[ a + k ][ a ] * macm[ l ][ k + a ];
		}
	return odp;
*/

	mvector res( m.GetCols( ) );
	unsigned k,a;
	for (k=0; k<res.GetRows( ); k++)
	{
			for (a = 0; a <  prf[ k ];  a++ )  res[ k ] += mat[ k ][ a ] * m[ k - a ];
			for (a = 1; a < imax[ k ];  a++ )  if ( a  < prf[ k + a ] ) res[ k ] += mat[ a + k ][ a ] * m[ k + a ];
	}
	return res;
}


void CLargeMatrixSkyLine::Add( double w, CLargeMatrix *M )
{
   CLargeMatrixSkyLine *A = (CLargeMatrixSkyLine*) M ;
   double *m = A->GetData( );
   for ( unsigned int k = 0; k < total_elem_number; k++ ) 
									data[ k ]  +=  w * m[ k ];
}

void CLargeMatrixSkyLine::MultiplyColumn( double* W, unsigned ndof, double wr )
{
	 unsigned a;
	 for ( a = 0; a <  prf[ ndof ];  a++ )  W[ ndof - a ] += mat[ ndof ][ a ] * wr;
     for ( a = 1; a < imax[ ndof ];  a++ )  if ( a  < prf[ ndof + a ] ) W[ ndof + a ] += mat[ a + ndof ][ a ] * wr;
}

void CLargeMatrixSkyLine::SetZero()
{
	memset( data, 0, total_elem_number * sizeof( double ) );
}

bool CLargeMatrixSkyLine::CreateStructure( CLargeMatrix *K )
{
    CLargeMatrixSkyLine *Ks = ( CLargeMatrixSkyLine* )K;
    Reset();
    Create( Ks->GetDim() );
    memcpy(  prf , Ks->GetProfile() , Ks->GetDim() * sizeof( int ) );
    return CreateMatrix();
}

void CLargeMatrixSkyLine::Sum( double a, CLargeMatrix *K )
{
    CLargeMatrixSkyLine *Ks = ( CLargeMatrixSkyLine* )K;
    unsigned int k;
    double *kdata = Ks->GetData();
    for ( k=0; k<total_elem_number; k++) data[ k ] += a * kdata[ k ];
}

void CLargeMatrixSkyLine::MultiplyVector( const mvector &in, mvector &out ) const
{
        unsigned k, a;
		for ( k=0; k<dim; k++)
		{
			for ( a = 0; a <  prf[ k ];  a++ ) out[ k ] += mat[ k     ][ a ] * in[ k - a ];
			for ( a = 1; a < imax[ k ];  a++ ) 
                 if ( a  < prf[ k + a ] )      out[ k ] += mat[ a + k ][ a ] * in[ k + a ];
		}
}

void CLargeMatrixSkyLine::Copy( CLargeMatrix &K )
{
    CLargeMatrixSkyLine &Ks=(CLargeMatrixSkyLine&)K;
    memcpy( data, Ks.GetData(), total_elem_number * sizeof( double ) );
}

void CLargeMatrixSkyLine::print()
{

        ofstream file("CLargeMatrixSkyLine.txt");
		unsigned k,l;
		file << dim << endl;
		file.setf( ios::scientific, ios::floatfield );
		file.setf( ios::left );
		file.precision( 12 );
        file << "diagonal" << endl;
        for (k=0; k<dim; k++)
            file << mat[k][0] << endl;
        file << endl;
        for (k=0; k<dim; k++)
		{
            file << setw(5) << k << ", " << setw(5) << prf[ k ] << " : ";
			for (l=0; l<prf[k]; l++) file << setw(18) << mat[k][l] << ", ";
			file << endl;
		}
		file.close();

}

