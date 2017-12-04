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
    struct windowHeader
    {
    public:
        HINSTANCE instance;  
        HWND      hwnd;
        HDC       hDC;
        static object * findWindow( HWND p_hWnd );
        static LRESULT CALLBACK window_proc(HWND p_hWnd,UINT p_msg,WPARAM p_wParam,LPARAM p_lParam);
        static std::map< HWND, object * > ms_windowList;
    };

#endif

    class window : public object
    {
        friend LRESULT CALLBACK windowHeader::window_proc(HWND p_hWnd,UINT p_msg,WPARAM p_wParam,LPARAM p_lParam);
        CREATEFUNC(window);
    public:
        static window & Create( const std::string & p_title, const glm::vec2 & p_size, const glm::vec2 & p_position );

        bool run( void );
    protected:
        window();
        virtual ~window();

        virtual bool init( void ) override;
        virtual bool init( const std::string & p_title, const glm::vec2 & p_size, const glm::vec2 & p_position );
    private:
        std::string m_title;
        glm::vec2 m_size;
        glm::vec2 m_position;
        std::function<bool(void)> m_tick;
        bool m_run;
        bool m_active;
        bool m_minimized;
        windowHeader m_header;
    };
}

#endif //__WINDOWS_H__