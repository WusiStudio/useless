#pragma once
#ifndef __BASE_OBJ_H__
#define __BASE_OBJ_H__

#include <mutex>

namespace ROOT_NAMESPACE
{
    class baseObj
    {
		friend class gcWorker;
    public:
    
        virtual void retain( void );

        virtual const char * realType( void );
        
        virtual void release( void );

        int frequency( void ) const;

		int frequencyMinusOne(void);

		void setFrequency ( const int p_frequency );

        int quote( void ) const;

        virtual ~baseObj( void );
    protected:
        baseObj( void );
        
        //初始化 用来替代new函数的初始化功能
        //有错误返回true 没有错误返回false
        virtual bool init( void );

        //放入缓存中时调用
        virtual bool destroy( void );

    private:
        //缓存频率 频率高的在一级缓存  频率低的被回收
        int mFrequency;

        //对象引用数量
        int mQuote;

        static std::mutex & QuoteMutex(void);
        
    };
}

#endif //__BASE_OBJ_H__
#include "achieve/baseObject.inl"