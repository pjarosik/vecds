#include "StatementNodes.h"
#include "../LangLib/statdef.h"

CStatementNodes::CStatementNodes( CFEMProject &f ):FP( f )
{
}

CStatementNodes::~CStatementNodes(void)
{
}

void CStatementNodes::Execute( istream &in )
{
	TokenParam name(string("set"));
	NamedParameterDoubleListWhite2 coords(string("coords"));
	in >> name >> coords;

	if ( coords.GetParam().GetList().size() == 0 ) return;
	if ( coords.GetParam().GetList().size() % 3 ) throw CFEException( "Malformed nodes's coordinates list" );

	mvector x( 3 );
    unsigned i = 0,ni=0;
	while ( i < coords.GetParam().GetList().size() )
	{
		x[ 0 ] = coords.GetParam().GetList()[ i++ ];
		x[ 1 ] = coords.GetParam().GetList()[ i++ ];
		x[ 2 ] = coords.GetParam().GetList()[ i++ ];
        FP.AddNode(  name.GetParam().toString(), new CFENode( x ) );
	}

	cout << "readed nodes :" << FP.GetNodes().size() << endl;

}
