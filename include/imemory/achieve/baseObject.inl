#pragma once
#ifndef __BASE_OBJECT_INL__
#define __BASE_OBJECT_INL__

namespace ROOT_NAMESPACE
{
    inline void baseObj::retain( void )
    {
        QuoteMutex().lock();
        mQuote++;
        QuoteMutex().unlock();
    }

    inline const char * baseObj::realType( void )
    {
        return typeid(baseObj).name();
    }
    
    inline void baseObj::release( void )
    {
        QuoteMutex().lock();
        mQuote--;
        QuoteMutex().unlock();
    }

    inline int baseObj::frequency( void ) const
    {
        return mFrequency;
    }

    inline int baseObj::frequencyMinusOne(void)
    {
        return --mFrequency;
    }

    inline void baseObj::setFrequency ( const int p_frequency )
    {
        mFrequency = p_frequency;
    }

    inline int baseObj::quote( void ) const
    {
        return mQuote;
    }

    inline baseObj::~baseObj( void )
    {
        
    }

    inline baseObj::baseObj( void )
    {
        mFrequency = 0;
    }

    //初始化 用来替代new函数的初始化功能
    //有错误返回true 没有错误返回false
    inline bool baseObj::init( void )
    {
        mQuote = 0;
        return false;
    }

    //放入缓存中时调用
    inline bool baseObj::destroy( void )
    {
        mQuote = 0;
        return false;
    }

    inline std::mutex & baseObj::QuoteMutex(void)
    {
        static std::mutex smQuoteMutex;
        return smQuoteMutex;
    }
}

#endif //__BASE_OBJECT_INL__