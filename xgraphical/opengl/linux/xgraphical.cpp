#include "xgraphical.h"

#include <GL/gl.h>                        //必要的头文件

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <GL/glx.h>

namespace ROOT_NAMESPACE
{
    xgraphical & xgraphical::Create ( windowStruct & p_windowStruct )
    {
        xgraphical * t_result = &Create ();

        if (t_result->init ( p_windowStruct ))
        {
            t_result->destroy ();
            t_result = nullptr;
        }

        assert ( t_result );

        return *t_result;
    }

    bool xgraphical::init ( void )
    {
        if (object::init ())
        {
            return true;
        }
        m_drawable = 0;
        m_display = nullptr;
        return false;
    }

    bool xgraphical::init ( windowStruct & p_windowStruct )
    {
        m_display = p_windowStruct.x_display;

        m_context = ::glXCreateNewContext( p_windowStruct.x_display, p_windowStruct.x_fb_config, GLX_RGBA_TYPE, 0, true);
        if( !m_context )
        {
            LOG.error( "glXCreateNewContext failed" );
            return true;
        }

        /* Create GLX Window */

        GLXWindow t_glxwindow = glXCreateWindow( p_windowStruct.x_display, p_windowStruct.x_fb_config, p_windowStruct.xcb_window, 0 );

        m_drawable = t_glxwindow;

        /* make OpenGL context current */
        if(!glXMakeContextCurrent( p_windowStruct.x_display, m_drawable, m_drawable, m_context))
        {
            LOG.error( "glXMakeContextCurrent failed" );
            return true;
        }

        return false;
    }

    bool xgraphical::destroy( void )
    {
        CALL( object::destroy() );

        if( m_display )
        {
            ::glXDestroyWindow( m_display, m_drawable );

            ::glXDestroyContext( m_display, m_context );

            m_display = nullptr;
        }

        return false;
    }

    bool xgraphical::renderStart ( windowStruct & p_windowStruct )
    {

        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        return false;
    }
    bool xgraphical::renderEnd ( windowStruct & p_windowStruct )
    {
        glXSwapBuffers( p_windowStruct.x_display, m_drawable);
        return false;
    }


    xgraphical::~xgraphical ( void )
    {

    }

    xgraphical::xgraphical ( void )
    {

    }

}
