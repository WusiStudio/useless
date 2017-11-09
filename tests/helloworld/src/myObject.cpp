#include "../include/myObject.h"
#include "tools/log.hpp"


bool myObject::init( void )
{
    if( ws::object::init() )
    {
        return true;
    }

    LOG.debug("init {0}", realType());

    return false;
}

bool myObject::destory( void )
{
    if( ws::object::destory() )
    {
        return true;
    }

    LOG.debug("destory {0}", realType());

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