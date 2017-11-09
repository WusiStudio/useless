#pragma once
#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include "baseObj.hpp"
#include "tools/uuidExt.hpp"

#include <map>
#include <vector>

namespace ws
{
    template<typename eventType>
    struct event
    {
    public:
        event()
        {

        }

        ~event()
        {
            
        }


        event & operator+( const eventType & p_func )
        {
            bind( p_func );
            return * this;
        }

        event & operator=( const eventType & p_func )
        {
            bind( p_func );
            return * this;
        }

        event & operator+=( const eventType & p_func )
        {
            bind( p_func );
            return * this;
        }

        uuid append( const eventType & p_func )
        {
            return bind( p_func );
        }

        uuid bind( const eventType & p_func )
        {
            uuid result;
            mEvents[result.toString()] = p_func;
            return result;
        }

        event & operator-( const ws::uuid & p_funcId )
        {
            return unbind( p_funcId );
        }

        event & unbind( ws::uuid & p_funcId )
        {
            if( mEvents.find( p_funcId.toString() ) != mEvents.end() )
            {
                mEvents.erase( p_funcId.toString() );
            }
            return * this;
        }

        event & clear( void )
        {
            mEvents.clear();
            return * this;
        }

        template<typename... Arguments>
        void operator()( const Arguments & ... p_args )
        {
            call( p_args... );
        }

        template<typename... Arguments>
        void call( const Arguments & ... p_args )
        {
            for( auto item : mEvents)
            {
                item.second( p_args... );
            }
        }

    private:
        std::map< std::string, eventType > mEvents;
    };
}

#endif //__EVENT_HPP__