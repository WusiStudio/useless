#pragma once
#ifndef __X_GRAPHICAL_HPP__
#define __X_GRAPHICAL_HPP__

#include "windowStruct.h"
#include "imemory/object.hpp"

#ifdef OS_LINUX
    #include <GL/glx.h>
#endif

namespace ROOT_NAMESPACE
{
    class xgraphical : public object
    {
    public:
        static xgraphical & Create ( windowStruct & p_windowStruct );

        bool renderStart ( windowStruct & p_windowStruct );
        bool renderEnd ( windowStruct & p_windowStruct );

    protected:
        CREATEFUNC ( xgraphical );
        virtual bool init ( void ) override;
        virtual bool init ( windowStruct & p_windowStruct );

        virtual bool destroy( void ) override;
        
        xgraphical ( void );
        virtual ~xgraphical ( void );
    private:
        #ifdef OS_LINUX
            GLXDrawable m_drawable;
            Display * m_display;
            GLXContext m_context;
        #endif
    };
}

#endif //__X_GRAPHICAL_HPP__