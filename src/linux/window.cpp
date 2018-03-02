#include "window.h"
#include "tools/log.hpp"
#include "xgraphical.h"

#include <unistd.h>
#include <xcb/xcb_ewmh.h>

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
        xcb_change_property( m_Header.xcb_connection, XCB_PROP_MODE_REPLACE, m_Header.xcb_window,
                            XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                            p_title.size(), p_title.c_str() );
        return false;
    }

    bool window::setSize( const glm::ivec2 & p_size )
    {
        return false;
    }

    bool window::setPosition( const glm::ivec2 & p_position )
    {
        return false;
    }

    bool window::setCursorPos( const glm::ivec2 & p_cursorPos )
    {
        return false;
    }

    const bool window::processEvent( void )
    {
        return false;
    }

    bool window::setFullScreenState( const bool p_fullScreenState )
    {
        return false;
    }

    bool window::init( const std::string & p_title, const glm::ivec2 & p_size, const glm::ivec2 & p_position, const bool p_fullScreenState, const bool p_centerInDesktop, const bool p_showCursor )
    {

        m_Header.xcb_connection = ::xcb_connect( nullptr, nullptr);

        if( ::xcb_connection_has_error( m_Header.xcb_connection ) )
        {
            LOG.error( "Cannot open display: {0}", ::getenv("DISPLAY") );
            return true;
        }

        m_Header.xcb_screen = ::xcb_setup_roots_iterator( ::xcb_get_setup( m_Header.xcb_connection ) ).data;
        if( !m_Header.xcb_screen )
        {
            LOG.error( "Cannot retrieve screen information" );
            return true;
        }

        m_Header.xcb_window = ::xcb_generate_id( m_Header.xcb_connection );
        uint32_t     mask      = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
        uint32_t     values[2] = {  m_Header.xcb_screen->white_pixel,
                                    XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_BUTTON_PRESS   |
                                    XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
                                    XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW   |
                                    XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE };

        xcb_void_cookie_t gc_cookie = ::xcb_create_window(
            m_Header.xcb_connection,
            XCB_COPY_FROM_PARENT,
            m_Header.xcb_window,
            m_Header.xcb_screen->root,
            p_position.x, p_position.y,
            p_size.x, p_size.y,
            10,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            m_Header.xcb_screen->root_visual,
            mask, values);

        xcb_generic_error_t * t_error = xcb_request_check( m_Header.xcb_connection, gc_cookie );

        if ( t_error != nullptr )
        {
            LOG.error( "Could not create window. Error code: {0}", t_error->error_code );
        }

        /* set the title of the window */
        xcb_change_property( m_Header.xcb_connection, XCB_PROP_MODE_REPLACE, m_Header.xcb_window,
                            XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                            p_title.size(), p_title.c_str() );

        // xcb_ewmh_connection_t EWMH;
        // xcb_intern_atom_cookie_t * EWMHCookie = xcb_ewmh_init_atoms( m_Header.xcb_connection, &EWMH );
        // xcb_ewmh_init_atoms_replies(&EWMH, EWMHCookie, NULL);
        // xcb_change_property( m_Header.xcb_connection, XCB_PROP_MODE_REPLACE, m_Header.xcb_window, EWMH._NET_WM_STATE, XCB_ATOM, 32, 1, &(EWMH._NET_WM_STATE_FULLSCREEN) ); 

        ::xcb_map_window( m_Header.xcb_connection, m_Header.xcb_window );
        ::xcb_flush( m_Header.xcb_connection );
        
        return false;
    }

    bool window::init( void )
    {
        CALL( object::init() );

        m_Header.xcb_connection = nullptr;
        m_Header.xcb_screen = nullptr;

        return false;
    }

    bool window::run( void )
    {
        m_Run = true;
        xcb_generic_event_t * t_event;
        
        while ( (t_event = ::xcb_wait_for_event ( m_Header.xcb_connection )) && m_Run )
        {

            switch ( t_event->response_type & ~0x80 ) {
            case XCB_EXPOSE: {
                xcb_expose_event_t *expose = (xcb_expose_event_t *)t_event;

                LOG.info ("Window {0} exposed. Region to be redrawn at location ({1}, {2}), with dimension ({3}, {4})",
                        expose->window, expose->x, expose->y, expose->width, expose->height );
                break;
            }
            case XCB_BUTTON_PRESS: {
                xcb_button_press_event_t *t_bp = (xcb_button_press_event_t *)t_event;
                print_modifiers (t_bp->state);

                switch (t_bp->detail) {
                case 4:
                    LOG.info ("Wheel Button up in window {0}, at coordinates ({1}, {2})",
                            t_bp->event, t_bp->event_x, t_bp->event_y );
                    break;
                case 5:
                    LOG.info ("Wheel Button down in window {0}, at coordinates ({1}, {2})",
                            t_bp->event, t_bp->event_x, t_bp->event_y );
                    break;
                default:
                    LOG.info ("Button {0} pressed in window {1}, at coordinates ({2}, {3})",
                            t_bp->detail, t_bp->event, t_bp->event_x, t_bp->event_y );
                    break;
                }
                break;
            }
            case XCB_BUTTON_RELEASE: {
                xcb_button_release_event_t * t_br = (xcb_button_release_event_t *)t_event;
                print_modifiers(t_br->state);

                LOG.info ("Button {0} released in window {1}, at coordinates ({2}, {3})",
                        t_br->detail, t_br->event, t_br->event_x, t_br->event_y );
                break;
            }
            case XCB_MOTION_NOTIFY: {
                xcb_motion_notify_event_t * t_motion = (xcb_motion_notify_event_t *)t_event;

                LOG.info ("Mouse moved in window {0}, at coordinates ({1}, {2})",
                        t_motion->event, t_motion->event_x, t_motion->event_y );
                break;
            }
            case XCB_ENTER_NOTIFY: {
                xcb_enter_notify_event_t * t_enter = (xcb_enter_notify_event_t *)t_event;

                LOG.info ("Mouse entered window {0}, at coordinates ({1}, {2})",
                        t_enter->event, t_enter->event_x, t_enter->event_y );
                break;
            }
            case XCB_LEAVE_NOTIFY: {
                xcb_leave_notify_event_t * t_leave = (xcb_leave_notify_event_t *)t_event;

                LOG.info ("Mouse left window {0}, at coordinates ({1}, {2})",
                        t_leave->event, t_leave->event_x, t_leave->event_y );
                break;
            }
            case XCB_KEY_PRESS: {
                xcb_key_press_event_t * t_kp = (xcb_key_press_event_t *)t_event;
                print_modifiers( t_kp->state );

                LOG.info ("Key pressed in window {0}", t_kp->event);
                break;
            }
            case XCB_KEY_RELEASE: {
                xcb_key_release_event_t * t_kr = (xcb_key_release_event_t *)t_event;
                print_modifiers( t_kr->state );

                LOG.info ("Key released in window {0}", t_kr->event);
                break;
            }
            default:
                /* Unknown event type, ignore it */
                LOG.info ("Unknown event: {0}", t_event->response_type);
                break;
            }

            free ( t_event );

            usleep ( 10000 );
        }
    }

    bool window::destroy( void )
    {
        CALL( object::destroy() );

        if( m_Header.xcb_connection )
        {
            ::xcb_disconnect( m_Header.xcb_connection );
            m_Header.xcb_connection = nullptr;
        }
            
        return false;
    }
}