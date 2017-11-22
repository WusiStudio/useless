#pragma once
#ifndef __MY_OBJECT_H__
#define __MY_OBJECT_H__

#include "imemory/object.hpp"

class myObject : public ROOT_NAMESPACE::object
{
public:
    CREATEFUNC(myObject);
protected:

    virtual bool init( void ) override;

    virtual bool destory( void ) override;

    myObject( void );
    ~myObject( void );
};

#endif //__MY_OBJECT_H__