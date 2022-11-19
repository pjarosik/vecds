#pragma once

#include "../MathLib/threads.h"
#include "../MathLib/Progress.h"
#include <vector>

using namespace std;

template< class Iterator, class ParameterType, class Functor >
class CTask;

template < class Iterator, class ParameterType, class Functor >
class CTasker;

template < class Iterator, class ParameterType, class Functor >
class CTasker
{
public:

    CTasker( Iterator &begin, Iterator &end, CProgress *p = 0 );
    ~CTasker( void );
    ParameterType* GetTaskData();

//    void SetTasks( const CTask<  Iterator,  ParameterType,  Functor > &ts, vector< Functor* > &execs );

	void Run();
    void RunJob( const CTask< Iterator,  ParameterType,  Functor > &ts, Functor *fn, unsigned nt );

protected:

     CProgress *prg;
     unsigned nthreads,counter;
	 Section threads_locker;
     vector< CTask<  Iterator,  ParameterType,  Functor >* > tasks;
     Iterator begin, end, iter;

};

template < class Iterator, class ParameterType, class Functor >
CTasker<  Iterator,  ParameterType,  Functor >::~CTasker( void )
{	
	unsigned k;
	for (k=0; k<tasks.size(); k++)
		delete tasks[ k ];
    freeMutex( threads_locker );
}

template < class Iterator, class ParameterType, class Functor >
CTasker<  Iterator,  ParameterType,  Functor >::CTasker( Iterator &b, Iterator &e, CProgress *p):prg(p),begin(b), end(e)
{
    threads_locker = createSection();
}

template< class Iterator, class ParameterType, class Functor >
void CTasker<  Iterator,  ParameterType,  Functor >::RunJob( const CTask< Iterator,  ParameterType,  Functor > &ts, Functor *functor, unsigned nt )
{
    nthreads = nt;
    unsigned k;
    cout << "multithread job started, nthreads :" << nthreads << endl;
    for (k=0; k<nthreads; k++)
    {
        tasks.push_back( ts.Clone() );
        stringstream ss;
        ss << "thread_" << k;
        tasks.back()->SetName( ss.str( ) );
        tasks.back()->SetTasker( this, functor->Clone() );
    }

    iter = begin;
    counter=0;
    for (k=1; k<tasks.size(); k++)
        tasks[ k ]->RunThread();

    tasks[ 0 ]->Run();

    for (k=1; k<tasks.size(); k++)
        tasks[ k ]->WaitForEnd();

}

/*
template < class Iterator, class ParameterType, class Functor >
void CTasker<  Iterator,  ParameterType,  Functor >::SetTasks( const CTask<  Iterator,  ParameterType,  Functor > &ts, vector< Functor* > &execs, unsigned nt )
{
	nthreads = nt;
	unsigned k;
	for (k=0; k<nthreads; k++)
	{
		tasks.push_back( ts.Clone() );
		stringstream ss;
		ss << "thread_" << k;
		tasks.back()->SetName( ss.str() );
		tasks.back()->SetTasker( this,execs[ k ] );
	}

}
*/

template< class Iterator, class ParameterType, class Functor >
void CTasker<  Iterator,  ParameterType,  Functor >::Run()
{
	unsigned k;
    iter = begin;
    for (k=0; k<tasks.size()-1; k++)
		tasks[ k ]->RunThread();		
		
	tasks.back()->Run();

	for (k=0; k<tasks.size()-1; k++)
		tasks[ k ]->WaitForEnd();

}

template < class Iterator, class ParameterType, class Functor >
ParameterType* CTasker<  Iterator,  ParameterType,  Functor >::GetTaskData( )
{
    ParameterType* resp = 0;
    enterSection( threads_locker );
        if ( iter != end )
		{
            resp = *iter;
			iter++;
		}
    if ( prg ) prg->Increment( 1 );
	leaveSection( threads_locker );
	return resp;
}
