#include "window.h"
#include "tools/log.hpp"


namespace ROOT_NAMESPACE
{

    window & window::Create( const std::string & p_title, const glm::vec2 & p_size, const bool p_fullScene, const bool p_centerInDesktop, const bool p_showCursor )
    {
        window & t_result = Create();
        
        if( t_result.init( p_title, p_size, glm::vec2(0.0f),  p_fullScene, p_centerInDesktop, p_showCursor ) )
        {
            LOG.error( "window init faile!" );
        }
        
        return t_result;
    }

    window & window::Create( const std::string & p_title, const glm::vec2 & p_size, const glm::vec2 & p_position, const bool p_showCursor )
    {
        window & t_result = Create();
        
        if( t_result.init( p_title, p_size, p_position, p_showCursor ) )
        {
            LOG.error( "window init faile!" );
        }
        
        return t_result;
    }
#ifdef OS_WINDOWS
    glm::vec2 window::GetSystemResolution( void )
    {
        glm::vec2 t_result = glm::vec2( 0.0f );

        RECT t_desktopRect;
        GetWindowRect( GetDesktopWindow(), &t_desktopRect );
        t_result = glm::vec2( t_desktopRect.right, t_desktopRect.bottom );
        return t_result;
    }

    const bool window::processEvent( void )
    {
        MSG t_msg;
        while( PeekMessage( &t_msg, NULL, 0, 0, PM_REMOVE ) > 0 )
        {
            if ( t_msg.message == WM_QUIT )
            {
                m_run = false;
            }
            else
            {
                TranslateMessage( &t_msg );
                DispatchMessage( &t_msg );
            }
        }

        m_input.keyInput[KEY_SHIFT_LEFT]    = GetAsyncKeyState( KEY_SHIFT_LEFT );
        m_input.keyInput[KEY_CTRL_LEFT]		= GetAsyncKeyState( KEY_CTRL_LEFT );
        m_input.keyInput[KEY_ALT_LEFT]		= GetAsyncKeyState( KEY_ALT_LEFT );
        m_input.keyInput[KEY_CURSOR_UP]		= GetAsyncKeyState( KEY_CURSOR_UP );
        m_input.keyInput[KEY_CURSOR_DOWN]   = GetAsyncKeyState( KEY_CURSOR_DOWN );
        m_input.keyInput[KEY_CURSOR_LEFT]	= GetAsyncKeyState( KEY_CURSOR_LEFT );
        m_input.keyInput[KEY_CURSOR_RIGHT]	= GetAsyncKeyState( KEY_CURSOR_RIGHT );

        return false;
    }
    
#endif

    bool window::run( void )
    {
        m_run = true;
#ifdef OS_WINDOWS
        while ( m_run )
        {
            if( processEvent() )
            {
                LOG.error( "error: process event" );
                break;
            }

            if( m_input.keyInput[KEY_ESCAPE] )
            {
                m_run = false;
                break;
            }

            if( m_input.keyInput[KEY_F] )
            {
                destory();
                
                m_fullScene = !m_fullScene;

                if( init( m_title, m_size, m_position, m_fullScene, m_centerInDesktop, m_showCursor ) )
                {
                    LOG.error("change fullScene reopen window falid!");
                    break;
                }
            }

            Sleep( 10 );

            consumeInput();
        }
#endif
        return false;
    }

    bool window::init( const std::string & p_title, const glm::vec2 & p_size, const glm::vec2 & p_position, const bool p_fullScene, const bool p_centerInDesktop, const bool p_showCursor )
    {
        m_fullScene = p_fullScene;
        m_centerInDesktop = p_centerInDesktop;
        m_showCursor = p_showCursor;

#ifdef OS_WINDOWS
        m_header.hInstance = GetModuleHandle(NULL); 

        DWORD       t_dwExStyle;                    // 扩展窗口风格
        DWORD       t_dwStyle;                      // 窗口风格
        RECT        t_WindowRect;                   // 取得矩形的左上角和右下角的坐标值
        const LPCSTR t_displayDevice = NULL;
        WNDCLASS t_wc;
        t_wc.lpszClassName = "window";                                  //注册类名  
        t_wc.lpfnWndProc = (WNDPROC)windowHeader::window_proc;          //窗口过程  
        t_wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;                //窗口风格  
        t_wc.hInstance = m_header.hInstance;                             //应用程序实例  
        t_wc.hIcon = LoadIcon(0,IDI_WINLOGO);                           //应用程序图标  
        t_wc.hCursor = LoadCursor(0,IDC_ARROW);                         //应用程序光标  
        t_wc.hbrBackground = NULL;                                      //窗口背景  
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
            memset( &t_dmScreenSettings, 0, sizeof(t_dmScreenSettings) );   // 确保内存清空为零
            t_dmScreenSettings.dmDisplayFlags = CDS_UPDATEREGISTRY;
            t_dmScreenSettings.dmDisplayFrequency = 120;
            t_dmScreenSettings.dmFields = DM_DISPLAYFLAGS | DM_DISPLAYFREQUENCY;

            if( ChangeDisplaySettingsExA( t_displayDevice, &t_dmScreenSettings, NULL, CDS_FULLSCREEN, NULL ) != DISP_CHANGE_SUCCESSFUL )
            {
                LOG.warning("try change display frequency is faild!");
            }
        }

        //尝试启用全屏模式
        if( m_fullScene )
        {                      
            memset( &t_dmScreenSettings, 0, sizeof(t_dmScreenSettings) );   // 确保内存清空为零
            t_dmScreenSettings.dmSize = sizeof( t_dmScreenSettings );       // Devmode 结构的大小
            t_dmScreenSettings.dmPelsWidth    = (int)p_size.x;              // 所选屏幕宽度
            t_dmScreenSettings.dmPelsHeight   = (int)p_size.y;              // 所选屏幕高度
            t_dmScreenSettings.dmBitsPerPel   = 32;                         // 每象素所选的色彩深度
            t_dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
            // 尝试设置显示模式并返回结果。注: CDS_FULLSCREEN 移去了状态条。
            if ( ChangeDisplaySettingsExA( t_displayDevice, &t_dmScreenSettings, NULL, CDS_FULLSCREEN, NULL ) != DISP_CHANGE_SUCCESSFUL )
            {
                LOG.warning("try fullscene is faild");
                m_fullScene = false;
            }
        }

        //获取当前系统刷新频率
        DEVMODEA t_lpDevMode;
        memset( &t_lpDevMode, 0, sizeof( DEVMODEA ) );
        t_lpDevMode.dmSize = sizeof( DEVMODEA );
        t_lpDevMode.dmDriverExtra = 0;

        if ( EnumDisplaySettingsA( t_displayDevice, ENUM_CURRENT_SETTINGS, &t_lpDevMode ) != FALSE )
        {
            LOG.debug( "windowRefreshRate: ", (float)t_lpDevMode.dmDisplayFrequency );
        }

        ShowCursor( m_showCursor );                                         // 隐藏或显示鼠标指针
        if( m_fullScene )
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

        AdjustWindowRectEx( &t_WindowRect, t_dwStyle, false, t_dwExStyle);  // 调整窗口达到真正要求的大小

        if( !m_fullScene && m_centerInDesktop )
        {
            glm::vec2 t_systemResolution = GetSystemResolution();
            const int t_offsetX = ( (int)t_systemResolution.x - ( t_WindowRect.right - t_WindowRect.left ) ) / 2 - t_WindowRect.left;
            const int t_offsetY = ( (int)t_systemResolution.y - ( t_WindowRect.bottom - t_WindowRect.top ) ) / 2 - t_WindowRect.top;

            t_WindowRect.left += t_offsetX;
            t_WindowRect.right += t_offsetX;
            t_WindowRect.top += t_offsetY;
            t_WindowRect.bottom += t_offsetY;
        }

        m_header.hWnd = CreateWindowEx(
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
        m_header.hInstance,                                                  //当前应用程序的实例句柄  
        NULL);                                                              //没有附加数据，为NULL

        if( !m_header.hWnd )
        {
            LOG.error( "Create Window Faile [{0}] !", GetLastError() );
            return true;
        }

        windowHeader::ms_windowList[m_header.hWnd] = (object *)this;

        SetWindowLongPtrA( m_header.hWnd, GWLP_USERDATA, (LONG_PTR) &m_header );
        m_header.hDC = GetDC( m_header.hWnd );
        if( !m_header.hDC )
        {
            LOG.error("Failed to acquire device context.");
            return true;
        }

        ShowWindow( m_header.hWnd, SW_SHOW | SW_NORMAL );
        SetForegroundWindow( m_header.hWnd );
        SetFocus( m_header.hWnd ); 
        // UpdateWindow( m_header.hWnd );

#endif
        
        return false;
    }

    window::window()
    {

    }

    window::~window()
    {

    }

    bool window::init( void )
    {
        if( object::init() ) return true;

        m_run = false;
        m_active = false;
        m_minimized = false;
        m_fullScene = false;
        m_centerInDesktop = true;
        m_showCursor = false;
        m_title = "";
        m_size = glm::vec2( 0.0f );
        m_position = glm::vec2( 0.0f );
        m_tick = NULL;

        memset( &m_input, 0, sizeof( m_input ) );

#ifdef OS_WINDOWS
        m_header.hInstance = NULL;
        m_header.hWnd = NULL;
        m_header.hDC = NULL;
#endif

        return false;
    }

    bool window::destory( void )
    {

#ifdef OS_WINDOWS
        if( m_fullScene )
        {
            ChangeDisplaySettingsA( NULL, 0 );
		    ShowCursor( TRUE );
        }

        if( m_header.hDC )
        {
            if ( !ReleaseDC( m_header.hWnd, m_header.hDC ) )
            {
                LOG.error( "Failed to release device context." );
            }
            m_header.hDC = NULL;
        }

        if ( m_header.hWnd )
        {

            std::map< HWND, object * >::iterator t_findRes = windowHeader::ms_windowList.find( m_header.hWnd );

            if( t_findRes != windowHeader::ms_windowList.end() && t_findRes->second == this ){
                windowHeader::ms_windowList.erase( t_findRes );
            }

            if ( !DestroyWindow( m_header.hWnd ) )
            {
                LOG.error( "Failed to destroy the window." );
            }
            m_header.hWnd = NULL;
        }

        if ( m_header.hInstance )
        {
            if ( !UnregisterClassA( "window", m_header.hInstance ) )
            {
                LOG.error( "Failed to unregister window class." );
            }
            m_header.hInstance = NULL;
        }
        
#endif
        return false;
    }

    const bool window::consumeInput( void )
    {
        memset( &m_input, 0, sizeof( m_input ) );
        return false;
    }

#ifdef OS_WINDOWS

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
                t_window->m_active = LOWORD( p_wParam ) != WA_INACTIVE;
                t_window->m_minimized = (bool)HIWORD( p_wParam );
                LOG.debug( "window active state changed: {0} {1}", t_window->m_active, t_window->m_minimized );
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
                t_window->m_size = glm::vec2( (int) LOWORD( p_lParam ), (int) HIWORD( p_lParam ) );
                LOG.debug( "window size changed: vec2({0}, {1})", t_window->m_size.x, t_window->m_size.y );
                return 0;
            }
            case WM_MOVE:
            {
                t_window->m_position = glm::vec2( (int)(short) LOWORD( p_lParam ), (int)(short) HIWORD( p_lParam ) );
                LOG.debug( "window position changed: vec2({0}, {1})", t_window->m_position.x, t_window->m_position.y );
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
                        t_window->m_input.keyInput[(int)p_wParam] = true;
                    }
                }
                break;
            }
            case WM_LBUTTONDOWN:
            {
                t_window->m_input.mouseInput[MOUSE_LEFT] = true;
                t_window->m_input.mouseInputX[MOUSE_LEFT] = LOWORD( p_lParam );
                t_window->m_input.mouseInputY[MOUSE_LEFT] = (int)t_window->m_size.y - HIWORD( p_lParam );
                break;
            }
            case WM_RBUTTONDOWN:
            {
                t_window->m_input.mouseInput[MOUSE_RIGHT] = true;
                t_window->m_input.mouseInputX[MOUSE_RIGHT] = LOWORD( p_lParam );
                t_window->m_input.mouseInputY[MOUSE_RIGHT] = (int)t_window->m_size.y - HIWORD( p_lParam );
                break;
            }
            case WM_MOUSEMOVE:
            {
                LOG.info( "mouse move: ", LOWORD( p_lParam ), ",", HIWORD( p_lParam ) );
                t_window->m_cursorPosition = glm::vec2( LOWORD( p_lParam ), HIWORD( p_lParam ) );
                break;
            }
        }

        return ::DefWindowProcA( p_hWnd, p_msg, p_wParam, p_lParam);  
    }
#endif
}