#include "window.h"
#include "tools/log.hpp"
#include "xgraphical.h"

namespace ROOT_NAMESPACE
{

    glm::ivec2 window::GetSystemResolution( void )
    {
        glm::ivec2 t_result = glm::ivec2( 0 );

        RECT t_desktopRect;
        GetWindowRect( GetDesktopWindow(), &t_desktopRect );
        t_result = glm::ivec2( t_desktopRect.right, t_desktopRect.bottom );
        return t_result;
    }

    bool window::setTitle( const std::string & p_title )
    {
        if( ::SetWindowTextA( m_Header.hWnd, TEXT( p_title.c_str() ) ) )
        {
            m_Title = p_title;
            return false;
        }

        LOG.error( "set window text faild!" );
        return true;
    }
    bool window::setSize( const glm::ivec2 & p_size )
    {
        DWORD       t_dwExStyle;                    // 扩展窗口风格
        DWORD       t_dwStyle;                      // 窗口风格
        RECT        t_WindowRect;                   // 窗口显示区域

        t_dwExStyle = ::GetWindowLong ( m_Header.hWnd, GWL_EXSTYLE );
        t_dwStyle = ::GetWindowLong( m_Header.hWnd, GWL_STYLE ); 

        t_WindowRect.left = 0;
        t_WindowRect.right = p_size.x;
        t_WindowRect.top = 0;
        t_WindowRect.bottom = p_size.y;

        AdjustWindowRectEx( &t_WindowRect, t_dwStyle, false, t_dwExStyle);  // 调整窗口达到真正要求的大小

        if( ::SetWindowPos( m_Header.hWnd, HWND_TOP, 0, 0, t_WindowRect.right - t_WindowRect.left, t_WindowRect.bottom - t_WindowRect.top, SWP_NOMOVE | SWP_NOREDRAW  ) )
        {
            return false;
        }

        LOG.error( "set window size faild!" );
        return true;
    }
    bool window::setPosition( const glm::ivec2 & p_position )
    {

        DWORD       t_dwExStyle;                    // 扩展窗口风格
        DWORD       t_dwStyle;                      // 窗口风格
        RECT        t_WindowRect;                   // 窗口显示区域

        t_dwExStyle = ::GetWindowLong( m_Header.hWnd, GWL_EXSTYLE );
        t_dwStyle = ::GetWindowLong( m_Header.hWnd, GWL_STYLE ); 

        t_WindowRect.left = p_position.x;
        t_WindowRect.right = 0;
        t_WindowRect.top = p_position.y;
        t_WindowRect.bottom = 0;

        AdjustWindowRectEx( &t_WindowRect, t_dwStyle, false, t_dwExStyle);  // 调整窗口达到真正要求的大小

        if( ::SetWindowPos( m_Header.hWnd, NULL, p_position.x, p_position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER  ) )
        {
            return false;
        }

        LOG.error( "set window pos faild!" );
        return true;
    }

    bool window::setCursorPos( const glm::ivec2 & p_cursorPos )
    {
        if( ::SetCursorPos( p_cursorPos.x, p_cursorPos.y ) )
        {
            return false;
        }

        LOG.error( "set cursor pos faild!" );
        return true;
    }

    const bool window::processEvent( void )
    {
        MSG t_msg;
        while( PeekMessage( &t_msg, NULL, 0, 0, PM_REMOVE ) > 0 )
        {
            if ( t_msg.message == WM_QUIT )
            {
                m_Run = false;
            }
            else
            {
                TranslateMessage( &t_msg );
                DispatchMessage( &t_msg );
            }
        }

        m_Input.keyInput[KEY_SHIFT_LEFT]    = GetAsyncKeyState( KEY_SHIFT_LEFT );
        m_Input.keyInput[KEY_CTRL_LEFT]		= GetAsyncKeyState( KEY_CTRL_LEFT );
        m_Input.keyInput[KEY_ALT_LEFT]		= GetAsyncKeyState( KEY_ALT_LEFT );
        m_Input.keyInput[KEY_CURSOR_UP]		= GetAsyncKeyState( KEY_CURSOR_UP );
        m_Input.keyInput[KEY_CURSOR_DOWN]   = GetAsyncKeyState( KEY_CURSOR_DOWN );
        m_Input.keyInput[KEY_CURSOR_LEFT]	= GetAsyncKeyState( KEY_CURSOR_LEFT );
        m_Input.keyInput[KEY_CURSOR_RIGHT]	= GetAsyncKeyState( KEY_CURSOR_RIGHT );

        return false;
    }

    bool window::setFullScreenState( const bool p_fullScreenState )
    {
        if( p_fullScreenState == m_FullScreenState )
        {
            return false;
        }

        if ( p_fullScreenState )
        {
            DEVMODE t_dmScreenSettings;                                         // 设备模式
            memset ( &t_dmScreenSettings, 0, sizeof ( t_dmScreenSettings ) );   // 确保内存清空为零
            t_dmScreenSettings.dmSize = sizeof ( t_dmScreenSettings );       // Devmode 结构的大小
            t_dmScreenSettings.dmPelsWidth = (int)m_Size.x;              // 所选屏幕宽度
            t_dmScreenSettings.dmPelsHeight = (int)m_Size.y;              // 所选屏幕高度
            t_dmScreenSettings.dmBitsPerPel = 32;                         // 每象素所选的色彩深度
            t_dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
            // 尝试设置显示模式并返回结果。注: CDS_FULLSCREEN 移去了状态条。

            if ( ChangeDisplaySettingsExA ( NULL, &t_dmScreenSettings, NULL, CDS_FULLSCREEN, NULL ) != DISP_CHANGE_SUCCESSFUL )
            {
                LOG.warning ( "try fullscene is faild" );
                m_FullScreenState = false;
                return false;
            }

            m_Header.hStyle = GetWindowLong ( m_Header.hWnd, GWL_STYLE );
            SetWindowLong ( m_Header.hWnd, GWL_STYLE, WS_BORDER );

            SetWindowPos ( m_Header.hWnd, HWND_TOPMOST, 0, 0, m_Size.x, m_Size.y, SWP_SHOWWINDOW );

            ShowWindow ( m_Header.hWnd, SW_MAXIMIZE );

            UpdateWindow ( m_Header.hWnd );
        }
        else {
            
            if (ChangeDisplaySettingsExA ( NULL, NULL, NULL, CDS_FULLSCREEN, NULL ) != DISP_CHANGE_SUCCESSFUL )
            {
                LOG.warning ( "try exit fullscene is faild" );
                m_FullScreenState = false;
                return false;
            }

            SetWindowLong ( m_Header.hWnd, GWL_STYLE, m_Header.hStyle );

            ShowWindow ( m_Header.hWnd, SW_SHOW );

            UpdateWindow ( m_Header.hWnd );


            setSize( m_Size );
            setPosition( ( GetSystemResolution() - m_Size ) / 2 );

        }

        m_FullScreenState = p_fullScreenState;

        return false;
    }

    bool window::init( const std::string & p_title, const glm::ivec2 & p_size, const glm::ivec2 & p_position, const bool p_fullScreenState, const bool p_centerInDesktop, const bool p_showCursor )
    {
        m_FullScreenState = p_fullScreenState;
        m_CenterInDesktop = p_centerInDesktop;
        m_ShowCursor = p_showCursor;

        m_Header.hInstance = GetModuleHandle( NULL ); 

        DWORD       t_dwExStyle;                    // 扩展窗口风格
        DWORD       t_dwStyle;                      // 窗口风格
        RECT        t_WindowRect;                   // 取得矩形的左上角和右下角的坐标值
        const LPCSTR t_displayDevice = NULL;
        WNDCLASS t_wc;
        t_wc.lpszClassName = "window";                                  //注册类名  
        t_wc.lpfnWndProc = (WNDPROC)windowHeader::window_proc;          //窗口过程  
        t_wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;                //窗口风格  
        t_wc.hInstance = m_Header.hInstance;                             //应用程序实例  
        t_wc.hIcon = LoadIcon(0,IDI_WINLOGO);                           //应用程序图标  
        t_wc.hCursor = LoadCursor(0,IDC_ARROW);                         //应用程序光标  
        t_wc.hbrBackground = (HBRUSH)GetStockObject ( BLACK_BRUSH );    //窗口背景  
        t_wc.lpszMenuName = NULL;                                       //窗口菜单名        
        t_wc.cbClsExtra = 0;                                            //默认  
        t_wc.cbWndExtra = 0;                                            //默认  


        t_WindowRect.left = (long)p_position.x;
        t_WindowRect.right = (long)( p_size.x + p_position.x );
        t_WindowRect.top = (long)p_position.y;
        t_WindowRect.bottom = (long)( p_size.y + p_position.y );

        if( !RegisterClass( &t_wc ) )  
        {  
            LOG.error( "Register Window Faile!" );
            return true;
        }

        DEVMODE t_dmScreenSettings;                                         // 设备模式

        // 尝试更改刷新率
        {
            memset( &t_dmScreenSettings, 0, sizeof( t_dmScreenSettings ) );   // 确保内存清空为零
            t_dmScreenSettings.dmDisplayFlags = CDS_UPDATEREGISTRY;
            t_dmScreenSettings.dmDisplayFrequency = 120;
            t_dmScreenSettings.dmFields = DM_DISPLAYFLAGS | DM_DISPLAYFREQUENCY;

            if( ChangeDisplaySettingsExA( t_displayDevice, &t_dmScreenSettings, NULL, NULL, NULL ) != DISP_CHANGE_SUCCESSFUL )
            {
                LOG.warning( "try change display frequency is faild!" );
            }
        }

        //尝试启用全屏模式
        if( m_FullScreenState )
        {                      
            memset( &t_dmScreenSettings, 0, sizeof( t_dmScreenSettings ) );   // 确保内存清空为零
            t_dmScreenSettings.dmSize = sizeof( t_dmScreenSettings );       // Devmode 结构的大小
            t_dmScreenSettings.dmPelsWidth    = (int)p_size.x;              // 所选屏幕宽度
            t_dmScreenSettings.dmPelsHeight   = (int)p_size.y;              // 所选屏幕高度
            t_dmScreenSettings.dmBitsPerPel   = 32;                         // 每象素所选的色彩深度
            t_dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
            // 尝试设置显示模式并返回结果。注: CDS_FULLSCREEN 移去了状态条。
            if ( ChangeDisplaySettingsExA( t_displayDevice, &t_dmScreenSettings, NULL, CDS_FULLSCREEN, NULL ) != DISP_CHANGE_SUCCESSFUL )
            {
                LOG.warning( "try fullscene is faild" );
                m_FullScreenState = false;
            }
        }

        //获取当前系统刷新频率
        DEVMODEA t_lpDevMode;
        memset( &t_lpDevMode, 0, sizeof( DEVMODEA ) );
        t_lpDevMode.dmSize = sizeof( DEVMODEA );
        t_lpDevMode.dmDriverExtra = 0;

        if ( EnumDisplaySettingsA( t_displayDevice, ENUM_CURRENT_SETTINGS, &t_lpDevMode ) != false )
        {
            LOG.debug( "windowRefreshRate: ", (float)t_lpDevMode.dmDisplayFrequency );
        }

        ShowCursor( m_ShowCursor );                                         // 隐藏或显示鼠标指针
        if( m_FullScreenState )
        {
            t_dwExStyle = WS_EX_APPWINDOW;                                  // 扩展窗体风格
            t_dwStyle = WS_POPUP;                                           // 窗体风格
        }   
        else    
        {   
            t_dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;               // 扩展窗体风格
            //  窗体风格
            t_dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        }

        AdjustWindowRectEx( &t_WindowRect, t_dwStyle, false, t_dwExStyle );  // 调整窗口达到真正要求的大小

        if( !m_FullScreenState && m_CenterInDesktop )
        {
            glm::ivec2 t_systemResolution = GetSystemResolution();
            const int t_offsetX = ( (int)t_systemResolution.x - ( t_WindowRect.right - t_WindowRect.left ) ) / 2 - t_WindowRect.left;
            const int t_offsetY = ( (int)t_systemResolution.y - ( t_WindowRect.bottom - t_WindowRect.top ) ) / 2 - t_WindowRect.top;

            t_WindowRect.left += t_offsetX;
            t_WindowRect.right += t_offsetX;
            t_WindowRect.top += t_offsetY;
            t_WindowRect.bottom += t_offsetY;
        }

        m_Header.hWnd = CreateWindowEx(
        t_dwExStyle,                                                        //扩展窗体风格
        "window" ,                                                          //类名，要和刚才注册的一致  
        p_title.c_str() ,                                                   //窗口标题文字  
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | t_dwStyle,                      //窗口外观样式  
        t_WindowRect.left,                                                  //窗口相对于父级的X坐标  
        t_WindowRect.top,                                                   //窗口相对于父级的Y坐标  
        t_WindowRect.right - t_WindowRect.left,                             //窗口的宽度  
        t_WindowRect.bottom - t_WindowRect.top,                             //窗口的高度  
        NULL,                                                               //没有父窗口，为NULL   
        NULL,                                                               //没有菜单，为NULL   
        m_Header.hInstance,                                                  //当前应用程序的实例句柄  
        NULL);                                                              //没有附加数据，为NULL

        if( !m_Header.hWnd )
        {
            LOG.error( "Create Window Faile [{0}] !", GetLastError() );
            return true;
        }

        windowHeader::ms_windowList[m_Header.hWnd] = (object *)this;

        SetWindowLongPtrA( m_Header.hWnd, GWLP_USERDATA, (LONG_PTR) &m_Header );
        m_Header.hDC = GetDC( m_Header.hWnd );
        if( !m_Header.hDC )
        {
            LOG.error("Failed to acquire device context.");
            return true;
        }

        ShowWindow( m_Header.hWnd, SW_SHOW | SW_NORMAL );
        SetForegroundWindow( m_Header.hWnd );
        SetFocus( m_Header.hWnd ); 
        // UpdateWindow( m_Header.hWnd );

        m_Xgraphical = &xgraphical::Create ( m_Header );
        
        return false;
    }

    bool window::init( void )
    {
        CALL( object::init() );

        m_Run = false;
        m_Active = false;
        m_Minimized = false;
        m_FullScreenState = false;
        m_CenterInDesktop = true;
        m_ShowCursor = false;
        m_Title = "";
        m_Size = glm::ivec2( 0 );
        m_Position = glm::ivec2( 0 );

        memset( &m_Input, 0, sizeof( m_Input ) );

        m_Header.hInstance = NULL;
        m_Header.hWnd = NULL;
        m_Header.hDC = NULL;

        return false;
    }

    bool window::run( void )
    {
        m_Run = true;
        
        while ( m_Run )
        {
            if( processEvent() )
            {
                break;
            }

            if( m_Input.keyInput[KEY_ESCAPE] )
            {
                m_Run = false;
                return false;
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

			Sleep ( 10 );

            consumeInput();
        }

        return false;
    }

    bool window::destroy( void )
    {
        CALL( object::destroy() );

        if( m_FullScreenState )
        {
            ChangeDisplaySettingsA( NULL, 0 );
		    ShowCursor( true );
        }

        if (m_Header.hRC)
        {
            wglDeleteContext ( m_Header.hRC );
        }

        if( m_Header.hDC )
        {
            if ( !ReleaseDC( m_Header.hWnd, m_Header.hDC ) )
            {
                LOG.error( "Failed to release device context." );
            }
            m_Header.hDC = NULL;
        }

        if ( m_Header.hWnd )
        {

            std::map< HWND, object * >::iterator t_findRes = windowHeader::ms_windowList.find( m_Header.hWnd );

            if( t_findRes != windowHeader::ms_windowList.end() && t_findRes->second == this ){
                windowHeader::ms_windowList.erase( t_findRes );
            }

            if ( !DestroyWindow( m_Header.hWnd ) )
            {
                LOG.error( "Failed to destroy the window." );
            }
            m_Header.hWnd = NULL;
        }

        if ( m_Header.hInstance )
        {
            if ( !UnregisterClassA( "window", m_Header.hInstance ) )
            {
                LOG.error( "Failed to unregister window class." );
            }
            m_Header.hInstance = NULL;
        }

        return false;
    }

    std::map< HWND, object * > windowHeader::ms_windowList;

    object * windowHeader::findWindow( HWND p_hWnd )
    {
        std::map< HWND, object * >::iterator t_result = ms_windowList.find( p_hWnd );
        if( t_result == ms_windowList.end() )
        {
            return NULL;
        }

        return t_result->second;
    }

    LRESULT CALLBACK windowHeader::window_proc(HWND p_hWnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam)
    {
        window * t_window = (window *)windowHeader::findWindow( p_hWnd );

        if ( t_window == NULL )
        {
            return ::DefWindowProcA( p_hWnd, p_msg, p_wParam, p_lParam);
        }
        switch( p_msg )  
        {
            case WM_ACTIVATE:
            {
				t_window->m_Active = LOWORD( p_wParam ) != WA_INACTIVE;
				t_window->m_Minimized = (bool)HIWORD( p_wParam );
                //LOG.debug( "window active state changed: {0} {1}", t_window->m_Active, t_window->m_Minimized );
                return 0;
            }  
            case WM_SYSCOMMAND:
            {
                switch ( p_wParam )                 // 检查系统调用
                {
                case SC_SCREENSAVE:                 // 屏保要运行
                case SC_MONITORPOWER:               // 显示器要进入节电模式
                    return 0;                       // 阻止发生
                }
                break;                              // 退出
            }
            case WM_CLOSE:                          // 收到Close消息?
            {
                ::PostQuitMessage(0);               // 发出退出消息
                return 0;                           // 返回
            }
            case WM_SIZE:
            {
				t_window->m_Size = glm::ivec2( (int) LOWORD( p_lParam ), (int) HIWORD( p_lParam ) );
                //LOG.debug( "window size changed: ivec2({0}, {1})", t_window->m_Size.x, t_window->m_Size.y );
                return 0;
            }
            case WM_MOVE:
            {
				t_window->m_Position = glm::ivec2( (int)(short) LOWORD( p_lParam ), (int)(short) HIWORD( p_lParam ) );
                //LOG.debug( "window position changed: ivec2({0}, {1})", t_window->m_Position.x, t_window->m_Position.y );
                return 0;
            }
            case WM_KEYDOWN:
            {
                if ( (int)p_wParam >= 0 && (int)p_wParam < 256 )
                {
                    if ( 	(int)p_wParam != KEY_SHIFT_LEFT &&
                            (int)p_wParam != KEY_CTRL_LEFT &&
                            (int)p_wParam != KEY_ALT_LEFT &&
                            (int)p_wParam != KEY_CURSOR_UP &&
                            (int)p_wParam != KEY_CURSOR_DOWN &&
                            (int)p_wParam != KEY_CURSOR_LEFT &&
                            (int)p_wParam != KEY_CURSOR_RIGHT )
                    {
						t_window->m_Input.keyInput[(int)p_wParam] = true;
                    }
                }
                break;
            }
            case WM_LBUTTONDOWN:
            {
				t_window->m_Input.mouseInput[MOUSE_LEFT] = true;
				t_window->m_Input.mouseInputX[MOUSE_LEFT] = LOWORD( p_lParam );
				t_window->m_Input.mouseInputY[MOUSE_LEFT] = (int)t_window->m_Size.y - HIWORD( p_lParam );
                break;
            }
            case WM_RBUTTONDOWN:
            {
				t_window->m_Input.mouseInput[MOUSE_RIGHT] = true;
				t_window->m_Input.mouseInputX[MOUSE_RIGHT] = LOWORD( p_lParam );
				t_window->m_Input.mouseInputY[MOUSE_RIGHT] = (int)t_window->m_Size.y - HIWORD( p_lParam );
                break;
            }
            case WM_MOUSEMOVE:
            {
                //LOG.info( "mouse move: ", LOWORD( p_lParam ), ",", HIWORD( p_lParam ) );
				t_window->m_CursorPosition = glm::vec2( LOWORD( p_lParam ), HIWORD( p_lParam ) );
                break;
            }
        }

        return ::DefWindowProcA( p_hWnd, p_msg, p_wParam, p_lParam);  
    }
    
}