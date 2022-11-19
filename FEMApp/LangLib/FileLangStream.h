#pragma once
#include "LangStream.h"
#include <fstream>



class CFileLangStream :
	public CLangStream
{
public:
	CFileLangStream( const string &filename );
	
	virtual void GetData();

	ifstream fin;
	virtual ~CFileLangStream(void);
};
