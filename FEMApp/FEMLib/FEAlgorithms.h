#pragma once

#include "FEFunctorMatrix.h"
#include "Task.h"
#include "Tasker.h"


template < class C,typename e >
e min( const C &c )
{
    e me = c[0];
    typename C::const_iterator ci;
    for(ci=c.begin(); ci!=c.end(); ci++)
        if ( *ci < me ) me=*ci;
    return me;
}

template < class C,typename e >
e max( const C &c )
{
    e me = c[0];
    typename C::const_iterator ci;
    for(ci=c.begin(); ci!=c.end(); ci++)
        if ( *ci > me ) me=*ci;
    return me;
}

template < class T >
class Delete
{
	Delete ( T *t ) { }
	void operator()( T *t ) { delete t; }
};

template< class FEIterator, class Functor, class progress >
void for_eachFE( FEIterator first, FEIterator last, Functor &fn, progress *prg = 0  )
{
        FEIterator i;
        unsigned k = 0;
        for ( i = first; i != last; i++ )
        {
            fn( **i );
			if ( prg ) prg->NewValue( k++ );
        }
}

template< class Iterator, class Functor, class progress >
void for_eachFE( Iterator first, Iterator last, Functor &fn, unsigned nthreads, progress *prg = 0  )
{
    CTasker < Iterator, CFiniteElementInstance, Functor > tasker( first, last, prg );
    CTask< Iterator, CFiniteElementInstance, Functor > task;
    tasker.RunJob( task, &fn, nthreads );
}

template< class FEIterator, class Functor, class progress >
void for_eachFE_in_map( FEIterator first, FEIterator last, Functor &ft, unsigned nthreads = 1, progress *prg = 0 )
{
	FEIterator i;
	if ( prg )
	{
		unsigned nmax = 0;
		for ( i = first; i!= last; i++ )
		{
			vector< CFiniteElementInstance* > &vFE = i->second;
			vector< CFiniteElementInstance* >::iterator j;
			for ( j = vFE.begin(); j != vFE.end(); j++ )
				nmax++;
		}
		prg->SetRange( nmax + 1 );
	}

	for (i = first; i != last; i++ )
	{
        vector< CFiniteElementInstance* > &vFE = i->second;
		CFEFunctorMatrix &mf = *i->first;
        if ( nthreads == 1 ) for_eachFE( vFE.begin(), vFE.end(), ft, prg );
        else for_eachFE( vFE.begin(), vFE.end(), ft, nthreads, prg );
	}
}


template< class FEIterator, class Aggregator, class progress >
void aggregate( FEIterator first, FEIterator last, Aggregator &ag, unsigned nthreads = 1, progress *prg = 0 )
{
    FEIterator i;
    if ( prg )
    {
        unsigned nmax = 0;
        for ( i = first; i!= last; i++ )
        {
            vector< CFiniteElementInstance* > &vFE = i->second;
            vector< CFiniteElementInstance* >::iterator j;
            for ( j = vFE.begin(); j != vFE.end(); j++ )
                nmax++;
        }
        prg->SetRange( nmax + 1 );
    }

    for (i = first; i != last; i++ )
    {
        vector< CFiniteElementInstance* > &vFE = i->second;
        CFEFunctorMatrix &mf = *i->first;
        ag.SetAggregatedFunctor( mf );
        for_eachFE( vFE.begin(), vFE.end(), ag, nthreads,  prg );
    }
}
