#pragma once
#include "Progress.h"
#include <string>

using namespace std;

class CProgressConsole :
	public CProgress
{
public:
	CProgressConsole( const string nm, unsigned nb = 50 );
	virtual ~CProgressConsole( void );

protected:

	string name;
	bool painted;
	unsigned ind;
	const unsigned bn;

	virtual void Update();
};

