#include "myAppaction.h"
#include "tools/log.hpp"

bool myAppaction::init( void )
{
    CALL( ws::appaction::init() );

    ws::window & my_window = ws::window::Create( "HelloWorld", glm::ivec2( 1360, 768 ), false );

    if( bindWindow( my_window ) )
    {
        return true;
    }

    return false;
}

bool myAppaction::tick( const float p_tm )
{
    CALL ( ws::appaction::tick ( p_tm ) );

    return false;
}