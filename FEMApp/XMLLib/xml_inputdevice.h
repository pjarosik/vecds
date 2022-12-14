// Class automatically generated by Dev-C++ New Class wizard

#ifndef CXML_INPUTDEVICE_H
#define CXML_INPUTDEVICE_H

/**
 * No description
 */

#include "xml_exceptions.h" 
#include <fstream>
#include <string>
#include <vector>

using namespace std;
 
#define BUFFER_MAX 20000
 
class CXML_InputDevice
{
    unsigned char buffer[ BUFFER_MAX ];
    unsigned pos, data_size;
    ifstream file;
	public:
		// class constructor
		CXML_InputDevice( const string &filename );
		// class destructor
		virtual ~CXML_InputDevice() { if ( file.is_open() ) file.close( );  }

	bool IsASCII( unsigned char c ) { return  c > 32 && c <= 127  ? true : false; }
	bool IsXMLName( unsigned char c ) { return  ( c >= 63 && c <= 126 ) || ( c >= 48 && c <= 59 ) || ( c == '-' )  ? true : false; }
	bool IsXMLParameter( unsigned char c ) { return  (c >= 97 && c <= 122) || (c == '_' ) ||(c >= 65 && c <= 90) || (c >= 48 && c <= 58)  || (c == '-' )? true : false; }
    void PassWhite();
    void GetStringUpToChar( unsigned char c, string &tx );
    void GetStringUpToString( const string &end, string &tx  );
    void GetASCIIString( string &tx );
    void GetParameterString( string &tx );
    void GetNameString( string &tx );
    char GetChar();
protected:		
    bool end_of_buffer() { return pos == data_size; }
    void load_buffer( );
	CXML_InputDevice():pos( 0 ) { }
};

inline CXML_InputDevice::CXML_InputDevice( const string &filename ):pos(0) 
{ 
	file.open( filename.c_str() , ios::in | ios::binary );
	if ( !file.is_open() ) throw CXML_Parser_Exception( file, "can't open file :" + filename );
    load_buffer();
}

inline void CXML_InputDevice::PassWhite()
{
    while( buffer[ pos ] <=32 ) 
    { 
        pos++; 
        if ( end_of_buffer() ) load_buffer(); 
    }
}

inline void CXML_InputDevice::GetASCIIString( string &tx )
{
   	tx="";
    while( IsASCII( buffer[ pos ] ) )
    {
        tx += buffer[ pos ];
        pos++;
        if ( end_of_buffer() ) load_buffer();
    }
}

inline void CXML_InputDevice::GetParameterString( string &tx )
{
   	tx="";
    while( IsXMLParameter( buffer[ pos ] ) )
    {
        tx += buffer[ pos ];
        pos++;
        if ( end_of_buffer() ) load_buffer();
    }
}

inline void CXML_InputDevice::GetStringUpToChar( unsigned char c, string &tx )
{
   	tx="";
    while( buffer[ pos ] != c )
    {
        tx += buffer[ pos ];
        pos++;
        if ( end_of_buffer() ) load_buffer();
    }
}

inline void CXML_InputDevice::GetNameString( string &tx )
{
	tx = "";
    while( IsXMLName( buffer[ pos ] ) )
    {
        tx += buffer[ pos ];
        pos++;
        if ( end_of_buffer() ) load_buffer();
	}
}

inline char CXML_InputDevice::GetChar( )
{
	char c = buffer[ pos++ ];
	if ( end_of_buffer() ) load_buffer();
	return c;
}

inline  void CXML_InputDevice::load_buffer()
{
	file.read( (char*)buffer, BUFFER_MAX );
	data_size = (unsigned)file.gcount();
	if ( !data_size ) throw CXML_Parser_Exception(file,"unexpected end of file");
	pos = 0;
}



#endif // CXML_INPUTDEVICE_H
