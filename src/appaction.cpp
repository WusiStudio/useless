#include "appaction.h"

namespace ROOT_NAMESPACE
{
    bool appaction::init( void ) 
    {
        m_window = &window::Create( "window", glm::ivec2( 800, 600 ), false );
        
        return object::init();
    }

    appaction::appaction( void ) : m_window ( nullptr )
    {
        
    }

    appaction::~appaction( void )
    {
        
    }

    void appaction::tick( const float p_tm )
    {
        
    }
}