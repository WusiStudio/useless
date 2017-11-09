#pragma once
#ifndef __LOG_HPP__
#define __LOG_HPP__

#include "strExt.hpp"

#include <string>
#include <iostream>
#include <mutex>
#include <vector>

namespace ws
{
    class log
    {
    public:
        enum level{ DEBUG = 0, INFO, NOTICE, WARN, ERR, CRIT, ALERT, EMERG };

        static log & Instance(void)
        {
            static log result;
            return result;
        }

        log()
        {
            mFilterLevel = level::DEBUG;
        }

        log( level p_level )
        {
            mFilterLevel = p_level;
        }
        
        void setFilterLevel( level p_level )
        {
            static std::mutex tMutex;
            tMutex.lock();
            mFilterLevel = p_level;
            tMutex.unlock();
        }

        // 调试: 调试信息
        template<typename... Arguments>
        void debug( std::string p_str, const Arguments & ... p_args) const
        {
            _log(level::DEBUG, p_str, p_args...);
        }

        // 信息: 程序输出信息
        template<typename... Arguments>
        void info( std::string p_str, const Arguments & ... p_args) const
        {
            _log(level::INFO, p_str, p_args...);
        }

        // 通知: 程序可以运行但是还不够完美的错误
        template<typename... Arguments>
        void notice( std::string p_str, const Arguments & ... p_args) const
        {
            _log(level::NOTICE, p_str, p_args...);
        }

        // 警告性错误: 需要发出警告的错误
        template<typename... Arguments>
        void warning( std::string p_str, const Arguments & ... p_args) const
        {
            _log(level::WARN, p_str, p_args...);
        }

        // 一般错误: 一般性错误
        template<typename... Arguments>
        void error( std::string p_str, const Arguments & ... p_args) const
        {
            _log(level::ERR, p_str, p_args...);
        }

        // 临界值错误: 超过临界值的错误，例如一天24小时，而输入的是25小时这样
        template<typename... Arguments>
        void critical( std::string p_str, const Arguments & ... p_args) const
        {
            _log(level::CRIT, p_str, p_args...);
        }

        // 警戒性错误: 必须被立即修改的错误
        template<typename... Arguments>
        void alert( std::string p_str, const Arguments & ... p_args) const
        {
            _log(level::ALERT, p_str, p_args...);
        }

        // 严重错误: 导致系统崩溃无法使用
        template<typename... Arguments>
        void emerg( std::string p_str, const Arguments & ... p_args) const
        {
            _log(level::EMERG, p_str, p_args...);
        }


    private:

		template<typename... Arguments>
        void _log(const level p_level, std::string p_str, const Arguments & ... p_args) const
        {
            //验证是否需要打印
            if( p_level < mFilterLevel ) return;

            _printLog( p_level, p_str, p_args... );
        }

         //输出日志
        void _printLog( const level p_level, const std::string & p_log ) const
        {
            //输出介质

            static std::mutex tMutex;
            tMutex.lock();
            std::cout << strExt::strFormat( "[{0, 5}] {2, T} {1} ", LevelNames()[p_level], p_log, time(nullptr) ) << std::endl;
            tMutex.unlock();
        }

        template<typename... Arguments>
        void _printLog( const level p_level, const std::string & p_format, const Arguments & ... p_args) const
        {
            _printLog( p_level, strExt::strFormat( p_format, p_args... ) );
        }

        static std::vector< std::string > & LevelNames(void)
        {
            static std::vector< std::string > smLevelNames ( { "DEBUG", "INFO", "NOTICE", "WARN", "ERR", "CRIT", "ALERT", "EMERG" } );
            return smLevelNames;
        }

        level mFilterLevel;
    };

    #define LOG ws::log::Instance()
}

#endif //__LOG_HPP__