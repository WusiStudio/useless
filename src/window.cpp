#include "window.h"
#include "tools/log.hpp"
#include "xgraphical.h"

namespace ROOT_NAMESPACE
{

    window & window::Create( const std::string & p_title, const glm::ivec2 & p_size, const bool p_fullScreenState, const bool p_centerInDesktop, const bool p_showCursor )
    {
        window & t_result = Create();
        
        if( t_result.init( p_title, p_size, glm::ivec2( 0 ),  p_fullScreenState, p_centerInDesktop, p_showCursor ) )
        {
            LOG.error( "window init faile!" );
        }
        
        return t_result;
    }

    window & window::Create( const std::string & p_title, const glm::ivec2 & p_size, const glm::ivec2 & p_position, const bool p_showCursor )
    {
        window & t_result = Create();
        
        if( t_result.init( p_title, p_size, p_position, p_showCursor ) )
        {
            LOG.error( "window init faile!" );
        }
        
        return t_result;
    }

    window::window() : m_Xgraphical(nullptr)
    {

    }

    window::~window()
    {

    }

    const bool window::consumeInput( void )
    {
        memset( &m_Input, 0, sizeof( m_Input ) );
        return false;
    }

}