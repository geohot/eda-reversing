// threading.h -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_THREADING_H_
#define EDA_THREADING_H_

//it's the geohot magic thread macros
//threadCreate(&threadContainer, &function, param)
#ifdef __MINGW32__  //|| __CYGWIN__
//windows threading

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/*void *CreateThread(int, int, DWORD WINAPI (*)(void*), void *, int, int);
int SuspendThread(void *);
int ExitThread(int);*/
//int

#define threadContainer void *  //HANDLE
#define threadCreate(y,x,z) (*y)=CreateThread(0,0,(DWORD WINAPI (*)(void*))x,(void *)z,0,0)
#define threadDestroy(y) SuspendThread(*y)
#define threadExit() ExitThread(0)
#define threadWait(x) WaitForSingleObject(*x,INFINITE)

#define mutexContainer CRITICAL_SECTION
#define mutexInit(x) InitializeCriticalSection(x)
#define mutexDestroy(x) DeleteCriticalSection(x)
#define mutexLock(x) EnterCriticalSection(x)
#define mutexTryLock(x) TryEnterCriticalSection(x)
#define mutexUnlock(x) LeaveCriticalSection(x)

#define eventContainer HANDLE
#define eventInit(x) (*x)=CreateEvent(0,true,false,0)
#define eventDestroy(x) CloseHandle(*x)
#define eventSet(x) SetEvent(*x)
#define eventReset(x) ResetEvent(*x)
#define eventWaitFor(x) WaitForSingleObject(*x, INFINITE)
#define eventIsSet(x) WaitForSingleObject(*x, 0)


#else
//pthreads
#include <pthread.h>

#define threadContainer pthread_t
#define threadCreate(y,x,z) pthread_create(y,0,x,(void *)z)
#define threadDestroy(y) pthread_cancel(*y);
#define threadExit() pthread_exit(0)
#define threadWait(y) pthread_join(*y)

#define mutexContainer pthread_mutex_t
#define mutexInit(x) pthread_mutex_init(x,0)
#define mutexDestroy(x) pthread_mutex_destroy(x)
#define mutexLock(x) pthread_mutex_lock(x)
#define mutexTryLock(x) pthread_mutex_trylock(x)
#define mutexUnlock(x) pthread_mutex_unlock(x)

//very hacky event code
#define eventContainer mutexContainer
#define eventInit(x) mutexInit(x); mutexLock(x)
#define eventDestroy(x) mutexDestroy(x)
#define eventSet(x) mutexUnlock(x)
#define eventWaitFor(x) mutexLock(x); mutexUnlock(x)
#define eventIsSet(x) mutexTryLock(x); mutexUnlock(x)
#define eventReset(x) mutexLock(x)

#endif


#endif /* EDA_THREADING_H_ */
