#pragma once
#ifndef __WINDOWS_H__
#define __WINDOWS_H__

#include "imemory/object.hpp"
#include "glm/glm.hpp"
#include <string>
#include <map>
#include <functional>

#ifdef OS_WINDOWS
#include <windows.h>
#elif defined OS_LINUX
#include <X11/Xlib.h>
#include <unistd.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
// #include <X11/extensions/xf86vmode.h>	// for fullscreen video mode
// #include <X11/extensions/Xrandr.h>		// for resolution changes
#endif

namespace ROOT_NAMESPACE
{
#ifdef OS_WINDOWS
    typedef struct
    {
    public:
        HINSTANCE hInstance;  
        HWND      hWnd;
        HDC       hDC;
        static object * findWindow( HWND p_hWnd );
        static LRESULT CALLBACK window_proc(HWND p_hWnd,UINT p_msg,WPARAM p_wParam,LPARAM p_lParam);
        static std::map< HWND, object * > ms_windowList;
    }windowHeader;

    typedef enum
    {
        KEY_A				= 0x41,
        KEY_B				= 0x42,
        KEY_C				= 0x43,
        KEY_D				= 0x44,
        KEY_E				= 0x45,
        KEY_F				= 0x46,
        KEY_G				= 0x47,
        KEY_H				= 0x48,
        KEY_I				= 0x49,
        KEY_J				= 0x4A,
        KEY_K				= 0x4B,
        KEY_L				= 0x4C,
        KEY_M				= 0x4D,
        KEY_N				= 0x4E,
        KEY_O				= 0x4F,
        KEY_P				= 0x50,
        KEY_Q				= 0x51,
        KEY_R				= 0x52,
        KEY_S				= 0x53,
        KEY_T				= 0x54,
        KEY_U				= 0x55,
        KEY_V				= 0x56,
        KEY_W				= 0x57,
        KEY_X				= 0x58,
        KEY_Y				= 0x59,
        KEY_Z				= 0x5A,
        KEY_RETURN			= VK_RETURN,
        KEY_TAB				= VK_TAB,
        KEY_ESCAPE			= VK_ESCAPE,
        KEY_SHIFT_LEFT		= VK_LSHIFT,
        KEY_CTRL_LEFT		= VK_LCONTROL,
        KEY_ALT_LEFT		= VK_LMENU,
        KEY_CURSOR_UP		= VK_UP,
        KEY_CURSOR_DOWN		= VK_DOWN,
        KEY_CURSOR_LEFT		= VK_LEFT,
        KEY_CURSOR_RIGHT	= VK_RIGHT
    } keyboardKey;

    typedef enum
    {
        MOUSE_LEFT		= 0,
        MOUSE_RIGHT		= 1
    } mouseButton;

#elif defined OS_LINUX

    typedef struct
    {
    public:
        Display * xDisplay;
        glm::int32 xScreen;
        Colormap xColormap;
        Visual * xVisual;
        Window xRoot, xWindow;
        Atom xDeleteWindowEvent;
    }windowHeader;

    typedef enum
    {
        KEY_A				= XK_a,
        KEY_B				= XK_b,
        KEY_C				= XK_c,
        KEY_D				= XK_d,
        KEY_E				= XK_e,
        KEY_F				= XK_f,
        KEY_G				= XK_g,
        KEY_H				= XK_h,
        KEY_I				= XK_i,
        KEY_J				= XK_j,
        KEY_K				= XK_k,
        KEY_L				= XK_l,
        KEY_M				= XK_m,
        KEY_N				= XK_n,
        KEY_O				= XK_o,
        KEY_P				= XK_p,
        KEY_Q				= XK_q,
        KEY_R				= XK_r,
        KEY_S				= XK_s,
        KEY_T				= XK_t,
        KEY_U				= XK_u,
        KEY_V				= XK_v,
        KEY_W				= XK_w,
        KEY_X				= XK_x,
        KEY_Y				= XK_y,
        KEY_Z				= XK_z,
        KEY_RETURN			= ( XK_Return & 0xFF ),
        KEY_TAB				= ( XK_Tab & 0xFF ),
        KEY_ESCAPE			= ( XK_Escape & 0xFF ),
        KEY_SHIFT_LEFT		= ( XK_Shift_L & 0xFF ),
        KEY_CTRL_LEFT		= ( XK_Control_L & 0xFF ),
        KEY_ALT_LEFT		= ( XK_Alt_L & 0xFF ),
        KEY_CURSOR_UP		= ( XK_Up & 0xFF ),
        KEY_CURSOR_DOWN		= ( XK_Down & 0xFF ),
        KEY_CURSOR_LEFT		= ( XK_Left & 0xFF ),
        KEY_CURSOR_RIGHT	= ( XK_Right & 0xFF )
    } keyboardKey;

    typedef enum
    {
        MOUSE_LEFT		= 0,
        MOUSE_RIGHT		= 1
    } mouseButton;

#endif

    typedef struct
    {
        bool    keyInput[256];
        bool    mouseInput[8];
        glm::int32	    mouseInputX[8];
	    glm::int32	    mouseInputY[8];
    }windowInput;

    class window : public object
    {
#ifdef OS_WINDOWS
        friend LRESULT CALLBACK windowHeader::window_proc(HWND p_hWnd,UINT p_msg,WPARAM p_wParam,LPARAM p_lParam);
#endif
        CREATEFUNC(window);
    public:
        static window & Create( const std::string & p_title, const glm::ivec2 & p_size, const bool p_fullScreenState = false, const bool p_centerInDesktop = true, const bool p_showCursor = false );
        static window & Create( const std::string & p_title, const glm::ivec2 & p_size, const glm::ivec2 & p_position, const bool p_showCursor = true );
        static glm::ivec2 GetSystemResolution( void );

        bool run( void );
        bool setTitle( const std::string & p_title );
        bool setSize( const glm::ivec2 & p_size );
        bool setPosition( const glm::ivec2 & p_position );
        bool setCursorPos( const glm::ivec2 & p_cursorPos );
        bool setFullScreenState( const bool p_fullScreenState );

        const std::string & getTitle( void ) const;
        const glm::ivec2 & getSize( void ) const;
        const glm::ivec2 & getPosition( void ) const;
        const glm::ivec2 & getCursorPosition( void ) const;
        bool getFullScreenState( void ) const;


    protected:
        window();
        virtual ~window();

        virtual bool init( void ) override;
        virtual bool init( const std::string & p_title, const glm::ivec2 & p_size, const glm::ivec2 & p_position, const bool p_fullScreenState = false, const bool p_centerInDesktop = false, const bool p_showCursor = true );

        virtual bool destroy( void ) override;
        
    private:

        const bool processEvent( void );
        const bool consumeInput( void );
    
        std::string m_title;
        glm::ivec2 m_size;
        glm::ivec2 m_position;
        glm::ivec2 m_cursorPosition;

        bool m_run;
        bool m_active;
        bool m_minimized;
        bool m_fullScreenState;
        bool m_centerInDesktop;
        bool m_showCursor;
        windowHeader m_header;
        windowInput m_input;
    };
}

#endif //__WINDOWS_H__