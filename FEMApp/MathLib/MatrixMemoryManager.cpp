#include "MatrixMemoryManager.h"
#include <assert.h>

#include <iostream>
#include <fstream>

unsigned CMatrixMemoryManager::mmem			= 0;
unsigned CMatrixMemoryManager::peak_mmem	= 0;
unsigned CMatrixMemoryManager::tmem			= 0;
unsigned CMatrixMemoryManager::peak_tmem	= 0;

unsigned CMatrixMemoryManager::elems		= 0;
unsigned CMatrixMemoryManager::peak_elems	= 0;
unsigned CMatrixMemoryManager::nbuf			= 0;
unsigned CMatrixMemoryManager::index		= 0;
unsigned CMatrixMemoryManager::nclean		= 0;
unsigned CMatrixMemoryManager::buffer_size	= 50000000;
char* CMatrixMemoryManager::memory			= 0;
char* CMatrixMemoryManager::pointer			= 0;



CMatrixMemoryManager::CMatrixMemoryManager(void)
{
	initialize();
}

void CMatrixMemoryManager::initialize(void)
{
	memory   = new char[ buffer_size ];
	pointer  = memory;
	nbuf     = 1;
}

CMatrixMemoryManager::~CMatrixMemoryManager(void)
{
	delete [] memory;
}


void CMatrixMemoryManager::view_content( ostream &out )
{
//	unsigned size = 0, counter = 0;
//	char *mem = memory;

	out << "*** matrix memory allocation ***" << endl;
	out << "matrix memory          = " << mmem << endl;
	out << "peak matrix memory     = " << peak_mmem << endl;
	out << "total memory           = " << tmem << endl;
	out << "peak total memory      = " << peak_tmem << endl;
	out << "peak  elems            = " << peak_elems << endl << endl;
	out << "total undeleted elems  = " << elems << endl;
	out << "cleaned                = " << nclean << endl << endl;
	out << "total buffer size      = " << nbuf * buffer_size << endl << endl;

}

void CMatrixMemoryManager::Allocate( double **who, unsigned nelems )
{
	#ifdef _DEBUG
			assert( nelems );
			assert( who );
			assert( sizeof(double) * nelems < buffer_size );
	#endif


	if ( ( nbuf * buffer_size - index ) <  ( sizeof( double** ) + sizeof( unsigned ) + sizeof( double ) * nelems ) ) clean();

	unsigned   *len_ptr = (unsigned*)&memory[ index ];
	double   ***who_ptr = (double***)&memory[ index + sizeof( unsigned ) ];

	*len_ptr = nelems;
	*who_ptr = who;

	*who = (double*)&memory[ index + sizeof( unsigned ) + sizeof(double**) ];

	elems++;

	unsigned ds = data_size( nelems );

	mmem  += ds;
	tmem  += ds;
	index += ds;

	if ( elems > peak_elems ) peak_elems = elems;
	if ( mmem > peak_mmem ) peak_mmem = mmem;



}

void CMatrixMemoryManager::Deallocate( double *mdt )
{

	if ( !mdt ) return;

	char *cm = (char*)mdt;
	cm -= sizeof( double** );
	double ***md = (double***)cm;
	*md = 0;
	cm -= sizeof( unsigned );
	unsigned *len_ptr = (unsigned*)cm;
	mmem  -= data_size( *len_ptr );
	elems --;


}

void CMatrixMemoryManager::clean( )
{

	if (  mmem > buffer_size * ( nbuf - 1 ) ) nbuf++;
	char *new_mem = new char[ nbuf * buffer_size ];

	#ifdef _DEBUG
		assert( new_mem );
	#endif

	double **owho;
	unsigned old_index = 0, new_index = 0,
			 olen;

//        print_memory( cout );

	while( old_index < index )
	{
		read_data( &memory[ old_index ], olen, &owho );
		if ( owho != 0 )
		{
			save_data( &new_mem[ new_index ], olen, owho );
			*owho = (double*)&new_mem[ new_index + sizeof(unsigned) + sizeof(double**) ];
			memcpy( &new_mem[ new_index + sizeof(unsigned) + sizeof(double**) ], &memory[ old_index + sizeof(unsigned) + sizeof(double**) ], sizeof(double) * olen );
			new_index += data_size( olen );
		}
		old_index += data_size( olen );
	}

	delete [] memory;
	memory = new_mem;
	index  = new_index;

	nclean++;

}


void CMatrixMemoryManager::print_memory( ostream &out )
{
	view_content( out );

	double **owho,*data;
	unsigned old_index = 0, olen,k;

	while( old_index < index )
	{
		read_data( &memory[ old_index ], olen, &owho );
		out << "pointer :" << (void*)&memory[ old_index ] << endl;
		out << "who     :" << owho << endl;
		out << "length  :" << olen << endl;
		out << "data    :" ;
		data = (double*)&memory[ old_index + sizeof(unsigned) + sizeof(double**) ];
		for (k=0; k<olen; k++) out << data[k] << ", ";
		old_index += data_size( olen );
		out << endl << endl;
	}

}
