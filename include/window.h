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
        friend LRESULT CALLBACK windowHeader::window_proc(HWND p_hWnd,UINT p_msg,WPARAM p_wParam,LPARAM p_lParam);
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

        virtual bool destory( void ) override;
        
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