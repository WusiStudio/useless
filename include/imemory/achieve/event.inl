#pragma once
#ifndef __EVENT_INL__
#define __EVENT_INL__

namespace ROOT_NAMESPACE
{
    template<typename eventType>
    inline event<eventType>::event()
    {

    }

    template<typename eventType>
    inline event<eventType>::~event()
    {
        
    }

    template<typename eventType>
    inline event<eventType> & event<eventType>::operator+( const eventType & p_func )
    {
        bind( p_func );
        return * this;
    }

    template<typename eventType>
    inline event<eventType> & event<eventType>::operator=( const eventType & p_func )
    {
        bind( p_func );
        return * this;
    }

    template<typename eventType>
    inline event<eventType> & event<eventType>::operator+=( const eventType & p_func )
    {
        bind( p_func );
        return * this;
    }

    template<typename eventType>
    inline uuid event<eventType>::append( const eventType & p_func )
    {
        return bind( p_func );
    }

    template<typename eventType>
    inline uuid event<eventType>::bind( const eventType & p_func )
    {
        uuid result;
        mEvents[result.toString()] = p_func;
        return result;
    }

    template<typename eventType>
    inline event<eventType> & event<eventType>::operator-( const ROOT_NAMESPACE::uuid & p_funcId )
    {
        return unbind( p_funcId );
    }

    template<typename eventType>
    inline event<eventType> & event<eventType>::unbind( const ROOT_NAMESPACE::uuid & p_funcId )
    {
        if( mEvents.find( p_funcId.toString() ) != mEvents.end() )
        {
            mEvents.erase( p_funcId.toString() );
        }
        return * this;
    }

    template<typename eventType>
    inline event<eventType> & event<eventType>::clear( void )
    {
        mEvents.clear();
        return * this;
    }

    template<typename eventType>
    template<typename... Arguments>
    inline void event<eventType>::operator()( const Arguments & ... p_args )
    {
        call( p_args... );
    }

    template<typename eventType>
    template<typename... Arguments>
    inline void event<eventType>::call( const Arguments & ... p_args )
    {
        for( auto item : mEvents)
        {
            item.second( p_args... );
        }
    }
}

#endif //__EVENT_INL__