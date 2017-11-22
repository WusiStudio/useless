#pragma once
#ifndef __OBJECT_INL__
#define __OBJECT_INL__

namespace ws
{
    //引用加一
    inline void object::retain( void )
    {
        baseObj::retain();
    }

    //引用减一
    inline void object::release( void )
    {
        baseObj::release();

        if( baseObj::quote() > 0 ) return;

        if( quote() < 0 )
        {
            LOG.warning( "release does not match retain" );
        }

        if( quote() > 0 || this->destory() )
        {
            return;
        }

        gcWorker::remove( *this );
        gc::instance().cacheObj( *this );
    }

    inline bool object::init( void )
    {
        return baseObj::init();
    }

    inline bool object::destory( void )
    {
        return baseObj::destory();
    }

    inline object::object ( void )
    {

    }

    inline object::~object ( void )
    {

    }
}

#endif //__OBJECT_INL__