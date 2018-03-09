#pragma once
#ifndef __OBJECT_INL__
#define __OBJECT_INL__

namespace ROOT_NAMESPACE
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

        if( quote() > 0 || this->destroy() )
        {
            return;
        }

        gcWorker::remove( *this );
        gc::instance().cacheObj( *this );
    }

    inline bool object::init( void )
    {
        CALL( baseObj::init() );
        return false;
    }

    inline bool object::destroy( void )
    {
        return baseObj::destroy();
    }

    inline object::object ( void )
    {

    }

    inline object::~object ( void )
    {

    }
}

#endif //__OBJECT_INL__