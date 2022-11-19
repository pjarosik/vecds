#ifndef FINITEELEMENTTRUSS3D_H
#define FINITEELEMENTTRUSS3D_H

#include "FiniteElement.h"

class CFiniteElementTruss3D : public CFiniteElement
{
public:
    CFiniteElementTruss3D(
            vector< FEDomain* > vdom,
            CMaterial &mat,
            CIntegrationManager &IM
);

protected:
     virtual void CreateElement();
};

#endif // FINITEELEMENTTRUSS3D_H
