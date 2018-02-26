#pragma once
#ifndef __APPACTION_H__
#define __APPACTION_H__

#include "imemory/object.hpp"
#include "window.h"

namespace ROOT_NAMESPACE
{
    class appaction : public object
    {
        friend class window;
    public:
        static appaction & instance( void );
    protected:
        CREATEFUNC(appaction);
        virtual bool init( void ) override;

        appaction( void );
        virtual ~appaction( void );

        virtual void tick( const float p_tm );
    private:

        window * m_window;
    };
}

#endif //__APPACTION_H__