
#ifndef CXMLEXCEPTION_H
#define CXMLEXCEPTION_H

#include <string>
#include <istream>
using namespace std;

class CXML_Parser_Exception
{
    public:
        CXML_Parser_Exception( istream &i, const string &msg ):message("XML EXCEPTION :"+msg),in(i) 
		{ 
			ExtracttLine(); 
		}
        const string& GetExceptionMessage( ) const { return message; }
		const string& GetLine( ) const { return line; }
protected:
	istream &in;
    string message,line;        

    void ExtracttLine();
};
 
inline void CXML_Parser_Exception::ExtracttLine( )  
{ 
	while ( (in.peek()=='\n' || in.peek()=='\r') && ( in.tellg() > 1 ) ) in.unget();
	while ( in.peek()!='\n' && in.peek()!='\r' && ( in.tellg() > 1 ) ) in.unget();
	in.get();
	char buffer[ 5000 ];
	buffer[0]=0;
	in.getline( buffer, 5000 );
	line = buffer;
}

class CXML_Exception
{
    public:
        CXML_Exception( const string &msg ):message("XML EXCEPTION :"+msg) { }
        const string& GetExceptionMessage( ) const { return message; }
		string GetLine(  );
protected:
    string message;        
};
 


#endif // CXMLPARAMETER_H
