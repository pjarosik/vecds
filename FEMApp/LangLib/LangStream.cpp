
#include "LangStream.h"


bool CLangStream::ExtractCommand( string &cmd )
{
	if ( IsEof() )  CLangException( &ils, "File is empty " );
	ils >> cmd;
	if ( IsEof() ) 
		if ( cmd.size() ) CLangException( &ils, "Unexpected end of file " );
		else return false;
	return true;
}

