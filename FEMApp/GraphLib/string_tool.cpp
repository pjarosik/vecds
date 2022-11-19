#include "string_tool.h"



string string_tool::ExtractExtension( const string &filename )
{
	string ext;
	unsigned i = (unsigned)filename.find_last_of('.');
	ext.assign( filename, i , filename.length() - i );
	return ext;
}

