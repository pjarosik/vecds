#ifndef CMOLECULARSTRUCTURE_H
#define CMOLECULARSTRUCTURE_H

#include "../MathLib/matrix.h"
#include "Atom.h"
#include<vector>
#include<map>
#include<iostream>
using namespace std;

class CMolecularStructure
{
    public:
        CMolecularStructure( );
        virtual ~CMolecularStructure();
        CAtom* CreateAtom( const string &symbol, const mvector &x );
        const vector< CAtom* >& GetAtoms() const { return atoms; }
        AtomStruct* GetAtomType( const string &as )  { return atom_types[ as ]; }
		void Init( const string &filename );
    protected:
        map< string, AtomStruct* > atom_types;
        vector< CAtom* > atoms;
    private:

};

#endif // CMOLECULARSTRUCTURE_H
