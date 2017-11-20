#pragma once
#ifndef __X_GRAPHICAL_H__
#define __X_GRAPHICAL_H__

#include "imemory/object.hpp"

namespace ws
{
    class xgraphical : public object
    {
        CREATEFUNC(xgraphical);
    public:
        virtual bool init( void ) override;
    };
}

#endif //__X_GRAPHICAL_H__