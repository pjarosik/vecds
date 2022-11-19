#include "StatementTitle.h"


CStatementTitle::CStatementTitle(CFEMProject &f ):FP( f )
{
}


CStatementTitle::~CStatementTitle(void)
{
}

void CStatementTitle::Execute( istream &i )
{
	string title;
	i >>  title;
	FP.SetJobTitle( title );
}