#include "window.h"
#include "tools/log.hpp"


namespace ROOT_NAMESPACE
{

    window & window::Create( const std::string & p_title, const glm::vec2 & p_size, const glm::vec2 & p_position, const bool p_fullScene )
    {
        window & t_result = Create();
        
        if( t_result.init( p_title, p_size, p_position, p_fullScene ) )
        {
            LOG.error( "window init faile!" );
        }
        
        return t_result;
    }

    glm::vec2 window::GetSystemResolution( void )
    {
        glm::vec2 t_result = glm::vec2( 0.0f );
#ifdef OS_WINDOWS
        RECT t_desktopRect;
        GetWindowRect( GetDesktopWindow(), &t_desktopRect );
        t_result = glm::vec2( t_desktopRect.right, t_desktopRect.bottom );
#endif
        return t_result;
    }

    bool window::run( void )
    {
        m_run = true;
#ifdef OS_WINDOWS
        MSG msg;
        while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0 )
        {
            if ( msg.message == WM_QUIT )
            {
                m_run = false;
            }
            else
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
#endif
        return false;
    }

    bool window::init( const std::string & p_title, const glm::vec2 & p_size, const glm::vec2 & p_position, const bool p_fullScene )
    {
        if( init() ) return true;

        m_fullScene = p_fullScene;

#ifdef OS_WINDOWS

        m_header.instance = 0;
        m_header.hwnd = 0;
        m_header.hDC = NULL;

        m_header.instance = GetModuleHandle(NULL); 

        DWORD       t_dwExStyle;                    // 扩展窗口风格
        DWORD       t_dwStyle;                      // 窗口风格
        RECT        t_WindowRect;                   // 取得矩形的左上角和右下角的坐标值
        const LPCSTR t_displayDevice = NULL;
        WNDCLASS t_wc;
        t_wc.lpszClassName = "window";                                  //注册类名  
        t_wc.lpfnWndProc = (WNDPROC)windowHeader::window_proc;          //窗口过程  
        t_wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;                //窗口风格  
        t_wc.hInstance = m_header.instance;                             //应用程序实例  
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

        DEVMODEA lpDevMode;
        memset( &lpDevMode, 0, sizeof( DEVMODEA ) );
        lpDevMode.dmSize = sizeof( DEVMODEA );
        lpDevMode.dmDriverExtra = 0;

        if ( EnumDisplaySettingsA( t_displayDevice, ENUM_CURRENT_SETTINGS, &lpDevMode ) != FALSE )
        {
            LOG.debug( "windowRefreshRate: ", (float)lpDevMode.dmDisplayFrequency );
            //window->windowRefreshRate = (float)lpDevMode.dmDisplayFrequency;
        }

        if( m_fullScene )
        {
            t_dwExStyle = WS_EX_APPWINDOW;                                  // 扩展窗体风格
            t_dwStyle = WS_POPUP;                                           // 窗体风格
            ShowCursor(true);                                               // 开启鼠标指针
        }   
        else    
        {   
            t_dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;               // 扩展窗体风格
            //  窗体风格
            t_dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        }

        AdjustWindowRectEx( &t_WindowRect, t_dwStyle, false, t_dwExStyle);  // 调整窗口达到真正要求的大小

        if( !m_fullScene && (int)(p_position.x + p_position.y) == 0 )
        {
            glm::vec2 t_systemResolution = GetSystemResolution();
            LOG.info( "t_systemResolution.x: ", t_systemResolution.x, ", t_systemResolution.y: ", t_systemResolution.y );
            const int t_offsetX = ( (int)t_systemResolution.x - ( t_WindowRect.right - t_WindowRect.left ) ) / 2 - t_WindowRect.left;
            const int t_offsetY = ( (int)t_systemResolution.y - ( t_WindowRect.bottom - t_WindowRect.top ) ) / 2 - t_WindowRect.top;

            t_WindowRect.left += t_offsetX;
            t_WindowRect.right += t_offsetX;
            t_WindowRect.top += t_offsetY;
            t_WindowRect.bottom += t_offsetY;
        }

        m_header.hwnd = CreateWindowEx(
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
        m_header.instance,                                                  //当前应用程序的实例句柄  
        NULL);                                                              //没有附加数据，为NULL

        if( !m_header.hwnd )
        {
            LOG.error( "Create Window Faile [{0}] !", GetLastError() );
            return true;
        }

        windowHeader::ms_windowList[m_header.hwnd] = (object *)this;

        SetWindowLongPtrA( m_header.hwnd, GWLP_USERDATA, (LONG_PTR) &m_header );
        m_header.hDC = GetDC( m_header.hwnd );
        if( !m_header.hDC )
        {
            LOG.error("Failed to acquire device context.");
            return true;
        }

        ShowWindow( m_header.hwnd, SW_SHOW | SW_NORMAL );
        SetForegroundWindow( m_header.hwnd );
        SetFocus( m_header.hwnd ); 
        // UpdateWindow( m_header.hwnd );

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
        m_title = "";
        m_size = glm::vec2( 0.0f );
        m_position = glm::vec2( 0.0f );
        m_tick = NULL;

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
        LRESULT  ret = 0;
        switch( p_msg )  
        {  
        case WM_SIZE:
		{
            if( t_window != NULL )
            {
                t_window->m_size = glm::vec2( (int) LOWORD( p_lParam ), (int) HIWORD( p_lParam ) );
                LOG.debug( "window size changed: vec2({0}, {1})", t_window->m_size.x, t_window->m_size.y );
            }
			return 0;
		}
        case WM_MOVE:
        {
            if( t_window != NULL )
            {
                t_window->m_position = glm::vec2( (int)(short) LOWORD( p_lParam ), (int)(short) HIWORD( p_lParam ) );
                LOG.debug( "window position changed: vec2({0}, {1})", t_window->m_position.x, t_window->m_position.y );
            }
            return 0;
        }
        case WM_ACTIVATE:
		{
			if ( t_window != NULL )
			{
				t_window->m_active = LOWORD( p_wParam ) != WA_INACTIVE;
                t_window->m_minimized = (bool)HIWORD( p_wParam );
                LOG.debug( "window active state changed: {0} {1}", t_window->m_active, t_window->m_minimized );
			}
			return 0;
		}   
        case WM_PAINT:  
            {  
            }  
            break; 
        default:  
            ret = ::DefWindowProcA( p_hWnd, p_msg, p_wParam, p_lParam);  
            break;  
        }
        return ret;  
    }
#endif
}