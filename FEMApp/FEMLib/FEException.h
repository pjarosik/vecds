#pragma once

#include <string>
using namespace std;

class CFEException
{
public:

	CFEException(
			const string &msg
			):message( msg )
	{
	}

	~CFEException(void)
	{
	}
	const string& GetMessage() const { return message; }
protected:
	string message;
};
