// Jan Bielecki - Visual 6.0 Threads
// Copyright 1999

#if !defined(_threads)
#define _threads
/*
Visual C++: Wymagane ustawienia dla bibliotek statycznych

Project / Settings // Link

1. Category: General

   Object/library modules:
       Zawartoœæ tej klatki czasami znika. 
       Kompilator informuje "delete already defined".
       Klatka powinna wyszczególniaæ
           kernel32.lib user32.lib gdi32.lib winspool.lib 
           comdlg32.lib advapi32.lib shell32.lib ole32.lib 
           oleaut32.lib uuid.lib odbc32.lib odbccp32.lib 
       Ponadto nale¿y do niej dodaæ
           libcmtd.lib   // w trybie Debug
           libcmt.lib    // w trybie Release

2. Category: Input

   Ignore libraries: 
           libcd.lib     // w trybie Debug
           libc.lib      // w trybie Release

Uwaga: Czasami libcmtd.lib albo libcmt.lib
       pojawia siê w 2 miejscach. 
       Nale¿y wówczas usun¹æ jeden egzemplarz.
*/

// ============================================= CONSOLE


#include <pthread.h>
#include <signal.h>

#define Infinite INFINITE

#define Failed WAIT_FAILED         // -1
#define Signaled WAIT_OBJECT_0     // 0
#define Timeout WAIT_TIMEOUT       // 258
#define Abandoned WAIT_ABANDONED   // 128
#define Raised Signaled


// ============================================= Object

/* typedef Handle Object;

inline bool closeObject(Object &object)
{
    return CloseHandle(object) != 0;
} */
// ============================================= (Object)


// ============================================= Process

/* typedef Handle Process;

inline Process getCurrentProcess(void)
{
    return GetCurrentProcess();
}

inline void exitProcess(int code =0)
{
    ExitProcess(code);
} */

// ============================================= (Process)


// ============================================= Thread

typedef pthread_t Thread;

typedef void* (*Run)(void *);

#define waitForThreads waitForObjects
#define closeThread closeObject

inline Thread createThread(Run pRun, void *pArg =0)
{
    //int id = 0;
    Thread thread;
    pthread_create( &thread, NULL, pRun, pArg);

/*        CreateThread(
            0,                           // no security
            0,                           // default stack
            (Runner)pRun,                // start function
            pArg,                        // start argument
            stop ? 1 : 0,                // start suspended
            (DWORD *)(pId ? pId : &id)   // thread ID
        ); */

//    if( !res ) throw GetLastError();

    return thread;
}

inline void exitThread(int code =0)
{
    pthread_exit(&code);
}

inline int waitForThread(Thread &thread )
{
    return pthread_join(thread, NULL);
}

inline bool killThread(Thread &thread, int code =0)
{
    return pthread_kill(thread, code) != 0;
}

inline Thread getCurrentThread(void)
{
    return pthread_self();
}

// ============================================= (Thread)


// ============================================= Mutex

typedef pthread_mutex_t Mutex;

/*
inline Mutex createMutex(bool signaled =true, 
                         char *pName =0)
{
    Mutex mutex =  PTHREAD_MUTEX_INITIALIZER;
    return mutex;
}*/

inline bool freeMutex(Mutex &mutex)
{
    return pthread_mutex_destroy(&mutex) != 0;
}

// ============================================= (Mutex)


// ============================================= Section

typedef Mutex Section;

inline Section createSection()
{
    Mutex mutex =  PTHREAD_MUTEX_INITIALIZER;
    return mutex;
}

inline void enterSection(Section &section)
{
    pthread_mutex_lock( &section );
}

inline void leaveSection(Section &section)
{
     pthread_mutex_unlock( &section );
}

#endif
