#pragma once
#ifndef __X_GRAPHICAL_HPP__
#define __X_GRAPHICAL_HPP__

#include "windowHeader.h"
#include "imemory/object.hpp"

#ifdef OS_LINUX
    #include <GL/glx.h>
#endif

namespace ROOT_NAMESPACE
{
    class xgraphical : public object
    {
    public:
        static xgraphical & Create ( windowHeader & p_windowHeader );

        bool renderStart ( windowHeader & p_windowHeader );
        bool renderEnd ( windowHeader & p_windowHeader );

         virtual bool destroy( void ) override;
    protected:
        CREATEFUNC ( xgraphical );
        virtual bool init ( void ) override;
        virtual bool init ( windowHeader & p_windowHeader );
        
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