#pragma once
#include <valarray>
#include "../MathLib/matrix.h"
#include "../MathLib/threads.h"

using namespace std;
template< class M, class T, class F >
class MatrixAggregator
{
public:
    MatrixAggregator( M &mtr, Section *sc=0 ):Matrix( mtr ),section( sc ),Fn( 0 ) { }
    MatrixAggregator( M &mtr, F *fn, Section *sc=0 ):Matrix(mtr),section( sc ),Fn( fn->CloneTree() ) { }
    MatrixAggregator( const MatrixAggregator &mt):Fn( dynamic_cast<F*>( mt.Fn->CloneTree() ) ),section( mt.section ),Matrix( mt.Matrix ) { }
    MatrixAggregator* Clone() { return new MatrixAggregator( *this ); }
    ~MatrixAggregator() { DeleteTree( Fn ); }
    void operator() ( T &elem );
    void Compute( T &elem ) { this->operator ()( elem ); }
    void SetAggregatedFunctor( F &f ) { Fn = dynamic_cast<F*>( f.CloneTree() ) ; }

protected:

    F *Fn;
    Section *section;
	M &Matrix;
	valarray< unsigned > alc, alr;
};

template<  class M, class T, class F >
void MatrixAggregator<M,T,F>::operator()( T &elem )
{
    Fn->InvalidateStructure();
    (*Fn)( &elem );
    Fn->GetAllocationVectors( alr, alc );

    enterSection( *section );
        Matrix.AllocateMatrix( alr, alc, *Fn );
    leaveSection( *section );
}


template< class V, class T, class F >
class VectorAggregatorFunctor
{
public:
    void operator() ( T &elem, F &functor );
    VectorAggregatorFunctor( V &vtr ):Vector( vtr ) { }
protected:
    V &Vector;
    valarray< unsigned > al;
};


template<  class M, class T, class F >
void VectorAggregatorFunctor<M,T,F>::operator()( T &elem, F &functor )
{
    functor.InvalidateStructure();
    functor( &elem );
    functor.GetAllocationVector( al );
    unsigned k;
    for ( k = 0;  k < al.size(); k++ )
        Vector[ al[ k ] ] = functor[ k ];
}


