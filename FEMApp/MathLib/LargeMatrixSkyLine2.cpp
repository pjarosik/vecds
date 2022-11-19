/* 
 * File:   CLargeMatrixSkyLine2.cpp
 * Author: piotrek
 * 
 * Created on December 4, 2011, 10:31 PM
 */

#include "LargeMatrixSkyLine2.h"
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLargeMatrixSkyLine2::CLargeMatrixSkyLine2():prf(0),gprf(0), cprf(0),       imax(0), 
                                 minp(0),       maxp(0),  srp(0),
								 Mc_ind(0),     bprf(0), 
								 n_threads(1),   mat(0),
								 diag(0),       data(0)
{
	
}

CLargeMatrixSkyLine2::CLargeMatrixSkyLine2( const CLargeMatrixSkyLine2 &K ):CLargeMatrix( K )
{
	
}

void CLargeMatrixSkyLine2::Reset()
{
	unsigned k;
//        delete [] gprf; gprf = 0;
        delete [] cprf; cprf = 0;
	delete [] data; data = 0; 
	delete [] prf;  prf  = 0; 
        delete [] diag; diag = 0; 
        delete [] mat;  mat  = 0;
	delete [] imax; imax = 0; 
	if ( bprf )
	{
		for( k = 0; k < dim; k++ ) delete [ ] bprf;
		delete bprf; bprf = 0;
	}
	
	dim               = 0;
	total_elem_number = 0;

}

bool CLargeMatrixSkyLine2::Create(unsigned ndof)
{
	Reset();

	dim = ndof;
	
	prf = new unsigned[ dim ]; 
	memset( prf,    0, dim * sizeof( unsigned ) );
        
//        gprf = new unsigned[ dim ]; 
//	memset( gprf,    0, dim * sizeof( unsigned ) );
        
        gprf.resize( dim );
        
        cprf = new unsigned[ dim ]; 
	memset( cprf,    0, dim * sizeof( unsigned ) );

	mat      = 0;
        diag     = 0;
	factor_c = 0.0;
	return true;

}

bool CLargeMatrixSkyLine2::CreateMatrix()
{
	unsigned k, l, counter = 0;
	
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
                 gprf[ k ] = k - prf[ k ] + 1;
	}
        
        for ( k = 0; k < dim; k++ )
	{
            for (l=k; l<dim; l++)
                if ( k >= gprf[ l ] ) cprf[k] = l;
        }
    
        SetRowExtremas( );
        
        cout << "*** MATRIX PROPERTIES ***" << endl;
        cout << "dim               :" << dim << endl;
        cout << "total_elem_number :" << total_elem_number << endl;
        cout << "average bandwidth :" << total_elem_number / dim << endl;
        cout << "size              :" << ( total_elem_number * 8 ) / 1024 / 1024 << " MB" << endl;

	return true;
}

void CLargeMatrixSkyLine2::SetRowExtremas()
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


void CLargeMatrixSkyLine2::UpdateProfile(const valarray<unsigned> &alr, const valarray<unsigned> &alc )
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

void CLargeMatrixSkyLine2::FindFactorC()
{
	    factor_c = 0.0;

		for ( unsigned k = 0; k < dim; k++ ) 
			if ( mat[ k ][ 0 ] > factor_c ) factor_c = mat[ k ][ 0 ];
		factor_c *= 1.0e5;
}

void CLargeMatrixSkyLine2::AllocateMatrix( valarray<unsigned> &alr, valarray<unsigned> &alc, const matrix &m )
{
        unsigned k, l;
        for ( k=0; k < alr.size(); k++ ) 
                for ( l=0; l < alc.size(); l++ ) 
                        if ( alr[ k ] >=  alc[ l ] ) L( alr[ k ], alc[ l ] ) += m( k, l );
				  
//                  ina( (int)alr[ (unsigned)k ] , (int)alc[ (unsigned)l ] , m( (unsigned)k, (unsigned)l ) );
}



void CLargeMatrixSkyLine2::operator =(CLargeMatrixSkyLine2 &A)
{
   Reset();
   Create( A.GetDim() );
   memcpy(  prf , A.GetProfile() , dim * sizeof( double ) );
   CreateMatrix();
   memcpy( data , A.GetData()    , dim * sizeof( double ) );
}

/*
void CLargeMatrixSkyLine2::Save(const char *filename)
{
	    ofstream file( filename, ios::out | ios::binary );
		file.write( (const char*)&dim                     , sizeof(int) );
        file.write( (const char*)&total_elem_number       , sizeof(int) );
        file.write( (const char*)prf        , sizeof(int) * dim);
        file.write( (const char*)&factor_c  , sizeof(double) );
        file.write( (const char*)diag       , sizeof(double) * dim);
		file.write( (const char*)data       , sizeof(double) * total_elem_number );
		file.close(  ); 
}
*/

void CLargeMatrixSkyLine2::Read(const char *filename)
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

void CLargeMatrixSkyLine2::EraseDof(unsigned dof)
{
	int k;
	for ( k = gprf[dof]; k < dof;  k++ ) L(dof,k) = 0.0;
        for ( k = dof + 1; k < (int)dim; k++ ) 
            if ( dof >= gprf[ k ] ) L( k, dof ) = 0.0;
	
	L( dof, dof ) = factor_c;

}

void CLargeMatrixSkyLine2::Set( CLargeMatrix &M )
{
	CLargeMatrixSkyLine2 &A = ( CLargeMatrixSkyLine2& )M;
	memcpy( data , A.GetData( ), sizeof( double ) * total_elem_number );
}

bool CLargeMatrixSkyLine2::Solve( const mvector &b, mvector &x )
{
	
	double r;
	int i, k;
	solv_time.Start( );
	x = b;

        for ( i = 0; i < dim; i++ ) 
        {  
                r = 0.0;
                for ( k = gprf[i]; k < i; k++ ) r += L(i,k) * x[ k ];
                x[ i ]  = ( x[ i ] - r ) / L(i,i);
        }

	  
        for ( i = dim - 1; i >= 0; i--) 
        {
                r = 0.0;
                for ( k = i+1; k < dim; k++ ) 
                        if ( i >= (int) gprf[ k ] ) r += L( k, i ) * x[ k ];
                x[ i ] = ( x[ i ] - r ) / L(i,i);
        }

	  solv_time.Stop();

	  return true;
}

mvector CLargeMatrixSkyLine2::operator*( const mvector &m )
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


void CLargeMatrixSkyLine2::Add( double w, CLargeMatrix *M )
{
   CLargeMatrixSkyLine2 *A = (CLargeMatrixSkyLine2*) M ;
   double *m = A->GetData( );
   for ( unsigned int k = 0; k < total_elem_number; k++ ) 
									data[ k ]  +=  w * m[ k ];
}

void CLargeMatrixSkyLine2::MultiplyColumn( double* W, unsigned ndof, double wr )
{
	 unsigned a;
	 for ( a = 0; a <  prf[ ndof ];  a++ )  W[ ndof - a ] += mat[ ndof ][ a ] * wr;
     for ( a = 1; a < imax[ ndof ];  a++ )  if ( a  < prf[ ndof + a ] ) W[ ndof + a ] += mat[ a + ndof ][ a ] * wr;
}

void CLargeMatrixSkyLine2::SetZero()
{
	memset( data, 0, total_elem_number * sizeof( double ) );
}

bool CLargeMatrixSkyLine2::CreateStructure( CLargeMatrix *K )
{
    CLargeMatrixSkyLine2 *Ks = ( CLargeMatrixSkyLine2* )K;
    Reset();
    Create( Ks->GetDim() );
    memcpy(  prf , Ks->GetProfile() , Ks->GetDim() * sizeof( int ) );
    return CreateMatrix();
}

void CLargeMatrixSkyLine2::Sum( double a, CLargeMatrix *K )
{
    CLargeMatrixSkyLine2 *Ks = ( CLargeMatrixSkyLine2* )K;
    unsigned int k;
    double *kdata = Ks->GetData();
    for ( k=0; k<total_elem_number; k++) data[ k ] += a * kdata[ k ];
}

void CLargeMatrixSkyLine2::MultiplyVector( const mvector &in, mvector &out ) const
{
        unsigned k, a;
        for ( k=0; k<dim; k++)
        {
                for ( a = gprf[ k ]; a <=  k;  a++ ) out[ k ] += L( k, a ) * in[ a ];
                for ( a = k+1; a < dim;  a++ ) 
                        if ( a >= gprf[ a ] ) out[ k ] += L( a, k ) * in[ a ];
        }
}

void CLargeMatrixSkyLine2::Copy( CLargeMatrix &K )
{
    CLargeMatrixSkyLine2 &Ks=(CLargeMatrixSkyLine2&)K;
    memcpy( data, Ks.GetData(), total_elem_number * sizeof( double ) );
}

void CLargeMatrixSkyLine2::print()
{

	ofstream file( "CLargeMatrixSkyLine2.txt", ios::app );
	int k,l;

	file << "Dim :" << dim << endl;
	file.setf( ios::scientific, ios::floatfield );
	file.setf( ios::right );
	file.precision( 12 );
        file << "Lower matrix " << endl;
        for (k=0; k<dim; k++)
	{
		for ( l = 0; l < dim; l++ ) 
                    file << setw( 25 ) << ( ( l <= k ) && ( l >= gprf[ k ] ) ? L( k, l ) : 0.0 );
                file << endl;
	}

/*
        file << endl << "Upper matrix ";
        for (k=0; k<dim; k++)
	{
		file << endl << U_prf[ k ] << " :";
		for (l=0; l<dim; l++) 
                    file << setw(25) << ( ( l>=k ) && ( l - k < U_prf[ k ] ) ? U[ k ][ l - k ] : 0.0 );
	}
 * 
 */
	file.close();

}


void CLargeMatrixSkyLine2::Save( const char *name )
{
        unsigned k,l;
        ofstream fout( name );
        fout << dim << endl;
        fout << total_elem_number << endl;
        for( k = 0; k < dim; k++ )
        {
            for ( l = gprf[ k ]; l < dim; l++ )
                fout  << k + 1 << "    " << l + 1 << "    " <<  L( k, l ) << endl;
        }
}
