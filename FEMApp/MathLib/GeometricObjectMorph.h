#ifndef GEOMETRICOBJECTMORPH_H
#define GEOMETRICOBJECTMORPH_H
#include "GeometricDomain.h"
#include "GeometricObject.h"
#include "SpaceMorph1D.h"


class CGeometricObjectMorph : public CGeometricObject
{
public:

    CGeometricObjectMorph( const CGeometricObject *go1, const CGeometricObject *go2  );
    virtual ~CGeometricObjectMorph() { }

protected:
    const CGeometricObject *g1, *g2;
};

#endif // GEOMETRICOBJECTMORPH_H
