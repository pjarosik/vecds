
#include "Material.h"
#include "FEException.h"

CMaterial::CMaterial(const string &tp, map< string, double > &cn):type(tp), constants(cn)
{

}

CMaterial::~CMaterial(void)
{

}

