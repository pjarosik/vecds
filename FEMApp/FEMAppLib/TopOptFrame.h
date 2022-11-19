#ifndef TOPOPTFRAME_H
#define TOPOPTFRAME_H
#include "../MathLib/matrix.h"

class CTopOptFrame
{
public:
    CTopOptFrame( unsigned gr );
    bool Read( ifstream &fin );
    double GetValue(unsigned k) const { return x[ k ]; }
    double GetC() const { return C; }
    double GetV() const { return V; }
    double GetVrel() const { return Vrel; }
    double GetPenalty() const { return penalty; }
protected:
unsigned iteration;
double penalty,C,V,Vrel;
unsigned active, pasive, ground_size;
mvector x;
};

#endif // TOPOPTFRAME_H

