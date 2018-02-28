#include "node.h"

namespace ROOT_NAMESPACE
{
    node::node( void )
    {
        
    }

    node::~node( void )
    {
        
    }

    bool node::init( void )
    {
        CALL( object::init() );

        return false;
    }

    bool node::tick( const float p_tm )
    {

        return false;
    }

    bool node::draw( void )
    {

        return false;
    }
}