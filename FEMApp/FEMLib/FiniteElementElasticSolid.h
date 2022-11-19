#ifndef FINITEELEMENTELASTICSOLID_H
#define FINITEELEMENTELASTICSOLID_H

#include "FiniteElement.h"

class CFiniteElementElasticSolid : public CFiniteElement
{
public:
    CFiniteElementElasticSolid(
            const vector< FEDomain* > &vdom,
            CMaterial &mat,
            CIntegrationManager &IM,
            CFEFunctorMatrix &d
);

     ~CFiniteElementElasticSolid() { delete D; }

protected:

     CFEFunctorMatrix *D;
     virtual void CreateElement();
};

#endif // FINITEELEMENTELASTICSOLID_H
