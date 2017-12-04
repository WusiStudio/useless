#pragma once
#ifndef __APPACTION_H__
#define __APPACTION_H__

#include "imemory/object.hpp"

namespace ROOT_NAMESPACE
{
    class appaction : public object
    {
        CREATEFUNC(appaction);
    public:
        virtual bool init( void ) override;
    protected:
        appaction( void );
        virtual ~appaction( void );
    };
}

#endif //__APPACTION_H__