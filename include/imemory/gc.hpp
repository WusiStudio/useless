#pragma once
#ifndef __GC_H__
#define __GC_H__

#include "baseObj.hpp"

#include <map>
#include <stack>
#include <list>

namespace ws
{
    class gc : public baseObj
    {
    public:

        static gc & instance();

        void makeCache(void);

        void cacheObj( baseObj & p_obj );

        baseObj * getObj( const std::string & p_classId );

        bool destory(void);
    private:

        std::list< baseObj* >::iterator insertToCacheList( baseObj & p_obj );

        static std::map<unsigned int, gc *> & Instances(void);
        
        //三级缓存
        std::map< std::string, std::list< std::list< baseObj* >::iterator > * > mCaches[3];
        std::list< baseObj* > mObjCacheList;
    };

}

#endif //__GC_H__

#include "gc.inl"