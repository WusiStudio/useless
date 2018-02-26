#pragma once
#ifndef __WINDOWS_H__
#define __WINDOWS_H__

#include "imemory/object.hpp"
#include "glm/glm.hpp"
#include <string>
#include <map>
#include <functional>

#include "windowHeader.h"
#include "xgraphical.h"

namespace ROOT_NAMESPACE
{
    class window : public object
    {
#ifdef OS_WINDOWS
        friend LRESULT CALLBACK windowHeader::window_proc(HWND p_hWnd,UINT p_msg,WPARAM p_wParam,LPARAM p_lParam);
#endif
    public:

        CREATEFUNC(window);

        static window & Create( const std::string & p_title, const glm::ivec2 & p_size, const bool p_fullScreenState = false, const bool p_centerInDesktop = true, const bool p_showCursor = false );
        static window & Create( const std::string & p_title, const glm::ivec2 & p_size, const glm::ivec2 & p_position, const bool p_showCursor = true );
        
        glm::ivec2 GetSystemResolution( void );

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

        xgraphical * m_xgraphical;
    };
}

#endif //__WINDOWS_H__