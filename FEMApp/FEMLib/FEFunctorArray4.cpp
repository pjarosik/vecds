#include "FEFunctorArray4.h"
#include <iomanip>

CFEFunctorArray4::CFEFunctorArray4( unsigned d1, unsigned d2, unsigned d3, unsigned d4, const string &nm ):CFEFunctor< CFiniteElementInstance >(nm),
        data(d1,
             vector< vector< vector< double > > >(d2,
                                        vector< vector< double > >( d3,
                                                                    vector< double >( d4, 0.0 )  ) ) )
{

}


ostream& CFEFunctorArray4::PrintFunctor( ostream &out ) const
{
    out.setf( ios::scientific );
    out.precision( 10 );
    unsigned k, l, m, n;

    for (n=0; n<data.size();n++)
        for (m=0; m<data.size(); m++)
            for (l=0; l<data[m].size(); l++)
            {
                for (k=0; k<data[m][l].size(); k++)
                    out << setw( 20 ) << "A(" << n << "," << m << "," << l << "," << k << ")=" << data[ n ][ m ][ l ][ k ];
                out << endl;
            }
        return out;
}

void CFEFunctorArray4::DebugPrint( ) const
{
    ofstream fout( "array_file_dump.txt", ios::app );
    PrintFunctor( fout );
}
