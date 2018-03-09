#pragma once
#ifndef __GC_WORKER_INL__
#define __GC_WORKER_INL__

#include "tools/log.hpp"
#include "tools/threadExt.hpp"

namespace ROOT_NAMESPACE
{
    inline gcWorker::gcWorker(void)
    {
        mThreadId = ROOT_NAMESPACE::PthreadSelf();
        if( GcWorkers().find( mThreadId ) == GcWorkers().end() )
        {
            GcWorkers()[ mThreadId ] = new std::stack< gcWorker * >();
        }
        GcWorkers()[ mThreadId ]->push( this );
    }

    inline gcWorker::~gcWorker(void)
    {
        //将不用的对象放入缓存中 
        GcWorkers()[ mThreadId ]->pop();
        gcWorker * t_gcWorker = nullptr;
        
        if( GcWorkers()[ mThreadId ]->size() > 0 )
        {
            t_gcWorker = GcWorkers()[ mThreadId ]->top();
        }else
        {
            //删除当前栈管理对象
            if( GcWorkers()[ mThreadId ]->size() <= 0 )
            {
                delete GcWorkers()[ mThreadId ];
                GcWorkers().erase( mThreadId );
            }
        }

        for(baseObj * item : mManageObjList)
        {
            if( t_gcWorker && item->quote() > 0 )
            {
                t_gcWorker->mManageObjList.push_back( item );
                continue;
            }

            if( item->destroy() )
            {
                continue;
            }

            gc::instance().cacheObj( *item );                
        }
    }

    //将对象加入自动回收系统
    inline void gcWorker::autoRelease( baseObj & p_bobj )
    {
        if ( !GcWorkers().size () || GcWorkers().find( ROOT_NAMESPACE::PthreadSelf () ) == GcWorkers().end() || !GcWorkers()[ROOT_NAMESPACE::PthreadSelf ()]->size())
        {
            LOG.warning ( "the current thread has no gcWorker!" );
            return;
        }
        GcWorkers()[ROOT_NAMESPACE::PthreadSelf()]->top()->mManageObjList.push_back( &p_bobj );
    }

    inline void gcWorker::remove( baseObj & p_bobj )
    {
        if ( !GcWorkers().size () || GcWorkers().find( ROOT_NAMESPACE::PthreadSelf () ) == GcWorkers().end() || !GcWorkers()[ROOT_NAMESPACE::PthreadSelf ()]->size())
        {
            LOG.warning ( "the current thread has no gcWorker!" );
            return;
        }
        std::list< baseObj * > & tMangeObjectList = GcWorkers()[ROOT_NAMESPACE::PthreadSelf()]->top()->mManageObjList;
        for( std::list< baseObj * >::iterator t_item = tMangeObjectList.begin(); t_item != tMangeObjectList.end(); ++t_item )
        {
            if( *t_item == &p_bobj )
            {
                tMangeObjectList.erase(t_item);
                break;
            }
        }
    }

    inline void gcWorker::immediatelyDestroy( void )
    {
        if ( !GcWorkers().size () || GcWorkers().find( ROOT_NAMESPACE::PthreadSelf () ) == GcWorkers().end() || !GcWorkers()[ROOT_NAMESPACE::PthreadSelf ()]->size())
        {
            LOG.warning ( "the current thread has no gcWorker!" );
            return;
        }

        std::list< baseObj * > & t_ManageObjList = GcWorkers()[ROOT_NAMESPACE::PthreadSelf()]->top()->mManageObjList;


        for(baseObj * item : t_ManageObjList)
        {
            if( item->quote() > 0 )
            {
                continue;
            }

            if( item->destroy() )
            {
                continue;
            }

            gc::instance().cacheObj( *item );                
        }
    }

    inline std::map< unsigned int, std::stack< gcWorker * > * > & gcWorker::GcWorkers(void)
    {
        static std::map< unsigned int, std::stack< gcWorker * > * > smGcWorkers;
        return smGcWorkers;
    }
}

#endif //__GC_WORKER_INL__