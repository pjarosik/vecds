#ifndef GEOMETRICDOMAIN_H
#define GEOMETRICDOMAIN_H

#include "Space.h"
#include <vector>
#include "DomainContainer.h"
#include "Clonable.h"

using namespace std;

class CGeometricObject;

class CGeometricDomain
{

public:

    CGeometricDomain( double e = 1.0E-06 ):eps( e ) { }
    ~CGeometricDomain() { clear(); }

    void AddSpace( CSpace *sp, const string &dn = "" ) { SP.AddMember(sp,dn); }
    void AddGeometricObject( CGeometricObject *g, const string &dn = "" ) { GO.AddMember(g,dn); }

    void AddSpaces( const vector<CSpace*> &sp, const string &dn = "" ) { SP.AddMembers(sp,dn); }
    void AddGeometricObjects( const vector<CGeometricObject*> &g, const string &dn = "" ) { GO.AddMembers(g,dn); }

    CGeometricObject* FindApproximatelyEqual( const CGeometricObject *obj ) const;

    const vector< CGeometricObject* >& GetAllGeometricObjects() const { return GO.GetAll(); }
    const DomainContainer< CGeometricObject >& GetGeometricDomain() const { return GO; }

    void ComputeBoundaryStat();
    void EnumerateObjects();
    void PrintConnections();
    unsigned DetermineSpaceDimension() const;

    void clear()
    {
        SP.clear();
        GO.clear();
    }

protected:

    double eps;
    DomainContainer< CGeometricObject > GO;
    DomainContainer< CSpace > SP;

};

#endif // GEOMETRICDOMAIN_H
