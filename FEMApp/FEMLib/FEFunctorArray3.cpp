#include "FEFunctorArray3.h"
#include <iomanip>

CFEFunctorArray3::CFEFunctorArray3( unsigned d1, unsigned d2, unsigned d3, const string &nm ):CFEFunctor< CFiniteElementInstance >(nm), data(d1, vector< vector< double > >(d2, vector<double>( d3, 0.0 ) ) )
{

}


ostream& CFEFunctorArray3::PrintFunctor( ostream &out ) const
{
    out.setf( ios::scientific );
    out.precision( 10 );
    unsigned k, l, m;

    for (m=0; m<data.size(); m++)
        for (l=0; l<data[m].size(); l++)
        {
            for (k=0; k<data[m][l].size(); k++)
                out << setw( 20 ) << data[ m ][ l ][ k ];
            out << endl;
        }
        return out;
}

void CFEFunctorArray3::DebugPrint( ) const
{
    ofstream fout( "array_file_dump.txt", ios::app );
    PrintFunctor( fout );
}
