#include "ProgressConsole.h"
#include <iostream>

CProgressConsole::CProgressConsole( const string nm, const unsigned n ): name( nm ), painted( false ), bn( n )
{

}


CProgressConsole::~CProgressConsole( void )
{

}

void CProgressConsole::Update() 
{
	unsigned k;
	if ( !painted )
	{
		cout << endl << endl << name << " progress" << endl;
		cout << "|";
		for (k=0; k<bn; k++) cout << '_';
		cout << "|" << endl << ' ';
		ind=0;
		painted = true;
	}
	unsigned nind = (unsigned)( value / max * (double)bn ) + 1;
	for (k=ind; k<nind; k++) cout << '=';
	ind=nind;
	cout.flush();
}