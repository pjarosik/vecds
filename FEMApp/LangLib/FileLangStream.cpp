
#include "FileLangStream.h"
#include <iostream>
#include <sstream>
#include <vector>

CFileLangStream::CFileLangStream( const string &filename ):fin( filename.c_str() )
{ 
	if ( !fin.is_open() )
		throw CLangException( &fin , "Can't open file :" + filename );	


	ils <<  fin.rdbuf();


/*

	fin.close();

	char buf[ 50000 ];
	while( !fin.eof() )
	{
		fin.getline( buf, 49999 );
		lines.push_back( buf );
	}

	nlines = (unsigned )lines.size();
	if ( nlines ) alen = (unsigned)lines[ 0 ].size();

*/

}

CFileLangStream::~CFileLangStream( void )
{

}


void CFileLangStream::GetData( )
{

}