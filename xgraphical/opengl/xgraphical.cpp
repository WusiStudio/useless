#include "xgraphical.h"

namespace ROOT_NAMESPACE
{

    xgraphical & xgraphical::Create ( windowHeader & p_windowHeader )
    {
        xgraphical * t_result = &Create ();

        if (t_result->init ( p_windowHeader ))
        {
            t_result->destroy ();
            t_result = nullptr;
        }

        assert ( t_result );

        return *t_result;
    }

    bool xgraphical::init ( void )
    {
        if (object::init ())
        {
            return true;
        }
        return false;
    }

    bool xgraphical::init ( windowHeader & p_windowHeader )
    {

        static PIXELFORMATDESCRIPTOR s_pfd =
        {
            sizeof ( PIXELFORMATDESCRIPTOR ), //上述格式描述符的大小
            1,                                //版本号
            PFD_DRAW_TO_WINDOW |              //格式支持窗口
            PFD_SUPPORT_OPENGL |              //格式必须支持OpenGL
            PFD_DOUBLEBUFFER,                 //必须支持双缓冲
            PFD_TYPE_RGBA,                    //申请RGBA 格式
            24,                               //选定色彩深度
            0, 0, 0, 0, 0, 0, 0, 0,           //忽略RGBA
            0,                                //无累加缓存
            0, 0, 0, 0,                       //忽略聚集位
            32,                               //32位Z-缓存(深度缓存)
            0,                                //无蒙板缓存
            0,                                //无辅助缓存
            PFD_MAIN_PLANE,                   //主绘图层
            0,                                //Reserved
            0, 0, 0                           //忽略层遮罩
        };


        int t_PixelFormat = ChoosePixelFormat ( p_windowHeader.hDC, &s_pfd );

        SetPixelFormat ( p_windowHeader.hDC, t_PixelFormat, &s_pfd );

        p_windowHeader.hRC = wglCreateContext ( p_windowHeader.hDC );

        return false;
    }

    bool xgraphical::render ( void )
    {

        return false;
    }


    xgraphical::~xgraphical ( void )
    {

    }

    xgraphical::xgraphical ( void )
    {

    }

}