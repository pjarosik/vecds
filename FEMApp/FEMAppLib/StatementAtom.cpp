#include "StatementAtom.h"
#include "../LangLib/statdef.h"
#include "../MolecularLib/MException.h"

CStatementAtom::CStatementAtom(CMolecularStructure &m):MS(m)
{
}

CStatementAtom::~CStatementAtom(void)
{
}

void CStatementAtom::Execute( istream &inp )
{

	TokenParam atomtype(string("type"));
	UnsignedListWhite2 coords(string("coords"));

	inp >> atomtype;
	inp >> coords;

	if ( coords.GetParam().GetList().size() % 3 ) throw CMException( "Malformed atom's coordinates list" );

	AtomStruct *as = MS.GetAtomType( atomtype.GetParam().toString() );
	if ( !as ) throw CMException( "Unknown atom type :" + atomtype.GetParam().toString() );
	mvector x(3);
	unsigned i = 0;
	while ( i < coords.GetParam().GetList().size() )
	{
		x[ 0 ] = coords.GetParam().GetList()[ i++ ];
		x[ 1 ] = coords.GetParam().GetList()[ i++ ];
		x[ 2 ] = coords.GetParam().GetList()[ i++ ];
		MS.CreateAtom( atomtype.GetParam().toString(), x );
	}

}

