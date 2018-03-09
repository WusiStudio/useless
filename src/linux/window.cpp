#include "window.h"
#include "tools/log.hpp"
#include "xgraphical.h"

#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <GL/glx.h>

namespace ROOT_NAMESPACE
{

    void print_modifiers (uint32_t mask)
    {
        const char *MODIFIERS[] = {
                "Shift", "Lock", "Ctrl", "Alt",
                "Mod2", "Mod3", "Mod4", "Mod5",
                "Button1", "Button2", "Button3", "Button4", "Button5"
        };

        printf ("Modifier mask: ");
        for (const char **modifier = MODIFIERS ; mask; mask >>= 1, ++modifier) {
            if (mask & 1) {
                printf (*modifier);
            }
        }
        printf ("\n");
    }

    glm::ivec2 window::GetSystemResolution( void )
    {
        if( !m_Header.xcb_screen )
        {
            LOG.error( "Not find scb_screen" );
            return glm::ivec2( 0 );
        }

        return glm::ivec2( m_Header.xcb_screen->width_in_pixels, m_Header.xcb_screen->height_in_pixels );
    }

    bool window::setTitle( const std::string & p_title )
    {
        ::xcb_change_property( m_Header.xcb_connection, XCB_PROP_MODE_REPLACE, m_Header.xcb_window,
                            XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                            p_title.size(), p_title.c_str() );

        ::xcb_flush( m_Header.xcb_connection );
        return false;
    }

    bool window::setSize( const glm::ivec2 & p_size )
    {
        if( !m_Header.xcb_connection || !m_Header.xcb_screen )
        {
            return true;
        }

        uint32_t t_mask = XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;
        uint32_t t_values[2] = { (uint32_t)p_size.x, (uint32_t)p_size.y };

        xcb_configure_window( m_Header.xcb_connection, m_Header.xcb_window, t_mask, t_values );

        ::xcb_flush( m_Header.xcb_connection );

        return false;
    }

    bool window::setPosition( const glm::ivec2 & p_position )
    {

        if( !m_Header.xcb_connection || !m_Header.xcb_screen )
        {
            return true;
        }

        uint32_t t_mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
        uint32_t t_values[2] = { (uint32_t)p_position.x, (uint32_t)p_position.y };

        xcb_configure_window( m_Header.xcb_connection, m_Header.xcb_window, t_mask, t_values );

        ::xcb_flush( m_Header.xcb_connection );
        
        return false;
    }

    bool window::setCursorPos( const glm::ivec2 & p_cursorPos )
    {
        return false;
    }

    const bool window::processEvent( void )
    {
        t_event = ::xcb_poll_for_event( m_Header.xcb_connection );

        if( t_event )
        {
            switch ( t_event->response_type & ~0x80 ) {
            case XCB_EXPOSE: {
                xcb_expose_event_t *expose = (xcb_expose_event_t *)t_event;

                m_Size = glm::ivec2( expose->width, expose->height );

                LOG.info ("Window {0} exposed. Region to be redrawn at location ({1}, {2}), with dimension ({3}, {4})",
                        expose->window, expose->x, expose->y, expose->width, expose->height );
                break;
            }
            case XCB_BUTTON_PRESS: {
                xcb_button_press_event_t *t_bp = (xcb_button_press_event_t *)t_event;
                // print_modifiers (t_bp->state);

                LOG.info ( "Button {0} pressed in window {1}, at coordinates ({2}, {3})",
                            t_bp->detail, t_bp->event, t_bp->event_x, t_bp->event_y );

                m_Input.mouseInput[ t_bp->detail ] = true;
                m_Input.mouseInputX[ t_bp->detail ] = t_bp->event_x;
                m_Input.mouseInputY[ t_bp->detail ] = t_bp->event_y;
                break;
            }
            case XCB_BUTTON_RELEASE: {
                xcb_button_release_event_t * t_br = (xcb_button_release_event_t *)t_event;
                print_modifiers(t_br->state);

                LOG.info ( "Button {0} released in window {1}, at coordinates ({2}, {3})",
                        t_br->detail, t_br->event, t_br->event_x, t_br->event_y );
                break;
            }
            case XCB_MOTION_NOTIFY: {
                xcb_motion_notify_event_t * t_motion = (xcb_motion_notify_event_t *)t_event;

                m_CursorPosition = glm::ivec2( t_motion->event_x, t_motion->event_y );
                // LOG.info ( "Mouse moved in window {0}, at coordinates ({1}, {2})",
                //         t_motion->event, t_motion->event_x, t_motion->event_y );
                break;
            }
            case XCB_ENTER_NOTIFY: {
                xcb_enter_notify_event_t * t_enter = (xcb_enter_notify_event_t *)t_event;

                // LOG.info ("Mouse entered window {0}, at coordinates ({1}, {2})",
                //         t_enter->event, t_enter->event_x, t_enter->event_y );
                break;
            }
            case XCB_LEAVE_NOTIFY: {
                xcb_leave_notify_event_t * t_leave = (xcb_leave_notify_event_t *)t_event;

                // LOG.info ("Mouse left window {0}, at coordinates ({1}, {2})",
                //         t_leave->event, t_leave->event_x, t_leave->event_y );
                break;
            }
            case XCB_KEY_PRESS: {
                xcb_key_press_event_t * t_kp = (xcb_key_press_event_t *)t_event;

                m_Input.keyInput[t_kp->detail] = true;

                LOG.info ("Key:({0}) pressed in window {1}", (int)t_kp->detail, t_kp->event);
                break;
            }
            case XCB_KEY_RELEASE: {
                xcb_key_release_event_t * t_kr = (xcb_key_release_event_t *)t_event;
                print_modifiers( t_kr->state );

                // LOG.info ("Key released in window {0}", t_kr->event);
                break;
            }
            case XCB_CLIENT_MESSAGE: {
                xcb_client_message_event_t * t_cme = ( xcb_client_message_event_t * )t_event;

                if( t_cme->data.data32[0] == m_Header.xcb_atom_delete_window )
                {
                    m_Run = false;
                }

                break;
            }
            default:
                /* Unknown event type, ignore it */
                LOG.info ("Unknown event: {0}", t_event->response_type);
                break;
            }
            free ( t_event );
        }

        return false;
    }

    bool window::setFullScreenState( const bool p_fullScreenState )
    {

        if( !m_Header.xcb_connection || !m_Header.xcb_screen || m_FullScreenState == p_fullScreenState )
        {
            return true;
        }

        if( p_fullScreenState )
        {
            m_fullscreenAfterSize = m_Size;
            setPosition( glm::ivec2( 0 ) );
            setSize( GetSystemResolution() );
        }else
        {
            setPosition( ( GetSystemResolution() - m_fullscreenAfterSize ) / 2 );
            setSize( m_fullscreenAfterSize );
        }

        m_FullScreenState = p_fullScreenState;
            
        // xcb_intern_atom_cookie_t t_wmStateAddCookie = xcb_intern_atom_unchecked( m_Header.xcb_connection, 1, 12, "_NET_WM_STATE" );
        // xcb_intern_atom_reply_t* t_wmStateAddReply = xcb_intern_atom_reply( m_Header.xcb_connection, t_wmStateAddCookie, 0);

        // xcb_intern_atom_cookie_t t_fullScreenCookie = xcb_intern_atom_unchecked( m_Header.xcb_connection, 1, 24, "_NET_WM_STATE_FULLSCREEN" );
        // xcb_intern_atom_reply_t* t_fullScreenReply = xcb_intern_atom_reply( m_Header.xcb_connection, t_fullScreenCookie, 0);
        // xcb_change_property( m_Header.xcb_connection, XCB_PROP_MODE_REPLACE, m_Header.xcb_window, t_fullScreenReply->atom, 4, 32, 1, &( t_fullScreenReply->atom ) );
        // free( t_fullScreenReply );
        // free( t_wmStateAddReply );


        // LOG.info("--------------");

        return false;
    }

    bool window::init( const std::string & p_title, const glm::ivec2 & p_size, const glm::ivec2 & p_position, const bool p_fullScreenState, const bool p_centerInDesktop, const bool p_showCursor )
    {
        glm::ivec2 t_position = p_position;

        m_Header.x_display = ::XOpenDisplay( nullptr );

        if( !m_Header.x_display  )
        {
            LOG.error( "Can't open display" );
            return true;
        }

        m_Header.x_default_screen = DefaultScreen( m_Header.x_display );

        m_Header.xcb_connection = ::XGetXCBConnection( m_Header.x_display );
        
        if( !m_Header.xcb_connection )
        {
            ::XCloseDisplay( m_Header.x_display );
            LOG.error( "Can't get xcb connection from display" );
            return true;
        }

        /* Acquire event queue ownership */
        ::XSetEventQueueOwner( m_Header.x_display, XCBOwnsEventQueue );

        xcb_screen_iterator_t t_screen_iter = ::xcb_setup_roots_iterator( xcb_get_setup( m_Header.xcb_connection ) );

        for( int screen_num = m_Header.x_default_screen; t_screen_iter.rem && screen_num > 0; --screen_num, xcb_screen_next( &t_screen_iter ) );
        m_Header.xcb_screen = t_screen_iter.data;

        if( !m_Header.xcb_screen )
        {
            LOG.error( "Cannot retrieve screen information" );
            return true;
        }

        int t_visualID = 0;
        GLXFBConfig * t_fb_configs = 0;
        int t_num_fb_configs = 0;
        t_fb_configs = ::glXGetFBConfigs( m_Header.x_display, m_Header.x_default_screen, &t_num_fb_configs );
        if( !t_fb_configs || t_num_fb_configs == 0 )
        {
            LOG.error( "glXGetFBConfigs failed" );
            return true;
        }

        /* Select first framebuffer config and query visualID */
        m_Header.x_fb_config = t_fb_configs[0];
        ::glXGetFBConfigAttrib( m_Header.x_display, m_Header.x_fb_config, GLX_VISUAL_ID , &t_visualID );

        /* Create XID's for colormap */
        xcb_colormap_t colormap = ::xcb_generate_id( m_Header.xcb_connection  );

        ::xcb_create_colormap(
            m_Header.xcb_connection,
            XCB_COLORMAP_ALLOC_NONE,
            colormap,
            m_Header.xcb_screen->root,
            t_visualID
            );

        xcb_alloc_color_reply_t *t_rep = xcb_alloc_color_reply ( m_Header.xcb_connection, xcb_alloc_color ( m_Header.xcb_connection, colormap, 65535, 0, 0 ), NULL );

        if ( !t_rep )
        {
            LOG.error( "alloc color reply fiald" );
            return true;
        }
            
        m_Header.xcb_window = ::xcb_generate_id( m_Header.xcb_connection );
        uint32_t     mask      =  XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;
        uint32_t     values[] = {   XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_BUTTON_PRESS   |
                                    XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
                                    XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW   |
                                    XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE,
                                    colormap, 0 };

        if( p_centerInDesktop )
        {
            glm::ivec2 t_screenSize = GetSystemResolution();
            t_position = glm::ivec2( (t_screenSize.x - p_size.x) / 2, ( t_screenSize.y - p_size.y ) / 2 );
        }
        

        LOG.info( "window position: ({0}, {1})", t_position.x, t_position.y );

        xcb_void_cookie_t gc_cookie = ::xcb_create_window(
            m_Header.xcb_connection,
            XCB_COPY_FROM_PARENT,
            m_Header.xcb_window,
            m_Header.xcb_screen->root,
            t_position.x, 
            t_position.y,
            p_size.x, p_size.y,
            0,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            t_visualID,
            mask, values);

        xcb_generic_error_t * t_error = xcb_request_check( m_Header.xcb_connection, gc_cookie );

        if ( t_error != nullptr )
        {
            LOG.error( "Could not create window. Error code: {0}", t_error->error_code );
        }

        /* set the title of the window */
        ::xcb_change_property( m_Header.xcb_connection, XCB_PROP_MODE_REPLACE, m_Header.xcb_window,
                            XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                            p_title.size(), p_title.c_str() );

        // setup close handler event
        xcb_intern_atom_cookie_t t_protocolCookie = xcb_intern_atom_unchecked( m_Header.xcb_connection, 1, 12, "WM_PROTOCOLS" );
        xcb_intern_atom_reply_t* t_protocolReply = xcb_intern_atom_reply( m_Header.xcb_connection, t_protocolCookie, 0);
        xcb_intern_atom_cookie_t t_closeCookie = xcb_intern_atom_unchecked( m_Header.xcb_connection, 0, 16, "WM_DELETE_WINDOW" );
        xcb_intern_atom_reply_t * t_closeReply = xcb_intern_atom_reply( m_Header.xcb_connection, t_closeCookie, 0 );
        m_Header.xcb_atom_wm_protocals = t_protocolReply->atom;
        m_Header.xcb_atom_delete_window = t_closeReply->atom;
        xcb_change_property( m_Header.xcb_connection, XCB_PROP_MODE_REPLACE, m_Header.xcb_window, t_protocolReply->atom, 4, 32, 1, &( t_closeReply->atom ) );
        free( t_protocolReply );
        free( t_closeReply );

        ::xcb_map_window( m_Header.xcb_connection, m_Header.xcb_window );
        ::xcb_flush( m_Header.xcb_connection );

        setPosition( t_position );

        m_Xgraphical = &xgraphical::Create ( m_Header );
        
        return false;
    }

    bool window::init( void )
    {
        CALL( object::init() );

        m_Header.xcb_connection = nullptr;
        m_Header.xcb_screen = nullptr;
        m_Header.x_display = nullptr;
        m_Header.x_default_screen = 0;

        return false;
    }

    bool window::run( void )
    {
        m_Run = true;
        xcb_generic_event_t * t_event;
        
        while ( m_Run )
        {
            if( processEvent() )
            {
                break;
            }

            if( m_Input.keyInput[KEY_ESCAPE] )
            {
                m_Run = false;
                break;
            }

            if( m_Input.keyInput[KEY_N] )
            {
                if( setTitle( "haha" ) )
                {
                    LOG.error( "setTitle faild" );
                }
            }

            if( m_Input.keyInput[KEY_R] )
            {
                if( setSize( glm::ivec2( 800, 600 ) ) )
                {
                    LOG.error( "setSize faild" );
                }
            }

            if( m_Input.keyInput[KEY_T] )
            {
                if( setSize( glm::ivec2( 1024, 768 ) ) )
                {
                    LOG.error( "setSize faild" );
                }
            }

            if( m_Input.keyInput[KEY_F] )
            {
                setFullScreenState( !m_FullScreenState );
            }

            m_Xgraphical->renderStart( m_Header );

            m_event_Tick( 1.0f );

            m_Xgraphical->renderEnd( m_Header );

            usleep ( 1000 );

            consumeInput();
        }

        return false;
    }

    bool window::destroy( void )
    {
        CALL( object::destroy() );

        m_Xgraphical->release();

        if( m_Header.xcb_connection )
        {
            
            unsigned int t_values[] = { XCB_EVENT_MASK_NO_EVENT };
            ::xcb_change_window_attributes( m_Header.xcb_connection, m_Header.xcb_window, XCB_CW_EVENT_MASK, t_values );

            ::xcb_destroy_window( m_Header.xcb_connection, m_Header.xcb_window );

            m_Header.xcb_connection = nullptr;

            ::XCloseDisplay( m_Header.x_display );
        }
            
        return false;
    }
}