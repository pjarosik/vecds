#include "MolecularStructure.h"
#include "MException.h"

CMolecularStructure::CMolecularStructure( )
{

}

CMolecularStructure::~CMolecularStructure()
{
    unsigned k;
    for (k=0; k<atoms.size(); k++) delete atoms[k];

    map< string, AtomStruct* >::iterator i;
    for ( i=atom_types.begin(); i!=atom_types.end(); i++ ) delete i->second;
}

void CMolecularStructure::Init( const string &filename )
{
//	AtomStruct *astr=0;
    ifstream finp( filename.c_str() );

    if ( !finp.is_open() ) throw CMException("Can't find atoms definition file");

    AtomStruct ras,*as;
    while( !finp.eof() )
    {
        finp >> ras.an;     if ( finp.eof() ) break;
        finp >> ras.symbol; if ( finp.eof() ) break;
        finp >> ras.m1;     if ( finp.eof() ) break;
        finp >> ras.m2;     if ( finp.eof() ) break;
        finp >> ras.d1;     if ( finp.eof() ) break;
        finp >> ras.d2;     if ( finp.eof() ) break;
        finp >> ras.d3;     if ( finp.eof() ) break;
        finp >> ras.name;   if ( finp.eof() ) break;

        as = new AtomStruct( ras );
        atom_types[ as->symbol ] = as;

    }
}

CAtom* CMolecularStructure::CreateAtom( const string &symbol, const mvector &x )
{
    AtomStruct *as = atom_types[ symbol ];
    if (!as) return 0;
    CAtom *atom = new CAtom( *as, x );
    atoms.push_back( atom );
    return atom;
}


