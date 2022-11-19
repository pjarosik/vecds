#ifndef SUBSPACEINDEXER_H
#define SUBSPACEINDEXER_H
#include "MathException.h"
#include "NCounter.h"
#include "matrix.h"

class CSubspaceIndexer
{

public:

    CSubspaceIndexer( unsigned d, unsigned sd, const vector< double > &us  );
    void Reset();

    const vector< mvector >& GetCompPoints() const { return cmppoints; }
    unsigned GetNumberOfCompPoints() const { return cmppoints.size(); }
    void Map( const mvector &subpoint, vector< mvector > &X );
    void Map( const mvector &subpoint, mvector  &X  );
    bool Next();
    operator unsigned() const { return ind; }

protected:

    void CreateSubPoints();
    unsigned dim, sdim, cdim, cind, ind;
    vector< double > cv;
    vector< mvector > cmppoints, points;
    vector< bool > imap;

};

#endif // SUBSPACEINDEXER_H
