#pragma once

#include <string>
using namespace std;

class CMathException
{
public:

    CMathException( const string &msg ):message( msg )	{ 	}
    ~CMathException(void)	{	}
	const string& GetExceptionMessage() const { return message; }

protected:

	string message;
};
