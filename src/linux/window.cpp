#include "window.h"
#include "tools/log.hpp"
#include "xgraphical.h"

#include <unistd.h>

namespace ROOT_NAMESPACE
{
    glm::ivec2 window::GetSystemResolution( void )
    {
        return glm::ivec2( 0 );
    }

    bool window::setTitle( const std::string & p_title )
    {
        return false;
    }

    bool window::setSize( const glm::ivec2 & p_size )
    {
        return false;
    }

    bool window::setPosition( const glm::ivec2 & p_position )
    {
        return false;
    }

    bool window::setCursorPos( const glm::ivec2 & p_cursorPos )
    {
        return false;
    }

    const bool window::processEvent( void )
    {
        return false;
    }

    bool window::setFullScreenState( const bool p_fullScreenState )
    {
        return false;
    }

    bool window::init( const std::string & p_title, const glm::ivec2 & p_size, const glm::ivec2 & p_position, const bool p_fullScreenState, const bool p_centerInDesktop, const bool p_showCursor )
    {
        return false;
    }

    bool window::init( void )
    {
        CALL( object::init() );
        return false;
    }

    bool window::run( void )
    {
        m_Run = true;
        
        while ( m_Run )
        {
            usleep ( 10000 );
        }
    }

    bool window::destroy( void )
    {
        CALL( object::destroy() );
        return false;
    }
}