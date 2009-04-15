// threading.h -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_THREADING_H_
#define EDA_THREADING_H_

//it's the geohot magic thread macros
//threadCreate(&threadContainer, &function)
#ifdef __MINGW32__  //|| __CYGWIN__
//windows threading
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define threadContainer HANDLE
#define threadCreate(y,x,z) (*y)=CreateThread(0,0,(DWORD WINAPI (*)(void*))x,(void *)z,0,0)
#define threadDestroy(y) SuspendThread(*y)
#define threadExit() ExitThread(0)
#define threadWait(x) WaitForSingleObject(*x,INFINITE)

#else
//pthreads
#include <pthread.h>

#define threadContainer pthread_t
#define threadCreate(y,x,z) pthread_create(y,0,x,(void *)z)
#define threadDestroy(y) pthread_cancel(*y);
#define threadExit() pthread_exit(0)
#define threadWait(y) pthread_join(*y)

#endif


#endif /* EDA_THREADING_H_ */
