#include "appaction.h"

namespace ROOT_NAMESPACE
{

    bool appaction::init( void ) 
    {
        return object::init();
    }

    int appaction::exec( void )
    {
        if( !m_window )
        {
            bindWindow( window::Create( "window", glm::ivec2( 800, 600 ), false ) );
        }

        if( m_window->run() )
        {
            return -1;
        }

        return 0;
    }

    bool appaction::bindWindow( window & p_window )
    {
        if( m_window )
        {
            m_window->release();
            m_window = nullptr;
            gcWorker::immediatelyDestroy();
        }
        m_window = &p_window;
        m_window->retain();

        //绑定事件
        if( bindEvent() )
        {
            return true;
        }

        return false;
    }

    appaction::appaction( void ) : m_window ( nullptr )
    {
        
    }

    appaction::~appaction( void )
    {
        
    }

    bool appaction::tick( const float p_tm )
    {
        return false;
    }

     bool appaction::bindEvent( void )
     {
         m_window->bind_Tick ( [this]( const float p_tm )->void {
             IMSTACK
             if( tick ( p_tm ) )
             {
                 //异常处理
             }
         } );
         return false;
     }

     bool appaction::unbindEvent( void )
     {
         return false;
     }
}