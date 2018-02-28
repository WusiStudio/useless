#pragma once
#ifndef __X_GRAPHICAL_HPP__
#define __X_GRAPHICAL_HPP__

#include "windowHeader.h"
#include "imemory\object.hpp"

namespace ROOT_NAMESPACE
{
    class xgraphical : public object
    {
    public:
        static xgraphical & Create ( windowHeader & p_windowHeader );

        bool renderStart ( windowHeader & p_windowHeader );
        bool renderEnd ( windowHeader & p_windowHeader );
    protected:
        CREATEFUNC ( xgraphical );
        virtual bool init ( void ) override;
        virtual bool init ( windowHeader & p_windowHeader );

        xgraphical ( void );
        virtual ~xgraphical ( void );
    private:
        
    };
}

#endif //__X_GRAPHICAL_HPP__