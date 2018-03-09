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

        ATTIRBUTE_R( bool, Activate );
        ATTIRBUTE_R( std::string, Title );
        ATTIRBUTE_R( glm::ivec2, Size );
        ATTIRBUTE_R( glm::ivec2, Position );
        ATTIRBUTE_R( glm::ivec2, CursorPosition );
        ATTIRBUTE_R( bool, FullScreenState );
        ATTIRBUTE_R( bool, ShowCursor );

        EVENT( std::function< void( const float p_tm ) >, Tick );

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

    protected:
        window();
        virtual ~window();

        virtual bool init( void ) override;
        virtual bool init( const std::string & p_title, const glm::ivec2 & p_size, const glm::ivec2 & p_position, const bool p_fullScreenState = false, const bool p_centerInDesktop = false, const bool p_showCursor = true );


        virtual bool destroy( void ) override;

    private:

        const bool processEvent( void );
        const bool consumeInput( void );

        bool m_Run;
        bool m_Active;
        bool m_Minimized;
        bool m_CenterInDesktop;

        glm::ivec2 m_fullscreenAfterSize;

        windowHeader m_Header;
        windowInput m_Input;
        xgraphical * m_Xgraphical;
    };
}

#endif //__WINDOWS_H__