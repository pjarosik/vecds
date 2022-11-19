#pragma once
#include <memory.h>
#include <string>
#include <ostream>
#include <fstream>
#include "assert.h"


using namespace std;

/*

struct mem_data
{
	double **who;
	unsigned len;
	double data;
};

*/



class CMatrixMemoryManager
{
	static unsigned nbuf, index, elems,peak_elems, mmem,peak_mmem, tmem, peak_tmem, nclean, nextend, buffer_size;
	static char *memory, *pointer;
public:
	CMatrixMemoryManager( void );
	~CMatrixMemoryManager( void );
	static void Allocate( double **who, unsigned amount );
	static void view_content(  ostream &out );
	static void initialize();
	static void Deallocate( double * );

protected:

	static void read_data( char *memory, unsigned &len, double ***who )
	{

		 len = *( (unsigned*) memory );
		 *who = *( (double***)  &memory[ sizeof(unsigned)] );

                #ifdef _DEBUG
                    assert( len < buffer_size );
                #endif
	}

	static void save_data( char *memory, unsigned len, double **who )
	{
                #ifdef _DEBUG
                    assert( len < buffer_size );
                #endif

		 *( (unsigned*) memory ) = len;
		 double ***who_ptr = (double***)&memory[ sizeof(unsigned) ];
		 *who_ptr = who;
	}

	static void print_memory( ostream &out );




    static void clean();

	static unsigned data_size( unsigned len )
	{
		return sizeof( unsigned ) + sizeof( double** ) + sizeof( double ) * len;
	}

//	static mem_data smd;

};
