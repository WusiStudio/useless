#include "window.h"
#include "tools/log.hpp"


namespace ROOT_NAMESPACE
{

    window & window::Create( const std::string & p_title, const glm::vec2 & p_size, const glm::vec2 & p_position )
    {
        window & t_result = Create();
        
        if( t_result.init( p_title, p_size, p_position ) )
        {
            LOG.error( "window init faile!" );
        }
        
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

    bool window::init( const std::string & p_title, const glm::vec2 & p_size, const glm::vec2 & p_position )
    {
        if( init() ) return true;

#ifdef OS_WINDOWS

        m_header.instance = GetModuleHandle(NULL); 

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


        if( !RegisterClass( &t_wc ) )  
        {  
            LOG.error( "Register Window Faile!" );
            return true;  
        }

        m_header.hwnd = CreateWindow(   
        "window" ,                                                      //类名，要和刚才注册的一致  
        p_title.c_str() ,                                               //窗口标题文字  
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,        //窗口外观样式  
        (int)p_position.x,                                              //窗口相对于父级的X坐标  
        (int)p_position.y,                                              //窗口相对于父级的Y坐标  
        (int)p_size.x,                                                  //窗口的宽度  
        (int)p_size.y,                                                  //窗口的高度  
        NULL,                                                           //没有父窗口，为NULL   
        NULL,                                                           //没有菜单，为NULL   
        m_header.instance,                                          //当前应用程序的实例句柄  
        NULL);                                                          //没有附加数据，为NULL

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
        UpdateWindow( m_header.hwnd );

        

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
        m_title = "";
        m_size = glm::vec2( 0.0f );
        m_position = glm::vec2( 0.0f );
        m_tick = NULL;

#ifdef OS_WINDOWS
        m_header.instance = 0;
        m_header.hwnd = 0;
        m_header.hDC = NULL;
#endif

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