#ifndef MESHGENERATORRECTANGULARTRIG_H
#define MESHGENERATORRECTANGULARTRIG_H
#include "MeshGeneratorRectangular2D.h"

enum TMPattern
{
    TM_DOUBLE,
    TM_DOUBLE2,
    TM_QUAD
};

class CMeshGeneratorRectangularTrig : public CMeshGeneratorRectangular2D
{

public:
    CMeshGeneratorRectangularTrig(const CSpaceShape& sf, unsigned x, unsigned y,TMPattern pt);
     virtual bool Generate();
protected:
    TMPattern pattern;

};

#endif // MESHGENERATORRECTANGULARTRIG_H
