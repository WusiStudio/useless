#pragma once
#ifndef __THREAD_EXT_HPP__
#define __THREAD_EXT_HPP__

namespace ROOT_NAMESPACE
{
    //获取当前线程id
    unsigned int PthreadSelf();
}

#endif //__THREAD_EXT_HPP__

#include "achieve/threadExt.inl"