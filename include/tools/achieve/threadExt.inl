#pragma once
#ifndef __THREAD_EXT_INL__
#define __THREAD_EXT_INL__

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
    #include <windows.h>
#elif defined(__linux__) || defined(__unix__)
    #include <pthread.h>
#endif

namespace ws
{
    inline unsigned int PthreadSelf()
    {
    #if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
        return ::GetCurrentThreadId();
    #elif defined(__linux__) || defined(__unix__)
        return pthread_self();
    #else
        return -1;
    #endif
    }
}

#endif //__THREAD_EXT_INL__