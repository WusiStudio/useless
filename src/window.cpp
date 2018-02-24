#include "window.h"
#include "tools/log.hpp"
#include "xgraphical.h"


#include <GL\gl.h>                        //必要的头文件
#pragma comment(lib, "opengl32.lib")      //加入必要的lib文件


namespace ROOT_NAMESPACE
{

    window & window::Create( const std::string & p_title, const glm::ivec2 & p_size, const bool p_fullScreenState, const bool p_centerInDesktop, const bool p_showCursor )
    {
        window & t_result = Create();
        
        if( t_result.init( p_title, p_size, glm::ivec2( 0 ),  p_fullScreenState, p_centerInDesktop, p_showCursor ) )
        {
            LOG.error( "window init faile!" );
        }
        
        return t_result;
    }

    window & window::Create( const std::string & p_title, const glm::ivec2 & p_size, const glm::ivec2 & p_position, const bool p_showCursor )
    {
        window & t_result = Create();
        
        if( t_result.init( p_title, p_size, p_position, p_showCursor ) )
        {
            LOG.error( "window init faile!" );
        }
        
        return t_result;
    }
#ifdef OS_WINDOWS
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
        if( ::SetWindowTextA( m_header.hWnd, TEXT( p_title.c_str() ) ) )
        {
            m_title = p_title;
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

        t_dwExStyle = ::GetWindowLong( m_header.hWnd, GWL_EXSTYLE );
        t_dwStyle = ::GetWindowLong( m_header.hWnd, GWL_STYLE ); 

        t_WindowRect.left = 0;
        t_WindowRect.right = p_size.x;
        t_WindowRect.top = 0;
        t_WindowRect.bottom = p_size.y;

        AdjustWindowRectEx( &t_WindowRect, t_dwStyle, false, t_dwExStyle);  // 调整窗口达到真正要求的大小

        if( ::SetWindowPos( m_header.hWnd, HWND_TOP, 0, 0, t_WindowRect.right - t_WindowRect.left, t_WindowRect.bottom - t_WindowRect.top, SWP_NOMOVE | SWP_NOREDRAW  ) )
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

        t_dwExStyle = ::GetWindowLong( m_header.hWnd, GWL_EXSTYLE );
        t_dwStyle = ::GetWindowLong( m_header.hWnd, GWL_STYLE ); 

        t_WindowRect.left = p_position.x;
        t_WindowRect.right = 0;
        t_WindowRect.top = p_position.y;
        t_WindowRect.bottom = 0;

        AdjustWindowRectEx( &t_WindowRect, t_dwStyle, false, t_dwExStyle);  // 调整窗口达到真正要求的大小

        if( ::SetWindowPos( m_header.hWnd, NULL, p_position.x, p_position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER  ) )
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

#elif defined OS_LINUX
    glm::ivec2 window::GetSystemResolution( void )
    {
        return glm::ivec2( ::XDisplayWidth( m_header.xDisplay, m_header.xScreen ), ::XDisplayHeight( m_header.xDisplay, m_header.xScreen ) );
    }

    bool window::setTitle( const std::string & p_title )
    {

        LOG.info("---------------------");
        if( !(int)AtomWM_NAME )
        {
            AtomWM_NAME = ::XInternAtom( m_header.xDisplay, "WM_NAME", false );
        }
        
        LOG.info("---------------------");
        ::XChangeProperty( m_header.xDisplay, m_header.xWindow, AtomWM_NAME,
                            XA_STRING, 8, PropModeReplace,
                            (const unsigned char *)p_title.c_str(), p_title.size() );
        LOG.info("---------------------");
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
        int t_count = ::XPending( m_header.xDisplay );
        for ( int i = 0; i < t_count; i++ )
	    {
            XEvent t_event;
            KeySym t_key;
            ::XNextEvent( m_header.xDisplay, &t_event );

            switch( t_event.type )
            {
            case KeyPress:
                t_key = ::XLookupKeysym( &t_event.xkey, 0 );
				if ( t_key < 256 || t_key == XK_Escape )
				{
					m_input.keyInput[t_key & 255] = true;
				}
            break;
            case ButtonPress:
			{
				m_input.mouseInput[t_event.xbutton.button] = true;
				m_input.mouseInputX[t_event.xbutton.button] = t_event.xbutton.x;
				m_input.mouseInputY[t_event.xbutton.button] = t_event.xbutton.y;

                LOG.info( "mouse button click: \\{{0}, {1}\\}", t_event.xbutton.x, t_event.xbutton.y );
                break;
			}
            case DestroyNotify:
                m_run = false;
            return true;
            case ClientMessage:
                if( (Atom)t_event.xclient.data.l[0] == AtomDeleteWindowCmd )
                {
                    m_run = false;
                    return true;
                }
            break;
            }
        }
        return false;
    }
    
#endif

    bool window::setFullScreenState( const bool p_fullScreenState )
    {
        if( p_fullScreenState == m_fullScreenState )
        {
            return false;
        }

        //if( destroy() )
        //{
        //    LOG.error("falid to destroy window!");
        //    return true;
        //}

        //if( init( m_title, m_size, m_position, p_fullScreenState, m_centerInDesktop, m_showCursor ) )
        //{
        //    LOG.error("falid to change fullScene reopen window!");
        //    return true;
        //}

        if (p_fullScreenState)
        {
            DEVMODE t_dmScreenSettings;                                         // 设备模式
            memset ( &t_dmScreenSettings, 0, sizeof ( t_dmScreenSettings ) );   // 确保内存清空为零
            t_dmScreenSettings.dmSize = sizeof ( t_dmScreenSettings );       // Devmode 结构的大小
            t_dmScreenSettings.dmPelsWidth = (int)m_size.x;              // 所选屏幕宽度
            t_dmScreenSettings.dmPelsHeight = (int)m_size.y;              // 所选屏幕高度
            t_dmScreenSettings.dmBitsPerPel = 32;                         // 每象素所选的色彩深度
            t_dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
            // 尝试设置显示模式并返回结果。注: CDS_FULLSCREEN 移去了状态条。

            if (ChangeDisplaySettingsExA ( NULL, &t_dmScreenSettings, NULL, CDS_FULLSCREEN, NULL ) != DISP_CHANGE_SUCCESSFUL)
            {
                LOG.warning ( "try fullscene is faild" );
                m_fullScreenState = false;
                return false;
            }

            m_header.hStyle = GetWindowLong ( m_header.hWnd, GWL_STYLE );
            SetWindowLong ( m_header.hWnd, GWL_STYLE, WS_BORDER );

            SetWindowPos ( m_header.hWnd, HWND_TOPMOST, 0, 0, m_size.x, m_size.y, SWP_SHOWWINDOW );

            ShowWindow ( m_header.hWnd, SW_MAXIMIZE );

            UpdateWindow ( m_header.hWnd );
        }
        else {
            
            if (ChangeDisplaySettingsExA ( NULL, NULL, NULL, CDS_FULLSCREEN, NULL ) != DISP_CHANGE_SUCCESSFUL)
            {
                LOG.warning ( "try exit fullscene is faild" );
                m_fullScreenState = false;
                return false;
            }

            SetWindowLong ( m_header.hWnd, GWL_STYLE, m_header.hStyle );

            //SetWindowPos ( m_header.hWnd, HWND_TOPMOST, 100, 100, m_size.x, m_size.y, SWP_SHOWWINDOW );

            ShowWindow ( m_header.hWnd, SW_SHOW );

            UpdateWindow ( m_header.hWnd );

        }

        m_fullScreenState = p_fullScreenState;

        return false;
    }

    const std::string & window::getTitle( void ) const
    {
        return m_title;
    }
    const glm::ivec2 & window::getSize( void ) const
    {
        return m_size;
    }
    const glm::ivec2 & window::getPosition( void ) const
    {
        return m_position;
    }
    const glm::ivec2 & window::getCursorPosition( void ) const
    {
        return m_cursorPosition;
    }

    bool window::getFullScreenState( void ) const
    {
        return m_fullScreenState;
    }

    bool window::run( void )
    {
        m_run = true;
        
        while ( m_run )
        {
            if( processEvent() )
            {
                break;
            }

            if( m_input.keyInput[KEY_ESCAPE] )
            {
                m_run = false;
                return false;
            }

            if( m_input.keyInput[KEY_N] )
            {
                if( setTitle( "haha" ) )
                {
                    LOG.error( "setTitle faild" );
                }
            }

            if( m_input.keyInput[KEY_R] )
            {
                if( setSize( glm::ivec2( 800, 600 ) ) )
                {
                    LOG.error( "setSize faild" );
                }
            }

            if( m_input.keyInput[KEY_T] )
            {
                if( setSize( glm::ivec2( 1024, 768 ) ) )
                {
                    LOG.error( "setSize faild" );
                }
            }

            if( m_input.keyInput[KEY_F] )
            {
                setFullScreenState( !m_fullScreenState );
            }

            wglMakeCurrent ( m_header.hDC, m_header.hRC );

            glClearColor ( 1.0f, 0.0f, 0.0f, 1.0f );

            glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


            SwapBuffers ( m_header.hDC );

            wglMakeCurrent ( NULL, NULL );

#ifdef  OS_WINDOWS
			Sleep ( 10 );
#elif defined OS_LINUX
			usleep ( 10000 );
#endif //  OS_WINDOWS

            consumeInput();
        }

        return false;
    }

    bool window::init( const std::string & p_title, const glm::ivec2 & p_size, const glm::ivec2 & p_position, const bool p_fullScreenState, const bool p_centerInDesktop, const bool p_showCursor )
    {
        m_fullScreenState = p_fullScreenState;
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
            memset( &t_dmScreenSettings, 0, sizeof(t_dmScreenSettings) );   // 确保内存清空为零
            t_dmScreenSettings.dmDisplayFlags = CDS_UPDATEREGISTRY;
            t_dmScreenSettings.dmDisplayFrequency = 120;
            t_dmScreenSettings.dmFields = DM_DISPLAYFLAGS | DM_DISPLAYFREQUENCY;

            if( ChangeDisplaySettingsExA( t_displayDevice, &t_dmScreenSettings, NULL, NULL, NULL ) != DISP_CHANGE_SUCCESSFUL )
            {
                LOG.warning("try change display frequency is faild!");
            }
        }

        //尝试启用全屏模式
        if( m_fullScreenState )
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
                m_fullScreenState = false;
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

        ShowCursor( m_showCursor );                                         // 隐藏或显示鼠标指针
        if( m_fullScreenState )
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

        if( !m_fullScreenState && m_centerInDesktop )
        {
            glm::ivec2 t_systemResolution = GetSystemResolution();
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

        xgraphical::init ( m_header );

#elif defined OS_LINUX

        ::XSetErrorHandler( x_error_handler );

        const char * t_displayName = NULL;
        m_header.xDisplay = ::XOpenDisplay( t_displayName );
        if( !m_header.xDisplay )
        {
            LOG.error("Unable to open X Display.");
            return true;
        }

        m_header.xScreen = ::XDefaultScreen( m_header.xDisplay );
        m_header.xVisual = ::XDefaultVisual( m_header.xDisplay, m_header.xScreen );
        m_header.xRoot = ::XRootWindow( m_header.xDisplay, m_header.xScreen );


        LOG.debug( "m_header.xDisplay: {0}, m_header.xScreen: {1}, m_header.xVisual: {2}, m_header.xRoot: {3}", m_header.xDisplay, m_header.xScreen, m_header.xVisual, m_header.xRoot );

        m_header.xWindowSize = p_size;
        m_header.xWindowRefreshRate = 120.0f;
        if( m_fullScreenState )
        {
            ChangeVideoMode_XF86VidMode( m_header.xDisplay, m_header.xScreen, m_header.xRoot, 
                                            &m_header.xDesktopSize.x, &m_header.xDesktopSize.y, &m_header.xDesktopRefreshRate,
                                            &m_header.xWindowSize.x, &m_header.xWindowSize.y, &m_header.xWindowRefreshRate );
            m_header.xWindowSize = p_size;

            LOG.info("m_header.xWindowSize: \\{{0}, {1}\\}", m_header.xWindowSize.x, m_header.xWindowSize.y);
        }else{
            ChangeVideoMode_XF86VidMode( m_header.xDisplay, m_header.xScreen, m_header.xRoot, 
                                            &m_header.xDesktopSize.x, &m_header.xDesktopSize.y, &m_header.xDesktopRefreshRate,
									        NULL, NULL, NULL );

            m_header.xWindowRefreshRate = m_header.xDesktopRefreshRate;
        }

        m_header.xColormap = ::XCreateColormap( m_header.xDisplay, m_header.xRoot, m_header.xVisual, AllocNone );

        const unsigned long t_wamask = CWColormap | CWEventMask | ( p_fullScreenState ? 0 : CWBorderPixel );

        XSetWindowAttributes t_wa;
        memset( &t_wa, 0, sizeof( t_wa ) );
        t_wa.colormap = m_header.xColormap;
        t_wa.border_pixel = 0;
        t_wa.event_mask = StructureNotifyMask | PropertyChangeMask | ResizeRedirectMask |
                        KeyPressMask | KeyReleaseMask |
                        ButtonPressMask | ButtonReleaseMask |
                        FocusChangeMask | ExposureMask | VisibilityChangeMask |
                        EnterWindowMask | LeaveWindowMask;

        LOG.info("--------------------------------A");

        m_header.xWindow = ::XCreateWindow( 
                                            m_header.xDisplay,
                                            m_header.xRoot,
                                            p_position.x,
                                            p_position.y,
                                            m_header.xWindowSize.x,
                                            m_header.xWindowSize.y,
                                            0,
                                            ::XDefaultDepth( m_header.xDisplay, m_header.xScreen ),
                                            InputOutput,
                                            m_header.xVisual,
                                            t_wamask,
                                            &t_wa );
        if( !m_header.xWindow )
        {
            LOG.error( "Failed to create window." );
            return true;
        }


        LOG.debug("xWindow: ", m_header.xWindow );

        

        // Change the window title.
        setTitle( p_title );

        LOG.info("--------------------------------B");

        
        if( !(int)AtomDeleteWindowCmd )
        {
            AtomDeleteWindowCmd = ::XInternAtom( m_header.xDisplay, "WM_DELETE_WINDOW", true );
            ::XSetWMProtocols( m_header.xDisplay, m_header.xWindow, &AtomDeleteWindowCmd, 1 );
        }
        
        LOG.info("--------------------------------C");

        if( p_fullScreenState )
        {
            // Bypass the compositor in fullscreen mode.
            const unsigned long bypass = 1;
            if( !(int)Atom_NET_WM_BYPASS_COMPOSITOR )
            {
                
                Atom_NET_WM_BYPASS_COMPOSITOR = ::XInternAtom( m_header.xDisplay, "_NET_WM_BYPASS_COMPOSITOR", false );
            }
            ::XChangeProperty( m_header.xDisplay, m_header.xWindow, Atom_NET_WM_BYPASS_COMPOSITOR,
                                XA_CARDINAL, 32, PropModeReplace, (const unsigned char*)&bypass, 1 );

            LOG.info("--------------------------------D");

            // Completely dissasociate window from window manager.
            XSetWindowAttributes attributes;
            attributes.override_redirect = false;
            ::XChangeWindowAttributes( m_header.xDisplay, m_header.xWindow, CWOverrideRedirect, &attributes );


             LOG.info("--------------------------------E");

            // Make the window visible.
            ::XMapRaised( m_header.xDisplay, m_header.xWindow );
            ::XMoveResizeWindow( m_header.xDisplay, m_header.xWindow, 0, 0, m_header.xDesktopSize.x, m_header.xDesktopSize.y );
            ::XFlush( m_header.xDisplay );

             LOG.info("--------------------------------F");

            // Grab mouse and keyboard input now that the window is disassociated from the window manager.
            ::XGrabPointer( m_header.xDisplay, m_header.xWindow, true, 0, GrabModeAsync, GrabModeAsync, m_header.xWindow, 0L, CurrentTime );
            ::XGrabKeyboard( m_header.xDisplay, m_header.xWindow, true, GrabModeAsync, GrabModeAsync, CurrentTime );

             LOG.info("--------------------------------G");
        }else
        {
            XSizeHints * t_hints = ::XAllocSizeHints();
            t_hints->flags = ( PMinSize | PMaxSize );
            t_hints->min_width = m_header.xWindowSize.x;
            t_hints->max_width = m_header.xWindowSize.x;
            t_hints->min_height = m_header.xWindowSize.y;
            t_hints->max_height = m_header.xWindowSize.y;
            ::XSetWMNormalHints( m_header.xDisplay, m_header.xWindow, t_hints );
            ::XFree( t_hints );

            // First map the window and then center the window on the screen.
            ::XMapRaised( m_header.xDisplay, m_header.xWindow );

            int t_window_pos_x = p_position.x;
            int t_window_pos_y = p_position.y;

            if( p_centerInDesktop )
            {
                t_window_pos_x = ( m_header.xDesktopSize.x - m_header.xWindowSize.x ) / 2;
                t_window_pos_y = ( m_header.xDesktopSize.y - m_header.xWindowSize.y ) / 2;
            }
            ::XMoveResizeWindow( m_header.xDisplay, m_header.xWindow, t_window_pos_x, t_window_pos_y, m_header.xWindowSize.x, m_header.xWindowSize.y );
            ::XFlush( m_header.xDisplay );
        }

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
        m_fullScreenState = false;
        m_centerInDesktop = true;
        m_showCursor = false;
        m_title = "";
        m_size = glm::ivec2( 0 );
        m_position = glm::ivec2( 0 );

        memset( &m_input, 0, sizeof( m_input ) );

#ifdef OS_WINDOWS
        m_header.hInstance = NULL;
        m_header.hWnd = NULL;
        m_header.hDC = NULL;
#elif defined OS_LINUX
        m_header.xDisplay = NULL;
        m_header.xScreen = -1;
        m_header.xColormap = 0;
        m_header.xVisual = NULL;
        m_header.xRoot = 0;
        m_header.xWindow = 0;

#endif

        return false;
    }

    bool window::destroy( void )
    {

#ifdef OS_WINDOWS
        if( m_fullScreenState )
        {
            ChangeDisplaySettingsA( NULL, 0 );
		    ShowCursor( true );
        }

        if (m_header.hRC)
        {
            wglDeleteContext ( m_header.hRC );
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
#elif defined OS_LINUX

        if( m_fullScreenState )
        {
            ChangeVideoMode_XF86VidMode( m_header.xDisplay, m_header.xScreen, m_header.xRoot,
									NULL, NULL, NULL,
									&m_header.xDesktopSize.x, &m_header.xDesktopSize.y, &m_header.xDesktopRefreshRate );

            XUngrabPointer( m_header.xDisplay, CurrentTime );
            XUngrabKeyboard( m_header.xDisplay, CurrentTime );
        }

        if( m_header.xWindow )
        {
            ::XUnmapWindow( m_header.xDisplay, m_header.xWindow );
            ::XDestroyWindow( m_header.xDisplay, m_header.xWindow );
            m_header.xWindow = 0;
        }
        
        if ( m_header.xColormap )
        {
            ::XFreeColormap( m_header.xDisplay, m_header.xColormap );
            m_header.xColormap = 0;
        }

        if ( m_header.xVisual && m_header.xVisual != ::XDefaultVisual( m_header.xDisplay, m_header.xScreen ) )
        {
            ::XFree( m_header.xVisual );
            m_header.xVisual = NULL;
        }

        ::XFlush( m_header.xDisplay );
        ::XCloseDisplay( m_header.xDisplay );
        m_header.xDisplay = NULL;

#endif
        return false;
    }

    const bool window::consumeInput( void )
    {
        memset( &m_input, 0, sizeof( m_input ) );
        return false;
    }

//
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
				t_window->m_size = glm::ivec2( (int) LOWORD( p_lParam ), (int) HIWORD( p_lParam ) );
                LOG.debug( "window size changed: ivec2({0}, {1})", t_window->m_size.x, t_window->m_size.y );
                return 0;
            }
            case WM_MOVE:
            {
				t_window->m_position = glm::ivec2( (int)(short) LOWORD( p_lParam ), (int)(short) HIWORD( p_lParam ) );
                LOG.debug( "window position changed: ivec2({0}, {1})", t_window->m_position.x, t_window->m_position.y );
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
#elif defined OS_LINUX

    Atom AtomDeleteWindowCmd = 0;
    Atom AtomWM_NAME = 0;
    Atom Atom_NET_WM_BYPASS_COMPOSITOR = 0;
    
    bool ChangeVideoMode_XF86VidMode( Display * p_xDisplay, int p_xScreen, Window p_xWindow,
								int * p_currentWidth, int * p_currentHeight, float * p_currentRefreshRate,
								int * p_desiredWidth, int * p_desiredHeight, float * p_desiredRefreshRate )
    {
        int t_videoModeCount;
        XF86VidModeModeInfo ** t_videoModeInfos;

        ::XF86VidModeGetAllModeLines( p_xDisplay, p_xScreen, &t_videoModeCount, &t_videoModeInfos );

        if ( p_currentWidth != NULL && p_currentHeight != NULL && p_currentRefreshRate != NULL )
        {
            XF86VidModeModeInfo * mode = t_videoModeInfos[0];
            *p_currentWidth = mode->hdisplay;
            *p_currentHeight = mode->vdisplay;
            *p_currentRefreshRate = ( mode->dotclock * 1000.0f ) / ( mode->htotal * mode->vtotal );
        }

        if ( p_desiredWidth != NULL && p_desiredHeight != NULL && p_desiredRefreshRate != NULL )
        {
            XF86VidModeModeInfo * bestMode = NULL;
            int bestModeWidth = 0;
            int bestModeHeight = 0;
            float bestModeRefreshRate = 0.0f;
            int bestSizeError = 0x7FFFFFFF;
            float bestRefreshRateError = 1e6f;
            for ( int j = 0; j < t_videoModeCount; j++ )
            {
                XF86VidModeModeInfo * mode = t_videoModeInfos[j];
                const int modeWidth = mode->hdisplay;
                const int modeHeight = mode->vdisplay;
                const float modeRefreshRate = ( mode->dotclock * 1000.0f ) / ( mode->htotal * mode->vtotal );

                const int dw = modeWidth - *p_desiredWidth;
                const int dh = modeHeight - *p_desiredHeight;
                const int sizeError = dw * dw + dh * dh;
                const float refreshRateError = fabs( modeRefreshRate - *p_desiredRefreshRate );
                if ( sizeError < bestSizeError || ( sizeError == bestSizeError && refreshRateError < bestRefreshRateError ) )
                {
                    bestSizeError = sizeError;
                    bestRefreshRateError = refreshRateError;
                    bestMode = mode;
                    bestModeWidth = modeWidth;
                    bestModeHeight = modeHeight;
                    bestModeRefreshRate = modeRefreshRate;
                }
            }

            ::XF86VidModeSwitchToMode( p_xDisplay, p_xScreen, bestMode );
            ::XF86VidModeSetViewPort( p_xDisplay, p_xScreen, 0, 0 );

            *p_desiredWidth = bestModeWidth;
            *p_desiredHeight = bestModeHeight;
            *p_desiredRefreshRate = bestModeRefreshRate;
        }

        for ( int i = 0; i < t_videoModeCount; i++ )
        {
            if ( t_videoModeInfos[i]->privsize > 0 )
            {
                //::XFree( t_videoModeInfos[i]->private );
                ::XFree( ( void * )( t_videoModeInfos[i] + sizeof( XF86VidModeModeInfo ) - sizeof( int ) ) );
            }
        }
        ::XFree( t_videoModeInfos );

        return true;
    }

    int x_error_handler( Display * p_display, XErrorEvent * p_event )
    {
        char t_err_desc[500];
        ::XGetErrorText(p_display, p_event->error_code, t_err_desc, 500);

        std::cerr << "X Error: \n"
            "\tdisplay = " << ::XDisplayName(NULL) << "'\n"
            "\tserial = " << p_event->serial << "'\n"
            "\terror = '" << t_err_desc << "\n"
            "\trequest = " << static_cast<int>(p_event->request_code) << "\n"
            "\tminor = " << static_cast<int>(p_event->minor_code) << "\n";

        return 0;
    }

#endif
}