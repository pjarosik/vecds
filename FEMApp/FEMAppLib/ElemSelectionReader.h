#ifndef ELEMSELECTIONREADER_H
#define ELEMSELECTIONREADER_H

#include "../FEMLib/ElemInstanceSelector.h"
#include "FEMProject.h"
#include <fstream>

class CElemSelectionReader
{
public:
    CElemSelectionReader(  CFEMProject &fp ):selector( fp.GetElemsDomain() ), elements( fp.GetElemsDomain() ),FP( fp ) {  }
    CElemInstanceSelector& GetSelector() { return selector; }
    void read( istream &inp );

protected:

    CFEMProject &FP;
    const DomainContainer< CFiniteElementInstance > &elements;
    CElemInstanceSelector selector;

};


inline istream& operator>>( istream &i, CElemSelectionReader &ns ) { ns.read( i ); return i; }

#endif // ELEMSELECTIONREADER_H
