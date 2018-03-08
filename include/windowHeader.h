#pragma once
#ifndef __WINDOWS_Header_H__
#define __WINDOWS_Header_H__

#ifdef OS_WINDOWS
#include <windows.h>
#elif defined OS_LINUX
#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif

#include <map>
#include "imemory/object.hpp"
#include "glm/vec2.hpp"

namespace ROOT_NAMESPACE
{
#ifdef OS_WINDOWS
    typedef struct
    {
    public:
        HINSTANCE hInstance;  
        HWND      hWnd;
        HDC       hDC;
        HGLRC     hRC;
        LONG      hStyle;
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
        xcb_connection_t * xcb_connection;
        xcb_screen_t  * xcb_screen;
        xcb_window_t xcb_window;
        xcb_void_cookie_t xcb_cookie;

        Display * x_display;
        int x_default_screen;

        GLXFBConfig x_fb_config;

        xcb_atom_t xcb_atom_wm_protocals, xcb_atom_delete_window;
       
        // glm::ivec2 xDesktopSize, xWindowSize;
        // float xDesktopRefreshRate, xWindowRefreshRate;
        
    }windowHeader;

    typedef enum
    {
        KEY_A				= 0x26,
        KEY_B				= 0x38,
        KEY_C				= 0x36,
        KEY_D				= 0x28,
        KEY_E				= 0x1A,
        KEY_F				= 0x29,
        KEY_G				= 0x2A,
        KEY_H				= 0x2B,
        KEY_I				= 0x1F,
        KEY_J				= 0x2C,
        KEY_K				= 0x2D,
        KEY_L				= 0x2E,
        KEY_M				= 0x3A,
        KEY_N				= 0x39,
        KEY_O				= 0x20,
        KEY_P				= 0x21,
        KEY_Q				= 0x18,
        KEY_R				= 0x1B,
        KEY_S				= 0x27,
        KEY_T				= 0x1C,
        KEY_U				= 0x1E,
        KEY_V				= 0x37,
        KEY_W				= 0x19,
        KEY_X				= 0x35,
        KEY_Y				= 0x1D,
        KEY_Z				= 0x34,
        KEY_RETURN			= 0x24,
        KEY_TAB				= 0x17,
        KEY_ESCAPE			= 0x9,
        KEY_SHIFT_LEFT		= 0x32,
        KEY_CTRL_LEFT		= 0x25,
        KEY_ALT_LEFT		= 0x40,
        KEY_CURSOR_UP		= 0x62,
        KEY_CURSOR_DOWN		= 0x68,
        KEY_CURSOR_LEFT		= 0x64,
        KEY_CURSOR_RIGHT	= 0x66
    } keyboardKey;

    typedef enum
    {
        MOUSE_LEFT		= 1,
        MOUSE_CENTER    = 2,
        MOUSE_RIGHT		= 3,
        MOUSE_UP        = 4,
        MOUSE_DOWN      = 5
    } mouseButton;


#endif

    typedef struct
    {
        bool    keyInput[256];
        bool    mouseInput[8];
        int	    mouseInputX[8];
        int	    mouseInputY[8];
    }windowInput;

}

#endif //__WINDOWS_Header_H__