#pragma once
#ifndef __GC_WORKER_HPP__
#define __GC_WORKER_HPP__

#include "baseObj.hpp"
#include "gc.hpp"

namespace ROOT_NAMESPACE
{
    class gcWorker
    {
    public:
        gcWorker(void);

        ~gcWorker(void);

        //将对象加入自动回收系统
        static void autoRelease( baseObj & p_bobj );

        static void remove( baseObj & p_bobj );

        static void immediatelyDestroy( void );

    private:

        static std::map< unsigned int, std::stack< gcWorker * > * > & GcWorkers(void);
		
        unsigned int mThreadId;
        std::list< baseObj * > mManageObjList;
    };
}

#endif //__GC_WORKER_HPP__

#include "achieve/gcWorker.inl"