#pragma once
#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "baseObj.hpp"
#include "gc.hpp"
#include "gcWorker.hpp"
#include "event.hpp"
#include "tools/log.hpp"
#include <cassert>

namespace ROOT_NAMESPACE
{

#define CREATEFUNC( class )													\
        static class & Create( void )                                       \
            {                                                               \
                class * result = ( class * )ROOT_NAMESPACE::gc::instance ().getObj ( typeid( class ).name () );\
                if( !result )                                               \
                {                                                           \
                    result = new class();                                   \
                }                                                           \
                                                                            \
                if( result->init() )                                        \
                {                                                           \
                    delete result;                                          \
                    result = nullptr;                                       \
                }                                                           \
                                                                            \
                assert( result );                                           \
                                                                            \
                ROOT_NAMESPACE::gcWorker::autoRelease ( *(baseObj *)result );           \
                return * result;                                            \
            }                                                               \
        virtual const char * realType( void ) override                      \
            {                                                               \
                return typeid( class ).name();                              \
            }                                                               \

//当前作用域中启
#define IMSTACK		ROOT_NAMESPACE::gcWorker __FILE_##__LINE__;
#define IMGCMake	ROOT_NAMESPACE::gc::instance().makeCache();

#define ATTIRBUTE( type, name )                     \
    DEFINE_ATTIRBUTE( type, name )                       

#define ATTIRBUTE_W( type, name )                   \
    DEFINE_ATTIRBUTE( type, name )                  \
    DEFINE_ATTIRBUTE_W( type, name )                

#define ATTIRBUTE_R( type, name )                   \
    DEFINE_ATTIRBUTE( type, name );                 \
    DEFINE_ATTIRBUTE_R( type, name );               

#define ATTIRBUTE_WR( type, name )                  \
    DEFINE_ATTIRBUTE( type, name );                 \
    DEFINE_ATTIRBUTE_R( type, name );               \
    DEFINE_ATTIRBUTE_W( type, name )                

#define ATTIRBUTE_RW( type, name )                  \
    ATTIRBUTE_WR( type, name );                 

#define DEFINE_ATTIRBUTE( type, name )              \
    private:                                        \
        type    m_##name;                            
#define DEFINE_ATTIRBUTE_R( type, name )            \
    public:                                         \
        type get##name( void ) const                \
        {                                           \
            return m_##name;                        \
        }
#define DEFINE_ATTIRBUTE_W( type, name )            \
    public:                                         \
        void set##name ( const type & p_##name )    \
        {                                           \
            m_##name = p_##name;                    \
        }                                           

#define EVENT( func, name )                         \
    private:                                        \
        event< func > m_event_##name;               \
    public:                                         \
        void bind_##name( const func & p_func )     \
        {                                           \
            m_event_##name.bind( p_func );          \
        }                                           \
        void unbind_##name( const uuid & p_event )  \
        {                                           \
            m_event_##name.unbind( p_event );       \
        }                                          

#define CALL( expression )                          \
        if( expression )                            \
        {                                           \
            return true;                            \
        }                                           \

    class object : public baseObj
    { 
    public:

        CREATEFUNC( object );

        //引用加一
        virtual void retain( void ) override;

        //引用减一
        virtual void release( void ) override;

    protected:

        virtual bool init( void ) override;

        virtual bool destroy( void ) override;

		object ( void );

		virtual ~object ( void );

    private:
    };
}

#endif //__OBJECT_HPP__

#include "achieve/object.inl"