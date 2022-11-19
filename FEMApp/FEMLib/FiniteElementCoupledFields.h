#ifndef FINITEELEMENTCOUPLEDFIELDS_H
#define FINITEELEMENTCOUPLEDFIELDS_H

#include "FiniteElement.h"

class CFiniteElementCoupledFields : public CFiniteElement
{
public:
    CFiniteElementCoupledFields(
                                    vector< FEDomain* > vdom,
                                    CMaterial &mat,
                                    CIntegrationManager &IM
            );

protected:
     virtual void CreateElement();
};

#endif // FINITEELEMENTCOUPLEDFIELDS_H
