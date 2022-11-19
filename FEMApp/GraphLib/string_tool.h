#ifndef __STRING_TOOL
#define __STRING_TOOL

#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

class string_tool
{
public:
	string_tool( void );
	static string CreateNumberedFilename( const string &basename, unsigned size, unsigned no, const string &ext );
	static string UnsignedIntToFixedString( const unsigned v )  { stringstream s; s << v; return s.str(); }
	static string IntToFixedString( const int v ) { stringstream s; s << v; return s.str(); }
	static string DoubleToFixedString( const unsigned size, const unsigned prec, const double value) { stringstream s;  s.setf( ios::fixed ); s.precision(prec); s << setw(size) << value; return s.str(); }
	static string DoubleToExpString( const unsigned size, const unsigned prec, const double value) { stringstream s;  s.setf(ios::scientific); s.precision(prec); s << setw(size) << value; return s.str(); }
	static string CheckPatchSeparator( const string &path, char sep );
	static string ExtractExtension( const string &filename );
	~string_tool( void );
};



inline string string_tool::CreateNumberedFilename( const string &basename, const unsigned size, const unsigned no, const string &ext )
{
	stringstream s;
	s << basename << '_' << setfill ('0') << setw( size )  << no << ext;
	return s.str();
}

inline string string_tool::CheckPatchSeparator( const string &path, char sep )
{
	if ( path[ path.length() ] != sep ) return  path + sep;
	return path;
}

#endif
