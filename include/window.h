#pragma once
#ifndef __WINDOWS_H__
#define __WINDOWS_H__

#include "object.hpp"

namespace ROOT_NAMESPACE
{
    class window : public object
    {
        CREATEFUNC(window);
    public:
        virtual bool init( void ) override;
    };
}

#endif //__WINDOWS_H__