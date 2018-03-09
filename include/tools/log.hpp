#pragma once
#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <string>
#include <vector>

namespace ROOT_NAMESPACE
{
    class log
    {
    public:
        enum level{ DEBUG = 0, INFO, NOTICE, WARN, ERR, CRIT, ALERT, EMERG };

        static log & Instance(void);

        log();

        log( level p_level );
        
        void setFilterLevel( level p_level );

        // 调试: 调试信息
        template<typename... Arguments>
        void debug( std::string p_str, const Arguments & ... p_args) const;

        // 信息: 程序输出信息
        template<typename... Arguments>
        void info( std::string p_str, const Arguments & ... p_args) const;

        // 通知: 程序可以运行但是还不够完美的错误
        template<typename... Arguments>
        void notice( std::string p_str, const Arguments & ... p_args) const;
        // 警告性错误: 需要发出警告的错误
        template<typename... Arguments>
        void warning( std::string p_str, const Arguments & ... p_args) const;

        // 一般错误: 一般性错误
        template<typename... Arguments>
        void error( std::string p_str, const Arguments & ... p_args) const;

        // 临界值错误: 超过临界值的错误，例如一天24小时，而输入的是25小时这样
        template<typename... Arguments>
        void critical( std::string p_str, const Arguments & ... p_args) const;

        // 警戒性错误: 必须被立即修改的错误
        template<typename... Arguments>
        void alert( std::string p_str, const Arguments & ... p_args) const;

        // 严重错误: 导致系统崩溃无法使用
        template<typename... Arguments>
        void emerg( std::string p_str, const Arguments & ... p_args) const;

    private:

		template<typename... Arguments>
        void _log(const level p_level, std::string p_str, const Arguments & ... p_args) const;

         //输出日志
        void _printLog( const level p_level, const std::string & p_log ) const;

        template<typename... Arguments>
        void _printLog( const level p_level, const std::string & p_format, const Arguments & ... p_args) const;

        static std::vector< std::string > & LevelNames(void);
        level mFilterLevel;
    };

    #define LOG ROOT_NAMESPACE::log::Instance()
}

#include "achieve/log.inl"

#endif //__LOG_HPP__