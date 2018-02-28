#include "myObject.h"
#include "tools/log.hpp"


bool myObject::init( void )
{
    if( ROOT_NAMESPACE::object::init() )
    {
        return true;
    }

    LOG.debug("init {0}", realType());

    return false;
}

bool myObject::destroy( void )
{
    if( ROOT_NAMESPACE::object::destroy() )
    {
        return true;
    }

    LOG.debug("destroy {0}", realType());

    return false;
}

myObject::myObject( void )
{
    LOG.debug("create {0}", realType());
}
myObject::~myObject( void )
{
    LOG.debug("delete {0}", realType());
}