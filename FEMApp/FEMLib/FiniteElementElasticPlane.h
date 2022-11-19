#ifndef FINITEELEMENTELASTICPLANE_H
#define FINITEELEMENTELASTICPLANE_H

#include "FiniteElement.h"

class CFiniteElementElasticPlane : public CFiniteElement
{
public:
    CFiniteElementElasticPlane(
                                    vector< FEDomain* > vdom,
                                    CMaterial &mat,
                                    CIntegrationManager &IM,
                                    CFEFunctorMatrix &d
            );

    ~CFiniteElementElasticPlane() { delete D; }

protected:
    CFEFunctorMatrix *D;
    virtual void CreateElement();
};

#endif // FINITEELEMENTELASTICPLANE_H
