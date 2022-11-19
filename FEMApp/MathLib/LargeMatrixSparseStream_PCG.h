#ifndef LARGEMATRIXSPARSESTREAM_PCG_H
#define LARGEMATRIXSPARSESTREAM_PCG_H

#include "LargeMatrixSparseStream.h"

class CLargeMatrixSparseStream_PCG : public CLargeMatrixSparseStream
{
       public:
            CLargeMatrixSparseStream_PCG();
            CLargeMatrixSparseStream_PCG( const CLargeMatrixSparseStream_PCG& orig );
            virtual ~CLargeMatrixSparseStream_PCG();

            virtual CLargeMatrixSparseStream_PCG* Clone() const { return new CLargeMatrixSparseStream_PCG( *this ); }
            virtual bool Solve( const mvector &b, mvector &x );
};

#endif // LARGEMATRIXSPARSESTREAM_PCG_H
