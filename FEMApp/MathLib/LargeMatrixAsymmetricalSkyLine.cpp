// LargeMatrixAsymmetricalSkyLine.cpp: implementation of the CLargeMatrixAsymmetricalSkyLine class.
//
//////////////////////////////////////////////////////////////////////

#include "LargeMatrixAsymmetricalSkyLine.h"
#include <fstream>
#include <iomanip>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLargeMatrixAsymmetricalSkyLine::CLargeMatrixAsymmetricalSkyLine():Uprf(0),Lprf(0),U(0),L(0),data(0), decomposed(false)
{

}

CLargeMatrixAsymmetricalSkyLine::CLargeMatrixAsymmetricalSkyLine( const CLargeMatrixAsymmetricalSkyLine &K ):CLargeMatrix(K), decomposed( K.decomposed ),
        Uprf(0),Lprf(0),U(0),L(0),data(0)
{
	unsigned k;

	if ( K.Lprf )
	{
		Lprf = new unsigned[ dim ];
		memcpy( Lprf, K.Lprf, dim * sizeof(unsigned) );
	}

	if ( K.Uprf )
	{
		Uprf = new unsigned[ dim ];
		memcpy( Uprf, K.Uprf, dim * sizeof(unsigned) );
	}
        
	if ( K.L )	L = new double*[ dim ];
	if ( K.U )	U = new double*[ dim ];

	if ( K.data )
	{
		data = new double[ K.total_elem_number ];
		memcpy( data, K.data, total_elem_number * sizeof(double) );
	}

	unsigned counter=0;

	for ( k = 0; k < dim; k++ )
	{
                U[ k ] = &data[ counter ];
                counter += k - Uprf[ k ] + 1;
                L[ k ] = &data[ counter ];
                counter += k - Lprf[ k ] + 1;
        }

}

CLargeMatrixAsymmetricalSkyLine::~CLargeMatrixAsymmetricalSkyLine()
{
    Reset();
}

void CLargeMatrixAsymmetricalSkyLine::MultiplyVector(const mvector &in, mvector &out) const
{
    assert( decomposed == false );
    unsigned k,l;
    out.SetDim( dim );
    for ( k=0; k<dim; k++)
    {
        for ( l=Lprf[k]; l<=k; l++ ) out[ k ] +=  in[ l ] * Lr(k,l);
        for ( l=k; l<dim; l++ ) 
            if ( k >= Uprf[l] ) out[ k ] +=  in[ l ] * Up(k,l);
    }
}

void CLargeMatrixAsymmetricalSkyLine::Add( double w, CLargeMatrix *M )
{
   CLargeMatrixAsymmetricalSkyLine *A = (CLargeMatrixAsymmetricalSkyLine*) M ;
   double *m = A->GetData( );
   for ( unsigned int k = 0; k < total_elem_number; k++ ) data[ k ]  +=  w * m[ k ];
}

void CLargeMatrixAsymmetricalSkyLine::MultiplyColumn(double *W, unsigned ndof, double wr)
{
    assert( decomposed == false );
    unsigned k;
    
    for (k=Uprf[ndof]; k<=ndof; k++)
        W[ k ] += Up(k,ndof) * wr;
    
    for (k=ndof; k<dim; k++)
        if ( ndof >= Lprf[k] ) W[ k ] += Lr(k,ndof) * wr;


}

bool CLargeMatrixAsymmetricalSkyLine::Solve( const mvector &b, mvector &x )
{
    assert( decomposed == true );
    x.SetDim( dim );
    mvector y( dim );
    int i, j;
    double r;

  //  y[dim-1] = b[dim-1] / L[dim-1][0];
    
    for (i=0; i<(int)dim; i++)
    {
        r = 0.0;
        for( j = Lprf[i]; j < i ; j++ ) 
            r += Lr(i,j) * y[ j ];
        y[ i ] = ( b[ i ] - r ) / L[i][0];
    }

    x[ dim - 1 ] = y[ dim - 1 ] / U[ dim - 1 ][ 0 ];
    
    for (i = dim-2; i >= 0;  i-- )
    {
        r = 0.0;
        for( j = i + 1; j <dim; j++ ) 
            if ( i >= Uprf[ j ]) 
                r += Up(i,j) * x[ j ];
        x[ i ] = ( y[ i ] - r ) / U[ i ][ 0 ];
    }

    return true;
}

void CLargeMatrixAsymmetricalSkyLine::Set( CLargeMatrix &M )
{
    CLargeMatrixAsymmetricalSkyLine &A = ( CLargeMatrixAsymmetricalSkyLine& )M;
	memcpy( data , A.GetData( ), sizeof( double ) * total_elem_number );
}

void CLargeMatrixAsymmetricalSkyLine::EraseDof( unsigned ndof )
{
        assert( decomposed == false );
        unsigned k;
        
        for ( k = Uprf[ ndof ]; k < ndof; k++ ) Up( k, ndof ) = 0.0;
        for ( k = Lprf[ ndof ]; k < ndof; k++ ) Lr( ndof, k ) = 0.0;

        U[ ndof ][ 0 ] = factor_c;
        
        for ( k = ndof + 1; k<dim; k++)
        {
            if ( ndof >= Lprf[ k ] ) Lr( k, ndof ) = 0.0;
            if ( ndof >= Uprf[ k ] ) Up( ndof, k ) = 0.0;
        }

}

void CLargeMatrixAsymmetricalSkyLine::AllocateMatrix( valarray<unsigned> &alr, valarray<unsigned> &alc, const matrix &m )
{
    unsigned k, l;
	for ( k=0; k < alr.size(); k++ )
	  for ( l=0; l < alc.size(); l++ )
		  ina( alr[ k ] , alc[ l ] , m( k, l ) );
}

void CLargeMatrixAsymmetricalSkyLine::FindFactorC()
{
        assert( decomposed == false );
        factor_c = 0.0;
        for ( unsigned k = 0; k < dim; k++ )
                if ( U[ k ][ 0 ] > factor_c ) factor_c = U[ k ][ 0 ];

    factor_c *= 1.0e5;
}

void CLargeMatrixAsymmetricalSkyLine::ina(unsigned row, unsigned col, double w)
{
    if ( col < row ) Lr(row,col) += w;
    else             Up(row,col) += w;
}

void CLargeMatrixAsymmetricalSkyLine::UpdateProfile( const valarray<unsigned> &alr, const valarray<unsigned> &alc )
{
        int row, col;
        for (unsigned k=0; k<(unsigned)alr.size(); k++)
                for (unsigned l=0; l<(unsigned)alc.size(); l++)
                {
                    row = alr[ k ]; col = alc[ l ];
                    if ( col >= row ) if ( row < Uprf[ col ] )  Uprf[ col ] = row;
                    if ( col <= row ) if ( col < Lprf[ row ] )  Lprf[ row ] = col;
                }
}

bool CLargeMatrixAsymmetricalSkyLine::CreateMatrix()
{
    unsigned k,l, counter = 0;

        if ( data ) { delete [ ] data; data = 0; }
        if ( L  )   { delete [ ] L;  L  = 0; }
        if ( U  )   { delete [ ] U;  U  = 0; }

        L  = new  double*[ dim ];
        U  = new  double*[ dim ];
        
//        for ( k = 0; k < dim; k++ )
//               Lprf[ k ] = Uprf[ k ] = 0;
        
        for ( k = 0; k < dim; k++ ) total_elem_number += k - Lprf[ k ] + 1  + k - Uprf[ k ] + 1;

	data = new double[ total_elem_number ];
        memset( data, 0, sizeof( double ) * total_elem_number );

	for ( k = 0; k < dim; k++ )
	{
                U[ k ] = &data[ counter ];
                counter += k - Uprf[ k ] + 1;
                L[ k ] = &data[ counter ];
                counter += k - Lprf[ k ] + 1;
        }
    
   
    
    return true;
}

bool CLargeMatrixAsymmetricalSkyLine::CreateStructure(CLargeMatrix *K)
{
    CLargeMatrixAsymmetricalSkyLine *Ks = ( CLargeMatrixAsymmetricalSkyLine* )K;
    Reset();
    Create( Ks->GetDim() );
    memcpy(  Lprf , Ks->GetLProfile() , Ks->GetDim() * sizeof( unsigned ) );
    memcpy(  Uprf , Ks->GetUProfile() , Ks->GetDim() * sizeof( unsigned ) );
    return CreateMatrix();
}

void CLargeMatrixAsymmetricalSkyLine::Reset()
{
    dim        = 0;
    decomposed = false;
    
    delete [] L; L = 0; 
    delete [] U; U = 0; 

    delete [] Lprf; Lprf = 0; 
    delete [] Uprf; Uprf = 0; 

    delete [] data; data = 0; 
}

void CLargeMatrixAsymmetricalSkyLine::SetZero()
{
    decomposed = false;
    memset( data, 0, total_elem_number * sizeof( double ) );
}

void CLargeMatrixAsymmetricalSkyLine::Sum(double a, CLargeMatrix *K)
{
    CLargeMatrixAsymmetricalSkyLine *Ks = ( CLargeMatrixAsymmetricalSkyLine* )K;
    unsigned int k;
    double *kdata = Ks->GetData();
    for ( k=0; k<total_elem_number; k++) data[ k ] += a * kdata[ k ];
}

mvector CLargeMatrixAsymmetricalSkyLine::operator*( const mvector &m )
{
    #ifdef _DEBUG
		assert( m.GetRows( ) == dim );
    #endif

        mvector out(  m.GetDim() );

        unsigned k,l;
        for (k=0; k<dim; k++)
        {
            out[ k ] = 0.0;
            for(l=Lprf[k]; l<k; l++)
                out[k] += m[l] * Lr(k,l);
            for(l=k; l<dim; l++)
                if ( k>= Uprf[ l ] )
                        out[ k ] += m[ l ] * Up(k,l);
        }
                
        return out;
}

bool CLargeMatrixAsymmetricalSkyLine::Create(unsigned ndof)
{
        Reset( );
        dim = ndof;

        Lprf = new unsigned[ dim ];
        Uprf = new unsigned[ dim ];
 
        unsigned k;
        for (k=0; k<dim; k++ )
        {
            Lprf[k] = k;
            Uprf[k] = k;
        }
       
	L = U = 0;
	factor_c = 0.0;
	return true;
}

void CLargeMatrixAsymmetricalSkyLine::Copy(CLargeMatrix &K)
{
    CLargeMatrixAsymmetricalSkyLine &Ks=(CLargeMatrixAsymmetricalSkyLine&)K;
    memcpy( data, Ks.GetData(), total_elem_number * sizeof( double ) );
}

void CLargeMatrixAsymmetricalSkyLine::print()
{
     	ofstream file("CLargeMatrixAsymmetricalSkyLine.txt"  ,ios::app );
        int k,l;

        file << "Dim :" << dim << endl;
        file.setf( ios::scientific, ios::floatfield );
        file.setf( ios::right );
        file.precision( 12 );
        file << "Lower matrix " << endl;
        for (k=0; k<dim; k++)
        {
                for (l=0; l<=k; l++) 
                    file << setw(25) << ( ( l >= Lprf[ k ] ) ? Lr(k,l) : 0.0 );
                for (l=k+1; l<dim; l++) 
                    file << setw(25) << 0.0;
                file << endl;
        }

        file << endl << "Upper matrix " << endl;
        for (k=0; k<dim; k++)
        {
                
                for (l=0; l<k; l++) 
                    file << setw(25) << 0.0;
                
                for (l=k; l<dim; l++) 
                    file << setw(25) << ( ( k >= Uprf[ l ] ) ? Up(k,l) : 0.0 );
                file << endl;
        }
}

void CLargeMatrixAsymmetricalSkyLine::SetMatrix( const matrix &m )
{
    unsigned k,l;
    Reset();
    assert( m.GetRows() == m.GetCols() );
    Create( m.GetRows() );
    for ( k=0; k<dim; k++)
    {
        Lprf[ k ] = 0;
        Uprf[ k ] = 0;
    }
    for ( k=0; k < dim; k++ )
        for ( l=0; l < dim; l++ )
                ina( k , l , m( k, l ) );
}
