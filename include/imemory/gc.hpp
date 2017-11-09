#pragma once
#ifndef __GC_H__
#define __GC_H__

#include "baseObj.hpp"
#include "object.hpp"
#include "tools/log.hpp"
#include "tools/threadExt.hpp"

#include <map>
#include <stack>
#include <list>

namespace ws
{
    class gc : public baseObj
    {
    public:

        static gc & instance()
        {
            unsigned int tThreadId = ws::threadExt::PthreadSelf();
            if( Instances().find( tThreadId ) == Instances().end() )
            {
                Instances()[ tThreadId ] = new gc();
            }

            return * Instances()[ tThreadId ];
        }

        void makeCache(void)
        {
            //清空三级缓存
            for( auto & cache : mCaches )
            {
                // for( auto & cacheByClass : cache )
                // {
                //     delete cacheByClass.second;
                //     LOG.info("---------test");
                // }
                cache.clear();
            }
            LOG.debug( "mObjCacheList.size() = {0}", mObjCacheList.size() );

            //缓存对象排序
            mObjCacheList.sort( []( const baseObj * first, const baseObj * second )->bool{
                return first->frequency() < second->frequency();
            } );

            LOG.debug( "mObjCacheList.size() = {0}", mObjCacheList.size() );

            //将缓存对象分配到三级缓存
            int tCurrObjIndex = 0;
            for( auto item = mObjCacheList.begin(); item != mObjCacheList.end(); item++ )
            {
                std::map< std::string, std::list< std::list< baseObj* >::iterator > * > * tCacheList = nullptr;
                if( tCurrObjIndex <= 1000 )
                {
                    tCacheList = mCaches;
                }
                else if( tCurrObjIndex <= 5000 )
                {
                    tCacheList = mCaches + 1;
                }
                else if( tCurrObjIndex <= 50000 )
                {
                    tCacheList = mCaches + 2;
                }

				tCurrObjIndex++;

                LOG.debug("---");
                //删除超出的缓存对象
                if((*item)->frequencyMinusOne () <= -300 || tCacheList == nullptr )
                {
                    delete *item;
                    mObjCacheList.erase( item-- );
                    continue;
                }

                std::string tObjTypeName = (*item)->realType();
				if ( tCacheList->find ( tObjTypeName ) == tCacheList->end () )
				{
					tCacheList->operator[]( tObjTypeName ) = new std::list< std::list< baseObj* >::iterator > ();
				}

                LOG.debug( "attch {0} class name: {1}", *item, tObjTypeName );
				tCacheList->operator[]( tObjTypeName )->push_front ( item );
            }
        }
        
        void cacheObj( baseObj & p_obj )
        {
            std::string tClassName = p_obj.realType();
            if( mCaches[0].find( tClassName ) == mCaches[0].end() )
            {
                mCaches[0][ tClassName ] = new std::list< std::list< baseObj* >::iterator >();
            }
            std::list< std::list< baseObj* >::iterator > & tCache = *mCaches[0][ tClassName ];
            
            tCache.push_front( insertToCacheList( p_obj ) );

            LOG.debug( " attch object:{1} mObjCacheList.size() = {0}", mObjCacheList.size(), tClassName );
        }

        baseObj * getObj( const std::string & p_classId )
        {
			baseObj * result = nullptr;
            for( auto & cache : mCaches )
            {
                if( cache.find( p_classId ) ==  cache.end() ) continue;
                std::list< std::list< baseObj* >::iterator > * tObjCache = cache[ p_classId ];
                if( tObjCache->empty() ) continue;
                std::list< baseObj* >::iterator tObjIterator = tObjCache->front();
                tObjCache->pop_front();
				result = *tObjIterator;
                mObjCacheList.erase( tObjIterator );
				result->setFrequency( result->frequency() < 0 ? 1 : result->frequency() + 1 );
				break;
            }
            return result;
        }

        bool destory(void)
        {
            //清空三级缓存
            for( auto & cache : mCaches )
            {
                cache.clear();
            }
            
            //销毁所有缓存对象
            for( auto obj : mObjCacheList )
            {
                delete obj;
            }
            mObjCacheList.clear();
			return baseObj::destory ();
        }
    private:

        std::list< baseObj* >::iterator insertToCacheList( baseObj & p_obj )
        {
            mObjCacheList.push_front( & p_obj );
            return mObjCacheList.begin();
        }

        static std::map<unsigned int, gc *> & Instances(void)
        {
            static std::map<unsigned int, gc *> smInstances;
            return smInstances;
        }
        

        //三级缓存
        std::map< std::string, std::list< std::list< baseObj* >::iterator > * > mCaches[3];
        std::list< baseObj* > mObjCacheList;
    };

}

#endif //__GC_H__