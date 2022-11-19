#include "SubspaceIndexer.h"


CSubspaceIndexer::CSubspaceIndexer( unsigned d, unsigned sd, const vector< double > &us ):dim( d ),sdim( sd ),cdim( d - sd ),cv( us ),imap( dim ),cind(0),ind(0)
{

    if ( sd >= d ) throw CMathException( "CSubspaceIndexer::CSubspaceIndexer : Subspace dimmension should be less than space dimmension");
    //if ( us.size() != cdim ) throw CMathException( "CSubspaceIndexer::CSubspaceIndexer :Complementary vector dimmension should be equal to complementarty space dimmension");
    Reset();
    CreateSubPoints();
}

 void CSubspaceIndexer::CreateSubPoints()
 {
     CNCounter counter(cdim,cv.size());
     counter.Reset();
     do {
         mvector x( cdim );
         unsigned k;
         for (k=0; k<cdim; k++)
             x[ k ] = cv[ counter.GetIndex( k ) ];
         cmppoints.push_back( x );
     } while( counter++ );
 }



void CSubspaceIndexer::Reset()
{
    unsigned k, ind = 0;
    for (k=0; k<sdim; k++) imap[ ind++ ] = true;
    for (k=0; k<cdim; k++) imap[ ind++ ] = false;
    ind = cind = 0;
}


bool CSubspaceIndexer::Next()
{
    int k;
    if ( dim == 1 ) return false;
    cind++;
    if ( cind == cmppoints.size() )
    {
        for (k=dim-2; k>=0; k--)
            if ( imap[ k ] == true && imap[ k + 1 ] == false )
            {
                imap[ k ] = false;
                imap[ k + 1 ] = true;
                cind = 0;
                ind++;
                return true;
            }
        return false;
    }
    ind++;
    return true;

}



void CSubspaceIndexer::Map( const mvector &subpoint, vector< mvector > &X  )
{
    unsigned k, l, is = 0,ic = 0;
    mvector x( dim );
    X.clear();
    for (k=0; k<cmppoints.size(); k++)
    {
        is = 0; ic = 0;
        for (l=0; l<dim; l++)
            if ( imap[ l ] ) x[ l ] = subpoint[ is++ ];
            else x[ l ] = cmppoints[ cind ][ ic++ ];
        X.push_back( x );
    }

}


void CSubspaceIndexer::Map( const mvector &subpoint, mvector  &X  )
{
    unsigned k, l, is = 0,ic = 0;
    X.SetDim(dim);
    is = 0; ic = 0;
    for (l=0; l<dim; l++)
      if ( imap[ l ] ) X[ l ] = subpoint[ is++ ];
       else X[ l ] = cmppoints[ cind ][ ic++ ];
}
