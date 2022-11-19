#pragma once
#include "../MathLib/threads.h"
#include <sstream>
#include <iostream>

using namespace std;

template< class Iterator, class ParameterType, class Functor >
class CTask;

template < class Iterator, class ParameterType, class Functor >
class CTasker;

template< class Iterator, class ParameterType, class Functor >
void* go_task( void *ptr )
{
	if ( ptr )
	{
        CTask<  Iterator,  ParameterType,  Functor > *task = (CTask<  Iterator,  ParameterType,  Functor >*) ptr;
		task->Run();
                return 0;
	}
        return 0;
}

template< class Iterator, class ParameterType, class Functor >
class CTask
{
public:
	CTask( );
    CTask( const CTask &task ):tasker(task.tasker),functor(0) {  }
    virtual ~CTask(void) {  delete functor; }
	virtual CTask* Clone() const { return new CTask( *this ); }
    void SetTasker( CTasker<  Iterator,  ParameterType,  Functor > *ts, Functor *fn ) { tasker = ts; functor = fn; }

	void RunThread();
	void WaitForEnd();

	void SetName( const string &nm ) { name = nm; }
	void Run();

    CTasker<  Iterator,  ParameterType,  Functor > *tasker;
    Functor *functor;

protected:

	Thread thread;
	string name;

};


template< class Iterator, class ParameterType, class Functor >
CTask<  Iterator,  ParameterType,  Functor >::CTask(  ):tasker( 0 ), thread( 0 ),functor( 0 )
{
	
}


template< class Iterator, class ParameterType, class Functor >
void CTask<  Iterator,  ParameterType,  Functor >::RunThread()
{
    thread = createThread( go_task<  Iterator,  ParameterType,  Functor >,  this );
}

template< class Iterator, class ParameterType, class Functor >
void CTask<  Iterator,  ParameterType,  Functor >::WaitForEnd()
{

	 waitForThread( thread );
}

template< class Iterator, class ParameterType, class Functor >
void CTask<  Iterator,  ParameterType,  Functor >::Run()
{
    ParameterType *data = 0;
    while( data = tasker->GetTaskData( ) )
        (*functor)( *data );
}
