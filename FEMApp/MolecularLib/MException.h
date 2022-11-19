#pragma once

#include <string>
using namespace std;

class CMException
{
public:

	CMException(
			const string &msg
			):message( msg )
	{
	}

	~CMException(void)
	{
	}
	const string GetMessage() const { return message; }
protected:
	string message;
};
