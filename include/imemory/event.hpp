#pragma once
#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include "baseObj.hpp"
#include "tools/uuidExt.hpp"

#include <map>

namespace ROOT_NAMESPACE
{
    template<typename eventType>
    struct event
    {
    public:
        event();

        ~event();

        event & operator+( const eventType & p_func );

        event & operator=( const eventType & p_func );

        event & operator+=( const eventType & p_func );

        uuid append( const eventType & p_func );

        uuid bind( const eventType & p_func );

        event & operator-( const ROOT_NAMESPACE::uuid & p_funcId );

        event & unbind( const ROOT_NAMESPACE::uuid & p_funcId );

        event & clear( void );

        template<typename... Arguments>
        void operator()( const Arguments & ... p_args );

        template<typename... Arguments>
        void call( const Arguments & ... p_args );

    private:
        std::map< std::string, eventType > mEvents;
    };
}

#endif //__EVENT_HPP__

#include "achieve/event.inl"