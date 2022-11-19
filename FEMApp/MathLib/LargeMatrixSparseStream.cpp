#include "LargeMatrixSparseStream.h"
#include "threads.h"

CLargeMatrixSparseStream::CLargeMatrixSparseStream(bool ss):CLargeMatrix(),data(0),symmetric_storage( ss ), blines_raw(0),elines_raw(0),data_raw(0),cols_raw(0)
{

}

CLargeMatrixSparseStream::CLargeMatrixSparseStream( const CLargeMatrixSparseStream& orig ):CLargeMatrix(orig),lines( orig.lines ), blines( orig.blines ), symmetric_storage( orig.symmetric_storage )
{
    if ( orig.data )
    {
        data = new CMatrixCell[ orig.total_elem_number ];
        memcpy( data, orig.data, orig.total_elem_number * sizeof( CMatrixCell ) );
    }
    if ( orig.blines_raw ) {
        blines_raw = new unsigned[ dim ];
        memcpy( blines_raw, orig.blines_raw, orig.GetDim() * sizeof( unsigned ) );
    }

    if ( orig.elines_raw ) {
        elines_raw = new unsigned[ dim ];
        memcpy( elines_raw, orig.elines_raw, orig.GetDim() * sizeof( unsigned ) );
    }

    if ( orig.data_raw ) {
        data_raw = new double[ total_elem_number ];
        memcpy( data_raw, orig.data_raw, orig.GetTotalElemNumber() * sizeof( double ) );
    }

    if ( orig.cols_raw ) {
        cols_raw = new unsigned[ dim ];
        memcpy( cols_raw, orig.cols_raw, orig.GetTotalElemNumber()* sizeof( unsigned ) );
    }
}

CLargeMatrixSparseStream::~CLargeMatrixSparseStream()
{
    Reset();
}

void CLargeMatrixSparseStream::Copy( CLargeMatrix &K )
{
    CLargeMatrixSparseStream *Ks = dynamic_cast< CLargeMatrixSparseStream* >( &K );
    if ( Ks == 0 ) return;
    Reset();
    total_elem_number = Ks->GetTotalElemNumber();

    data = new CMatrixCell[ total_elem_number ];
    memcpy( data, Ks->GetData(), total_elem_number * sizeof( CMatrixCell ) );

    lines   = Ks->GetLines();
    blines  = Ks->GetBeginLines();

}

void CLargeMatrixSparseStream::Reset()
{
    delete [] data; data=0;
    blines.clear();
    lines.clear();
    dim = 0;
    total_elem_number = 0;
    delete [] elines_raw; elines_raw = 0;
    delete [] blines_raw; blines_raw = 0;

    delete [] data_raw;  data_raw = 0;
    delete [] cols_raw;  cols_raw = 0;
}

void CLargeMatrixSparseStream::SetZero()
{
        unsigned k;
        for (k=0; k<total_elem_number; k++ )
        {
            data[ k ].value = 0.0;
            data_raw[ k ] = 0.0;
        }
}

void CLargeMatrixSparseStream::Sum( double, CLargeMatrix *K )
{
    CLargeMatrixSparseStream *Ks = dynamic_cast< CLargeMatrixSparseStream* >( K );
    if ( Ks == 0 ) return;
    unsigned k;
    for (k=0; k<total_elem_number; k++ )
    {
           data[ k ].value += Ks->GetData()[ k ].value;
           data_raw[ k ]   += Ks->GetData()[ k ].value;
    }
}

void CLargeMatrixSparseStream::MultiplyColumn( double* W, unsigned ndof, double wr )
{

}

void* go_task_stream( void *ptr )
{
    CLargeMatrixSparseStream::CSlice *slice = (CLargeMatrixSparseStream::CSlice*)ptr;
    slice->LM->MultiplyVectorSlice( *slice );
}

void CLargeMatrixSparseStream::MultiplyVector( const mvector &in, mvector &out ) const
{
    const unsigned nthreads=10;
    unsigned k,l,ndiv = dim / nthreads,begin=0;
    vector< CSlice > slices;
    vector< Thread > threads( nthreads - 1 );
    const double *pin = in.GetDataPointer();
    out.SetDim( dim );
    double *pout = out.GetDataPointer();

    for (k=0; k<nthreads-1; k++)
    {
        slices.push_back( CSlice( this, &data[ blines[ begin ] ], pin, pout, nthreads, begin + ndiv  ) );
        begin += ndiv;
    }

    for (k=0; k < nthreads - 1; k++)
        threads[ k ] = createThread( go_task_stream, &slices[ k ] );

    CSlice sle( this, &data[ blines[ begin ] ], pin, pout, nthreads, dim  );

    go_task_stream( &sle );

    for (k=0; k<threads.size(); k++)
        waitForThread( threads[ k ] );

}

void CLargeMatrixSparseStream::MultiplyVectorSlice( CSlice &sl ) const
{
    CMatrixCell *mdata = sl.data;
    const double *pin = sl.pin;
    double *pout = sl.pout;
    unsigned k, l, nthreads = sl.nthreads, en = sl.ten ;
    for( k = mdata->row; k < en; k++ )
        for (l=blines_raw[ k ]; l<blines_raw[ k + 1 ]; l++)
            pout[ k ] += pin[ data[ l ].col ] * data[ l ].value;
}

void CLargeMatrixSparseStream::print()
{
    ofstream fout("CLargeMatrixSparseStream.txt");
    fout.setf( ios::scientific, ios::floatfield );
    fout.setf( ios::left );
    fout.precision( 12 );
    unsigned k,l;
    fout << "diagonal" << endl;
    for( k = 0; k < total_elem_number; k++ )
    {
        if ( data[k].row == data[k].col )
        {
            fout << data[ k ].row << "  ";
            fout << data[ k ].value << endl;
        }
    }
    fout << endl << "Matrix dim :" << dim << " total number of elements :" << total_elem_number <<  " sparsity :" << total_elem_number * 100.0 / double(dim) / double(dim) << "%"<< endl;
    unsigned prow=0;
    for( k = 0; k < total_elem_number; k++ )
    {
            if ( prow != data[k].row )
            {
                fout << endl;
                prow = data[k].row;
            }
            fout << "K(" << data[ k ].row << "," << data[ k ].col << ")=" << data[ k ].value;
    }
}

bool CLargeMatrixSparseStream::Create( unsigned ndof )
{
    dim = ndof;
    lines.resize( ndof );
    blines.resize( ndof );
}

void CLargeMatrixSparseStream::UpdateProfile( unsigned row, unsigned col )
{
    list< unsigned > &line = lines[ row ];
    list< unsigned >::iterator i;
    for (i=line.begin(); i!=line.end(); i++)
        if ( *i == col ) return;
        else if ( *i > col ) { line.insert( i, col ); return; }
    line.push_back( col );
}

void CLargeMatrixSparseStream::UpdateProfile( const valarray<unsigned> &alr, const valarray<unsigned> &alc )
{
    unsigned k,l;
    if ( symmetric_storage )
    {
        for( k = 0; k<alr.size(); k++ )
            for (l=0; l<alc.size(); l++)
                if ( alc[ l ] >= alr[ k ] )
                    UpdateProfile( alr[ k ], alc[ l ] );
    } else {
        for( k = 0; k<alr.size(); k++ )
            for (l=0; l<alc.size(); l++)
                UpdateProfile( alr[ k ], alc[ l ] );
    }
}

void CLargeMatrixSparseStream::EraseDof( unsigned dof )
{
    unsigned k=0;
    list< unsigned >::iterator i;
    for ( k = blines[ dof ]; k < blines[ dof ] + lines[ dof ].size(); k++ )
          if ( data[ k ].col == dof && data[ k ].row == dof  )
          {
              data[ k ].value = 1E100;
              data_raw[ k ] = 1E100;
              return;
          }

}


void CLargeMatrixSparseStream::AllocateMatrix( valarray<unsigned> &alr, valarray<unsigned> &alc, const matrix &m )
{
    unsigned k,l;
    if ( symmetric_storage )
    {
        for (k=0; k<alr.size(); k++)
        {
            list< unsigned > &row = lines[ alr[ k ] ];
            for(l=0; l<alc.size(); l++ )
            {
                if ( alc[l] >= alr[k] )
                {
                    unsigned i = blines[ alr[ k ] ];
                    while( true )
                    {
                        if ( data[ i ].col == alc[ l ] )
                        {
                            data[ i ].value += m( k, l );
                            data_raw[ i ] += m( k, l );
                            break;
                        }
                        i++;
                    }
                }
            }
        }
    } else {
        for (k=0; k<alr.size(); k++)
        {
            list< unsigned > &row = lines[ alr[ k ] ];
            for(l=0; l<alc.size(); l++ )
            {
                unsigned i = blines[ alr[ k ] ];
                while( true )
                {
                    if ( data[ i ].col == alc[ l ] )
                    {
                        data[ i ].value += m( k, l );
                        data_raw[ i ] += m( k, l );
                        break;
                    }
                    i++;
                }
            }
        }
    }
}

void CLargeMatrixSparseStream::FindFactorC( )
{
    factor_c = 0.0;
    unsigned k,l;
    for(k=0; k<dim; k++)
        for ( l = blines[ k ]; l < blines[ k ] + lines[ k ].size(); l++ )
            if ( data[ l ].col == k && data[ l ].value > factor_c) factor_c = data[ l ].value;

    factor_c *= 1.0E+05;
}


bool CLargeMatrixSparseStream::CreateMatrix( )
{
        unsigned k,ind=0;
        total_elem_number = 0;
        elines_raw = new unsigned[ dim ];
        blines_raw = new unsigned[ dim + 1 ];
        for (k=0; k<lines.size(); k++)
        {
            blines[ k ] = total_elem_number;
            blines_raw[ k ] = total_elem_number;
            total_elem_number += lines[ k ].size();
            elines_raw[ k ] = total_elem_number - 1;
        }
        blines_raw[ dim ] = total_elem_number;
        data = new CMatrixCell[ total_elem_number ];
        data_raw = new double[ total_elem_number ];
        cols_raw = new unsigned[ total_elem_number ];
        list< unsigned >::const_iterator ci;
        for( k=0; k<dim; k++)
            for( ci = lines[ k ].begin(); ci != lines[ k ].end(); ci++)
            {
                data[ ind ].row = k;
                data[ ind ].col = *ci;
                cols_raw[ ind ] = *ci;
                data_raw[ ind ] = 0.0;
                data[ ind++ ].value = 0.0;

            }

}


bool CLargeMatrixSparseStream::CreateStructure( CLargeMatrix *K )
{
    Reset();
    CLargeMatrixSparseStream *Ks = dynamic_cast< CLargeMatrixSparseStream* >(K);
    if (!Ks) return false;
    dim = Ks->GetDim();
    total_elem_number = Ks->GetTotalElemNumber();
    lines = Ks->GetLines();
    blines = Ks->GetBeginLines();
}

void CLargeMatrixSparseStream::Save( const char *nm )
{
    unsigned k,l;
    ofstream fout( nm );
    fout << dim << endl;
    fout << total_elem_number << endl;
    for( k = 0; k < dim; k++ )
    {
        list<unsigned>::const_iterator i = lines[ k ].begin();
        for(l=blines[ k ]; l < blines[ k ] + lines[ k ].size(); l++, i++)
            fout << k + 1 << "   " << *i + 1 << "    " << data[ l ].value << endl;
    }
}


void CLargeMatrixSparseStream::Read( const char *nm )
{
    ifstream fin(nm);
    fin.close();
}



