#pragma once
#ifndef __APPACTION_H__
#define __APPACTION_H__

#include "imemory/object.hpp"
#include "window.h"

namespace ROOT_NAMESPACE
{
    class appaction : public object
    {
    public:
        int exec( void );

        bool bindWindow( window & p_window );
    protected:
        virtual bool init( void ) override;

        appaction( void );
        virtual ~appaction( void );

        virtual bool tick( const float p_tm );
    private:

        bool bindEvent( void );
        bool unbindEvent( void );

        window * m_window;
    };
}

#endif //__APPACTION_H__