#ifndef FINITEELEMENTHYPERELASTIC_H
#define FINITEELEMENTHYPERELASTIC_H
#include "FiniteElement.h"

class CFiniteElementHyperelastic : public CFiniteElement
{
public:
    CFiniteElementHyperelastic(
            vector< FEDomain* > vdom,
            CMaterial &mat,
            CIntegrationManager &IM,
            CFEFunctorMatrix &d
            );
    ~CFiniteElementHyperelastic() { delete D; }
protected:
    CFEFunctorMatrix *D;
    virtual void CreateElement();
};

#endif // FINITEELEMENTHYPERELASTIC_H
