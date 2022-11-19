#ifndef LARGEMATRIXSPARSEINCOMPLETELU_H
#define LARGEMATRIXSPARSEINCOMPLETELU_H

#include "LargeMatrixSparseCR.h"

class CLargeMatrixSparseIncompleteLU : public CLargeMatrixSparseCR
{

public:

    CLargeMatrixSparseIncompleteLU();
    CLargeMatrixSparseIncompleteLU(const CLargeMatrixSparseIncompleteLU &K ):CLargeMatrixSparseCR(K) { }
    virtual ~CLargeMatrixSparseIncompleteLU() { }

    virtual bool Decompose(CProgress *pr = 0);
    virtual CLargeMatrixSparseIncompleteLU* Clone() const { return new CLargeMatrixSparseIncompleteLU(*this); }
    virtual bool Solve( const mvector &b, mvector &x );

};

#endif // LARGEMATRIXSPARSEINCOMPLETELU_H
