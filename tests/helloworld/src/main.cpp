#include "imemory/object.hpp"
#include "tools/log.hpp"
#include "../include/myObject.h"
#include "tools/strExt.hpp"
#include "tools/uuidExt.hpp"

void test( void )
{
    IMSTACK
    myObject & myobject = myObject::Create();
}

int main( int argc, char ** argv )
{
    IMSTACK
    
    test();

    LOG.debug("------------");
    IMGCMake
    LOG.debug("------------");

    LOG.debug("{0}{2}", " 0,", " 1,", " 2,", " 3");

    myObject & myobject = myObject::Create();

    ws::object * t_object = &myobject;
    LOG.debug( "t_object: {0}, {1}", typeid(t_object).name(), t_object->realType() );

    ws::baseObj * t_baseObject = &myobject;
    LOG.debug( "t_baseObject: {0}, {1}", typeid(t_baseObject).name(), t_baseObject->realType() );

    LOG.debug("------------");
    myobject.release();
    LOG.debug("------------");

    IMGCMake

    myObject & myobject2 = myObject::Create();

    LOG.info("hello world");


    LOG.debug("[{0, 7}]", "1");
    LOG.debug("[{0, 7}]", "12");
    LOG.debug("[{0, 7}]", "123");
    LOG.debug("[{0, 7}]", "1234");
    LOG.debug("[{0, 7}]", "12345");
    LOG.debug("[{0, 7}]", "123456");
    LOG.debug("[{0, 7}]", "7654321");
    LOG.debug("[{0, 7}]", "87654321");
    LOG.debug("[{0, 7}]", "987654321");

    LOG.debug("[{0, 7}]", "你");
	LOG.info ("-------------------");
    LOG.debug("[{0, 7}]", "你吃");
	LOG.info ("-------------------");
    LOG.debug("[{0, 7}]", "你吃饭");
    LOG.debug("[{0, 7}]", "你吃饭了");
    LOG.debug("[{0, 7}]", "你吃饭了吗");
    LOG.debug("[{0, 7}]", "你吃饭了吗？");
    


    LOG.debug("[{0, -7}]", "1");
    LOG.debug("[{0, -7}]", "12");
    LOG.debug("[{0, -7}]", "123");
    LOG.debug("[{0, -7}]", "1234");
    LOG.debug("[{0, -7}]", "12345");
    LOG.debug("[{0, -7}]", "123456");
    LOG.debug("[{0, -7}]", "7654321");
    LOG.debug("[{0, -7}]", "87654321");
    LOG.debug("[{0, -7}]", "987654321");

    LOG.debug("[{0, -7}]", "你");
    LOG.debug("[{0, -7}]", "你吃");
    LOG.debug("[{0, -7}]", "你吃饭");
    LOG.debug("[{0, -7}]", "你吃饭了");
    LOG.debug("[{0, -7}]", "你吃饭了吗");
    LOG.debug("[{0, -7}]", "你吃饭了吗？");


    LOG.debug("uuid test: ", ws::uuid().toString());


    LOG.debug("{0, C}", 12345678.901);

    return 0;
}