#ifndef NODALSELECTIONREADER_H
#define NODALSELECTIONREADER_H

#include "../FEMLib/NodeSelector.h"
#include "FEMProject.h"
#include <fstream>


class CNodalSelectionReader
{
public:

    CNodalSelectionReader( CFEMProject &fp, CSpace *sp=0 );
    CNodeSelector& GetSelector() { return selector; }
    void read( istream &inp );

protected:

    CFEMProject &FP;

    CNodeSelector selector;
    CSpace *space;


};


inline istream& operator >> ( istream &i, CNodalSelectionReader &ns ) { ns.read( i ); return i; }

#endif // NODALSELECTIONREADER_H
