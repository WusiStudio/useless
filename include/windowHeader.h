#pragma once
#ifndef __WINDOWS_Header_H__
#define __WINDOWS_Header_H__

#ifdef OS_WINDOWS
#include <windows.h>
#elif defined OS_LINUX
#include <xcb/xcb.h>
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
       
        glm::ivec2 xDesktopSize, xWindowSize;
        float xDesktopRefreshRate, xWindowRefreshRate;
        
    }windowHeader;

    typedef enum
    {
        KEY_A				,//= XK_a,
        KEY_B				,//= XK_b,
        KEY_C				,//= XK_c,
        KEY_D				,//= XK_d,
        KEY_E				,//= XK_e,
        KEY_F				,//= XK_f,
        KEY_G				,//= XK_g,
        KEY_H				,//= XK_h,
        KEY_I				,//= XK_i,
        KEY_J				,//= XK_j,
        KEY_K				,//= XK_k,
        KEY_L				,//= XK_l,
        KEY_M				,//= XK_m,
        KEY_N				,//= XK_n,
        KEY_O				,//= XK_o,
        KEY_P				,//= XK_p,
        KEY_Q				,//= XK_q,
        KEY_R				,//= XK_r,
        KEY_S				,//= XK_s,
        KEY_T				,//= XK_t,
        KEY_U				,//= XK_u,
        KEY_V				,//= XK_v,
        KEY_W				,//= XK_w,
        KEY_X				,//= XK_x,
        KEY_Y				,//= XK_y,
        KEY_Z				,//= XK_z,
        KEY_RETURN			,//= ( XK_Return & 0xFF ),
        KEY_TAB				,//= ( XK_Tab & 0xFF ),
        KEY_ESCAPE			,//= ( XK_Escape & 0xFF ),
        KEY_SHIFT_LEFT		,//= ( XK_Shift_L & 0xFF ),
        KEY_CTRL_LEFT		,//= ( XK_Control_L & 0xFF ),
        KEY_ALT_LEFT		,//= ( XK_Alt_L & 0xFF ),
        KEY_CURSOR_UP		,//= ( XK_Up & 0xFF ),
        KEY_CURSOR_DOWN		,//= ( XK_Down & 0xFF ),
        KEY_CURSOR_LEFT		,//= ( XK_Left & 0xFF ),
        KEY_CURSOR_RIGHT	//= ( XK_Right & 0xFF )
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
        int	    mouseInputX[8];
        int	    mouseInputY[8];
    }windowInput;

}

#endif //__WINDOWS_Header_H__