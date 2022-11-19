/*
 * File:   CLargeMatrixSparseCR.cpp
 * Author: piotrek
 * 
 * Created on November 17, 2011, 9:55 AM
 */

#include "LargeMatrixSparseCR.h"
#include "threads.h"

CLargeMatrixSparseCR::CLargeMatrixSparseCR(bool ss):CLargeMatrix(),
    data(0), cols_raw(0), blines_raw(0), rdata(0), cdata(0), ird(0), symmetric_storage( ss ),diag_raw(0)
{
}

CLargeMatrixSparseCR::CLargeMatrixSparseCR(const CLargeMatrixSparseCR& orig):CLargeMatrix(orig),
    lines(orig.lines),blines(orig.blines), symmetric_storage( orig.symmetric_storage )
{
    if ( orig.data )
    {
        data = new double[ orig.total_elem_number ];
        memcpy( data, orig.data, orig.total_elem_number * sizeof( double ) );
    }

    if ( orig.blines_raw )
    {
        blines_raw = new unsigned[ orig.GetTotalElemNumber() ];
        memcpy( blines_raw, orig.blines_raw, orig.GetTotalElemNumber() * sizeof( unsigned ) );
    }

    if ( orig.cols_raw )
    {
        cols_raw = new unsigned[ orig.GetTotalElemNumber() ];
        memcpy( cols_raw, orig.cols_raw, orig.GetTotalElemNumber() * sizeof( unsigned ) );
    }

    if ( orig.diag_raw )
    {
        diag_raw = new unsigned[ dim ];
        memcpy( diag_raw, orig.diag_raw, orig.GetTotalElemNumber() * sizeof( unsigned ) );
    }
}

const CLargeMatrixSparseCR& CLargeMatrixSparseCR::operator=( const CLargeMatrixSparseCR &orig )
{
    Reset();
    section = createSection();
    total_elem_number = orig.total_elem_number;
    dim = orig.dim;
    factor_c = orig.factor_c;
    lines = orig.lines;
    blines = orig.blines;
    symmetric_storage = orig.symmetric_storage;

    if ( orig.data )
    {
        data = new double[ orig.total_elem_number ];
        memcpy( data, orig.data, orig.total_elem_number * sizeof( double ) );
    }

    if ( orig.blines_raw )
    {
        blines_raw = new unsigned[ orig.GetTotalElemNumber() ];
        memcpy( blines_raw, orig.blines_raw, orig.GetTotalElemNumber() * sizeof( unsigned ) );
    }

    if ( orig.cols_raw )
    {
        cols_raw = new unsigned[ orig.GetTotalElemNumber() ];
        memcpy( cols_raw, orig.cols_raw, orig.GetTotalElemNumber() * sizeof( unsigned ) );
    }

    if ( orig.diag_raw )
    {
        diag_raw = new unsigned[ dim ];
        memcpy( diag_raw, orig.diag_raw, orig.GetDim() * sizeof( unsigned ) );
    }
    return orig;
}

CLargeMatrixSparseCR::~CLargeMatrixSparseCR() 
{
    Reset();
}

void CLargeMatrixSparseCR::Copy( CLargeMatrix &K )
{
    if ( !CreateStructure( &K ) ) return;
    CLargeMatrixSparseCR *Ks = dynamic_cast< CLargeMatrixSparseCR* >( &K );
    total_elem_number = Ks->GetTotalElemNumber();

    if ( Ks->GetData() )
    {
        data = new double[ total_elem_number ];
        memcpy( data, Ks->GetData(), total_elem_number * sizeof( double ) );
    }

    if ( Ks->GetColsRaw() )
    {
        cols_raw = new unsigned[ total_elem_number ];
        memcpy( cols_raw, Ks->GetColsRaw(), total_elem_number * sizeof( unsigned ) );
    }

    if ( Ks->GetBeginLinesRaw() )
    {
        blines_raw = new unsigned[ total_elem_number ];
        memcpy( blines_raw, Ks->GetBeginLinesRaw(), total_elem_number * sizeof( unsigned ) );
    }

    cdata = new int[ total_elem_number ];
    memcpy( cdata, Ks->cdata, total_elem_number * sizeof( double ) );

    ird = Ks->ird;

    rdata = new int[ ird ];
    memcpy( rdata, Ks->rdata, ird * sizeof( double ) );
}
	
void CLargeMatrixSparseCR::Reset()
{
    delete [] data; data=0;
    delete [] rdata; rdata=0;
    delete [] cdata; cdata=0;
    delete [] blines_raw; blines_raw = 0;
    delete [] cols_raw; cols_raw = 0;
    delete [] diag_raw; diag_raw = 0;
    ird = 0;
    lines.clear();
    blines.clear();
    dim = 0;
    total_elem_number = 0;
}

void CLargeMatrixSparseCR::SetZero()
{
    unsigned k;
    for (k=0; k<total_elem_number; k++ )
        data[ k ] = 0.0;
}

void CLargeMatrixSparseCR::Sum( double, CLargeMatrix *K )
{
    CLargeMatrixSparseCR *Ks = dynamic_cast< CLargeMatrixSparseCR* >( K );
    unsigned k;  
    for (k=0; k<total_elem_number; k++ )
           data[ k ] += Ks->GetData()[ k ];
}

void CLargeMatrixSparseCR::MultiplyColumn( double* W, unsigned ndof, double wr )
{
    
}

void* go_task( void *ptr )
{
    CLargeMatrixSparseCR::CSlice *slice = (CLargeMatrixSparseCR::CSlice*)ptr;
    slice->LM.MultiplyVectorSlice( *slice );
}

void CLargeMatrixSparseCR::MultiplyVector( const mvector &in, mvector &out ) const
{
    out.SetDim( dim );
    const unsigned nthreads=10;
    unsigned one_thread = dim / nthreads, begin = 0, end = one_thread;
    unsigned k,l,ind;
    vector< CSlice* > slices;
    vector< Thread > threads( nthreads - 1 );
    
    for (k=0; k < nthreads - 1; k++)
    {
        CSlice *slice = new CSlice( *this, in, out, begin, end );
        slices.push_back(slice) ;
        threads[ k ] = createThread( go_task, slices[ k ] );
        begin += one_thread;
        end += one_thread;
    }
    
    CSlice sle( *this, in, out, begin, dim );
    go_task( &sle );
    
    for (k=0; k<threads.size(); k++)
        waitForThread( threads[ k ] );
    
    for (k=0; k < slices.size(); k++) delete slices[k];
    
}

void CLargeMatrixSparseCR::MultiplyVectorSlice( CSlice &sl ) const
{
    const mvector &in = sl.in;
    mvector &out = sl.out;
    unsigned k,l,ind,bg = sl.begin , en = sl.end;
    for( k = bg; k < en; k++ )
    {
        unsigned bl = blines_raw[ k ], el = blines_raw[ k + 1 ]; //, *col_ptr = &cols_raw[ bl ];
        for( l = bl;  l < el;  l++)
            out[ k ] += data[ l ] * in[ cols_raw[ l ] ];
    }
}

void CLargeMatrixSparseCR::print()
{
    ofstream fout("CLargeMatrixSparseCR.txt");
    fout.setf( ios::scientific, ios::floatfield );
    fout.setf( ios::left );
    fout.precision( 12 );
    unsigned k,l;
    fout << "diagonal" << endl;
    for( k = 0; k < dim; k++ )
    {
        list<unsigned>::iterator i = find( lines[k].begin(),lines[k].end(),k);
        fout << k << "  ";
        if ( i != lines[k].end() )
            fout << data[ blines[k] + distance( lines[k].begin() ,i ) ] << endl;
        else fout << "Not found" << endl;
    }
    fout << endl << "Matrix dim :" << dim << " total number of elements :" << total_elem_number <<  " sparsity :" << total_elem_number * 100.0 / double(dim) / double(dim) << "%"<< endl;
    for( k = 0; k < dim; k++ )
    {
        fout << endl << "row :" << k << " elem num :" << lines[ k ].size() << " begin num :" << *lines[ k ].begin() << " end  num :" << lines[ k ].back()  << " elems: ";
        list<unsigned>::const_iterator i = lines[ k ].begin();
        for(l=0; l < lines[ k ].size(); l++, i++)
            fout << '(' << *i << ')' << data[ blines[ k ] + l ] << ", ";
    }
}

bool CLargeMatrixSparseCR::Create( unsigned ndof )
{
    dim = ndof;
    lines.resize( dim );
    blines.resize( dim );
}

void CLargeMatrixSparseCR::UpdateProfile( const valarray<unsigned> &alr, const valarray<unsigned> &alc )
{
    unsigned k,l;
    if ( symmetric_storage )
    {
        for( k = 0; k<alr.size(); k++ )
        {
            list< unsigned > &rl = lines[ alr[ k ] ];
            for (l=0; l<alc.size(); l++)
            {
                if ( alc[l] >= alr[k] )
                {
                    list< unsigned >::iterator i = rl.begin();
                    while( i != rl.end()  )
                    {
                        if ( *i == alc[ l ] ) break;
                        if ( *i > alc[ l ] ) { rl.insert( i, alc[ l ] ); break; }
                        i++;
                    }
                    if ( i == rl.end() ) rl.push_back( alc[ l ] );
                }
            }
        }
    } else {
        for( k = 0; k<alr.size(); k++ )
        {
            list< unsigned > &rl = lines[ alr[ k ] ];
            for (l=0; l<alc.size(); l++)
            {
                    list< unsigned >::iterator i = rl.begin();
                    while( i != rl.end()  )
                    {
                        if ( *i == alc[ l ] ) break;
                        if ( *i > alc[ l ] ) { rl.insert( i, alc[ l ] ); break; }
                        i++;
                    }
                    if ( i == rl.end() ) rl.push_back( alc[ l ] );
            }
        }
    }
}

void CLargeMatrixSparseCR::EraseDof( unsigned dof )
{
    unsigned k=0;
    list< unsigned >::iterator i;
    for (i=lines[dof].begin(); i!=lines[dof].end(); k++, i++ )
          if ( *i == dof )  data[ blines[ dof ] + k ] = 1E100;
}

	
void CLargeMatrixSparseCR::AllocateMatrix( valarray<unsigned> &alr, valarray<unsigned> &alc, const matrix &m )
{
    unsigned k,l;
    if ( symmetric_storage )
    {
        for (k=0; k<alr.size(); k++)
        {
            list<unsigned> &row = lines[ alr[ k ] ];
            for(l=0; l<alc.size(); l++ )
            {
                if ( alc[l] >= alr[k] )
                {
                    unsigned pos = 0;;
                    list<unsigned>::iterator i = row.begin();
                    while( i != row.end() )
                    {
                        if ( *i == alc[ l ] ) break;
                        pos++;
                        i++;
                    }
                    data[ blines[ alr[ k ] ] + pos ] += m( k, l );
                }
            }
        }
    } else {
        for (k=0; k<alr.size(); k++)
        {
            list<unsigned> &row = lines[ alr[ k ] ];
            for(l=0; l<alc.size(); l++ )
            {
                unsigned pos = 0;;
                list<unsigned>::iterator i = row.begin();
                while( i != row.end() )
                {
                    if ( *i == alc[ l ] ) break;
                    pos++;
                    i++;
                }
                data[ blines[ alr[ k ] ] + pos ] += m( k, l );
            }
        }
    }
}
	
void CLargeMatrixSparseCR::FindFactorC( )
{
    factor_c = 0.0;
    unsigned k;
    for(k=0; k<dim; k++)
        if ( data[ *lines[k].begin() ] > factor_c) factor_c = data[ *lines[k].begin() ];

    factor_c *= 1.0E+05;
}

	
bool CLargeMatrixSparseCR::CreateMatrix( )
{
        unsigned k, l, ind = 0;
        total_elem_number = 0;
        blines_raw = new unsigned[ dim + 1 ];
        for (k=0; k<blines.size(); k++)
        {
            blines_raw[ k ] = blines[ k ] = total_elem_number;
            total_elem_number += lines[ k ].size();
        }
        blines_raw[ dim ] = total_elem_number;
        data = new double[ total_elem_number ];
        cols_raw = new unsigned[ total_elem_number ];
        diag_raw = new unsigned[ dim ];
        for (k=0; k<total_elem_number; k++)
            data[ k ] = 0.0;

        for (k=0; k<lines.size(); k++)
        {
            list< unsigned >::iterator i = find( lines[k].begin(),lines[k].end(),k);
            diag_raw[ k ] = blines[ k ] + distance( lines[ k ].begin() ,i );
            for ( i=lines[ k ].begin(); i!=lines[k].end(); i++ )
                cols_raw[ ind++ ] = *i;
        }

}

	
bool CLargeMatrixSparseCR::CreateStructure( CLargeMatrix *K )
{
    Reset();
    CLargeMatrixSparseCR *Ks = dynamic_cast< CLargeMatrixSparseCR* >(K);
    if (!Ks) return false;
    dim = Ks->GetDim();
    lines   = Ks->GetLinesStruct();
    blines  = Ks->GetBeginLinesStruct();
}

void CLargeMatrixSparseCR::Save( const char *nm )
{
    unsigned k,l;
    ofstream fout( nm );
    fout << dim << endl;
    fout << total_elem_number << endl;
    for( k = 0; k < dim; k++ )
    {
        list<unsigned>::const_iterator i = lines[ k ].begin();
        for(l=0; l < lines[ k ].size(); l++, i++)
            fout << k + 1 << "   " << *i + 1 << "    " << data[ blines[ k ] + l ] << endl;
    }
}


void CLargeMatrixSparseCR::Read( const char *nm )
{
    ifstream fin(nm);
    fin.close();
}


void CLargeMatrixSparseCR::CreateMatrixCRData1( bool IsZeroBasedInd )
{
    delete [] rdata; rdata=0;
    delete [] cdata; cdata=0;
    ird = total_elem_number;
    rdata = new int[ total_elem_number ];
    cdata = new int[ total_elem_number ];
    int rind = 0,k ,l;

    for( k = 0; k < dim; k++ )
    {
        list<unsigned>::const_iterator i = lines[ k ].begin();
        for(l=0; l < lines[ k ].size(); l++, i++)
        {
            rdata[ rind ] = IsZeroBasedInd ? k : k + 1;
            cdata[ rind++ ] = IsZeroBasedInd ? blines[ k ] + l : blines[ k ] + l + 1;
        }
    }
}

void CLargeMatrixSparseCR::CreateMatrixCRData2( bool IsZeroBasedInd )
{
    delete [] rdata; rdata=0;
    delete [] cdata; cdata=0;
    ird = 0;

    ird = dim + 1;
    rdata = new int[ dim + 1 ];
    cdata = new int[ total_elem_number ];
    int rind = 0,k,l;

    for( k = 0; k < dim; k++ )
    {
        list<unsigned>::const_iterator i = lines[ k ].begin();
        rdata[ k ] = IsZeroBasedInd ? blines[ k ] : blines[ k ] + 1;
        for(i=lines[ k ].begin(); i != lines[ k ].end(); i++)
            cdata[ rind++ ] = IsZeroBasedInd ? *i  : (*i) + 1;
    }
    rdata[ k ] = IsZeroBasedInd ? total_elem_number : total_elem_number + 1;
}

void CLargeMatrixSparseCR::_print()
{
    ofstream fout( "CLargeMatrixSparseCR-raw.txt" );
    unsigned k,l=rdata[0];

    fout << "dim:" << dim << endl;
    fout << "rows:" << endl;
    for (k=0; k<=dim; k++)
        fout << rdata[ k ] << ",";
    fout << endl;

    fout << "cols:" << endl;
    for (k=0; k<dim; k++)
    {
        for(l=rdata[k]; l<rdata[k+1]; l++)
            fout << cdata[ l - 1 ] << ",";
        fout << endl;
    }

    fout << "diagonal" << endl;
    for (k=0; k<dim; k++)
    {
        for(l=rdata[k]; l<rdata[k+1]; l++)
            if ( cdata[l-1] - 1 == k ) fout << k << "\t : " << data[  l  - 1 ] << endl;
    }

    for (k=0; k<dim; k++)
    {
        for(l=rdata[k]; l<rdata[k+1]; l++)
            fout << "A(" << k << "," << cdata[ l-1 ] - 1 << ")=" << data[ l-1 ] << "\t";
        fout << endl;
    }
    fout << "total element number = " << rdata[k] << endl;
}

void CLargeMatrixSparseCR::CheckSizesForIncompleteFactorisation()
{
    /*
    for r:= 1 step 1 until n-1 do

    d := 1/ arr
    for i := (r+1) step 1 until n do
    if (i,r)\inS then

        e := dai,r;
        ai,r := e ;
        for j := (r+1) step 1 until n do

            if ( (i,j)\inS ) and ( (r,j)\inS ) then
            ai,j := ai,j - e ar,j
            end if

        end (j-loop)

    end if
    end (i-loop)
    end (r-loop)
*/


}
