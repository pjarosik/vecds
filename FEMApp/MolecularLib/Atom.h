#ifndef CATOM_H
#define CATOM_H

#include "../MathLib/matrix.h"
#include <string>
using namespace std;

class AtomStruct{
public:
    string name, symbol;
    unsigned an ;
    double m1, m2, d1, d2, d3;

    AtomStruct( ):name("unknown"),
                    symbol("un"), an(0), m1(0.0), m2(0.0), d1(0.0), d2(0.0), d3(0.0) { }

    AtomStruct( const string &sm, const string &nm, unsigned a, double a1, double a2, double b1, double b2,double b3):name(nm),
                    symbol(sm), an(a), m1(a1), m2(a2), d1(b1), d2(b2), d3(b3) { }


};

class CAtom
{
    public:
        CAtom( const AtomStruct& as, const mvector &x );
        virtual ~CAtom();
        const mvector& GetX() const { return x; }
        const AtomStruct& GetAtomStruct() const { return as; }
    protected:
        const AtomStruct &as;
        mvector x;
    private:
};

#endif // CATOM_H
