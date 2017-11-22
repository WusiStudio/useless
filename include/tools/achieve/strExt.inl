#pragma once
#ifndef __STR_EXT_INL__
#define __STR_EXT_INL__

#include "../uuidExt.hpp"

#if defined(__unix__) || defined(__APPLE__)
#include <langinfo.h>
#endif

namespace ws
{
    struct placeholder
    {
        unsigned paraIndex;
        std::string str;
        std::string format;
    };

    inline std::string scientificToDefault ( const std::string & source )
    {
        std::string result = source;
        //处理科学计数法
        auto scientificIndex = result.find ( "e+" );
        if (scientificIndex != std::string::npos)
        {
            std::string baseNumber = result.substr ( 0, scientificIndex );
            int exponential = atoi ( result.substr ( scientificIndex + 2 ).c_str () );
            auto dotIndex = baseNumber.find ( '.' );
            baseNumber.replace ( dotIndex, 1, "" );
            while (baseNumber.length () < exponential + dotIndex)
            {
                baseNumber += "0";
            }
            result = baseNumber;
        }

        scientificIndex = result.find ( "e-" );
        if (scientificIndex != std::string::npos)
        {
            std::string baseNumber = result.substr ( 0, scientificIndex );
            int exponential = atoi ( result.substr ( scientificIndex + 2 ).c_str () );
            auto dotIndex = baseNumber.find ( '.' );
            baseNumber.replace ( dotIndex, dotIndex + 1, "" );

            std::string tempHead = "";
            while ((int)tempHead.length () + 1 < exponential)
            {
                tempHead += "0";
            }
            result = "0." + tempHead + baseNumber;
        }
        return result;
    }

    inline bool _format_D ( std::stringstream & p_strs, const std::string & p_format, const std::string & p_source )
    {
        static std::regex format_D ( "^[dD](\\d*)$" );
        auto matchBegin = std::sregex_iterator ( p_format.begin (), p_format.end (), format_D );
        auto matchEnd = std::sregex_iterator ();

        if (matchBegin == matchEnd) { return false; }

        int minCount = atoi ( matchBegin->str ( 1 ).c_str () );

        static std::regex checkNumber ( "^[+-]?(\\d+(\\.\\d+)?)(e\\+\\d+)?$|^[+-]?\\d{1,3}(,\\d{3})*(\\.(\\d{3},)*\\d{1,3})?(e\\+\\d+)?$" );
        matchBegin = std::sregex_iterator ( p_source.begin (), p_source.end (), checkNumber );

        if (matchBegin == matchEnd)
        {
            std::cout << "Data and format mismatch"  << std::endl;
            p_strs << p_source;
            return true;
        }

        std::string sourceData = matchBegin->str ();
        int convertData;

        for (auto index = sourceData.find ( ',' ); index != std::string::npos; index = sourceData.find ( ',' ))
        {
            sourceData.replace ( sourceData.begin () + (int)index, sourceData.begin () + (int)index + 1, "" );
        }

        if (sourceData.find ( '.' ) != std::string::npos)
        {
            convertData = (int)roundf ( (float)atof ( sourceData.c_str () ) );
        }
        else
        {
            convertData = atoi ( sourceData.c_str () );
        }

        std::stringstream tempSStr;
        tempSStr << convertData;

        for (int i = (int)(tempSStr.str ().length ()); i < minCount; ++i) { p_strs << "0"; }
        p_strs << convertData;

        return true;
    }

    inline bool _format_F ( std::stringstream & p_strs, const std::string & p_format, const std::string & p_source )
    {
        static std::regex format_F ( "^[fF](\\d*)$" );
        auto matchBegin = std::sregex_iterator ( p_format.begin (), p_format.end (), format_F );
        auto matchEnd = std::sregex_iterator ();

        if (matchBegin == matchEnd) { return false; }

        int decimalDigits = 2;
        if (matchBegin->str ( 1 ).length () > 0)
        {
            decimalDigits = atoi ( matchBegin->str ( 1 ).c_str () );
        }

        static std::regex checkNumber ( "^([+-]?\\d+(\\.\\d+)?)(e[\\+-]\\d+)?$|^[+-]?\\d{1,3}(,\\d{3})*(\\.(\\d{3},)*\\d{1,3})?(e[\\+-]\\d+)?$" );
        matchBegin = std::sregex_iterator ( p_source.begin (), p_source.end (), checkNumber );
        if (matchBegin == matchEnd)
        {
            std::cout << "Formatting data failed"  << std::endl;
            p_strs << p_source;
            return true;
        }

        std::string sourceData = matchBegin->str ();

        for (auto index = sourceData.find ( ',' ); index != std::string::npos; index = sourceData.find ( ',' ))
        {
            sourceData.replace ( sourceData.begin () + (int)index, sourceData.begin () + (int)index + 1, "" );
        }

        double convertData = round ( atof ( sourceData.c_str () ) * pow ( 10, decimalDigits ) ) / pow ( 10, decimalDigits );

        std::stringstream tempSStr;
        tempSStr << convertData;

        //科学计数法转换
        
        std::string tempStr = scientificToDefault ( tempSStr.str () );
        p_strs << tempStr;

        auto dotIndex = tempStr.find ( '.' );
        if (dotIndex == std::string::npos)
        {
            p_strs << ".";
            dotIndex = tempStr.length () - 1;
        }

        for (int i = (int)tempStr.length () - (int)dotIndex - 1; i < decimalDigits; ++i) { p_strs << "0"; }
        return true;
    }

    inline bool _format_C ( std::stringstream & p_strs, const std::string & p_format, const std::string & p_source )
    {
        static std::regex format_C ( "^[cC](\\d*)$" );
        auto matchBegin = std::sregex_iterator ( p_format.begin (), p_format.end (), format_C );
        auto matchEnd = std::sregex_iterator ();
        if (matchBegin == matchEnd) { return false; }

        int decimalDigits = 2;
        if (matchBegin->str ( 1 ).length () > 0) { decimalDigits = atoi ( matchBegin->str ( 1 ).c_str () ); }

        static std::regex checkNumber ( "^([^-+\\d]?)([+-]?\\d+(\\.\\d+)?(e\\+\\d+)?)$|^([^-+]?)([+-]?\\d{1,3}(,\\d{3})*(\\.(\\d{3},)*\\d{1,3})?(e\\+\\d+)?)$" );
        matchBegin = std::sregex_iterator ( p_source.begin (), p_source.end (), checkNumber );

        if (matchBegin == matchEnd)
        {
            std::cout << "Attempt to format data failed"  << std::endl;
            p_strs << p_format;
            return true;
        }

        std::string unit = matchBegin->str ( 1 ).length () > 0 ? matchBegin->str ( 1 ) : "$";

        std::string sourceData = matchBegin->str ( 2 );

        for (auto index = sourceData.find ( ',' ); index != std::string::npos; index = sourceData.find ( ',' ))
        {
            sourceData.replace ( sourceData.begin () + (int)index, sourceData.begin () + (int)index + 1, "" );
        }

        double convertData = round ( atof ( sourceData.c_str () ) * pow ( 10, decimalDigits ) ) / pow ( 10, decimalDigits );
        
        std::stringstream tempSStr;
        tempSStr << std::fixed << convertData;

        //科学计数法转换
        std::string tempStr = scientificToDefault ( tempSStr.str () );

        auto dotIndex = tempStr.find ('.');

        for (unsigned int i = tempStr.length () - 1; dotIndex != std::string::npos && i > dotIndex + decimalDigits; --i)
        {
            if (tempStr.at (i) != '0')
            {
                break;
            }
            tempStr.erase (i);
        }

        p_strs << unit << tempStr;

        if (dotIndex == std::string::npos)
        {
            p_strs << ".";
            dotIndex = tempStr.length () - 1;
        }

        for (int i = (int)tempStr.length () - (int)dotIndex - 1; i < decimalDigits; ++i) { p_strs << "0"; }

        return true;
    }

    inline bool _format_aline ( std::stringstream & p_strs, const std::string & p_format, const std::string & p_source )
    {
        static std::regex format_A ( "^([-]?)(\\d*)$" );
        auto matchBegin = std::sregex_iterator ( p_format.begin (), p_format.end (), format_A );
        auto matchEnd = std::sregex_iterator ();
        if (matchBegin == matchEnd) { return false; }

        int t_limitLen = atoi ( matchBegin->str ( 2 ).c_str () );


        p_strs << stringLimit( p_source, t_limitLen, matchBegin->str ( 1 ).c_str () != std::string ( "-" ) );

        return true;
    }

    inline bool _format_DataTime( std::stringstream & p_strs, const std::string & p_format, const std::string & p_source )
    {

        static std::regex format_T( "^([tT])(\\.*)$" );
        auto matchBegin = std::sregex_iterator ( p_format.begin (), p_format.end (), format_T );
        auto matchEnd = std::sregex_iterator ();
        if ( matchBegin == matchEnd ) { return false; }

        //解析原数据
        long stampTime = 0;


        //原数据为时间戳
        static std::regex regexTimestamp( "^\\d{10,19}$" );
        matchBegin = std::sregex_iterator ( p_source.begin (), p_source.end (), regexTimestamp );
        if( matchBegin != matchEnd )
        {
            stampTime = atol( p_source.c_str() );
        }

        if( !stampTime )
        {
            std::cout << "The data format is unknown" << std::endl;
            return false;
        }

        time_t t = (time_t)stampTime;
        struct tm* date_time = nullptr;
        
        #if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
        struct tm t_date_time;
        date_time = &t_date_time;
        localtime_s(&t_date_time, &t);
        #elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        date_time = localtime(&t);
        #endif

        //输出格式
        std::string tResult = matchBegin->str ( 2 );

        if( !tResult.length() )
        {
            p_strs << date_time->tm_year + 1900 << "-" << date_time->tm_mon + 1 << "-" << date_time->tm_mday << " " 
            << date_time->tm_hour << ":" << date_time->tm_min << ":" << date_time->tm_sec;
            return true;
        }

        return true;
    }

    //paramemt format
    template<typename Argument>
    inline static std::string _format ( const std::string & p_format, const Argument & p_source )
    {
        std::stringstream strs;
        if (p_format.length () <= 0)
        {
            strs << p_source;
            return strs.str ();
        }

        std::stringstream ss_source;
        ss_source << std::fixed << p_source;

        std::string s_source = ss_source.str ();
        if ( !( _format_aline( strs, p_format, s_source ) || 
                _format_D ( strs, p_format, s_source ) || 
                _format_F ( strs, p_format, s_source ) || 
                _format_C ( strs, p_format, s_source ) || 
                _format_DataTime( strs, p_format, s_source ) 
        ) )
        {
            strs << p_source;
        }
        return strs.str ();
    }

    template<typename Argument>
    inline static std::string & replace( std::string & p_str, const std::vector<placeholder> & p_placeholders, const unsigned p_parameterIndex, const Argument & p_arg )
    {
        //p_parameterIndex [1 - n]
        bool t_existIndex = false;
        for (auto item : p_placeholders)
        {
            if (item.paraIndex != p_parameterIndex - 1) continue;
            t_existIndex = true;
            for (auto index = p_str.find ( item.str ); index != std::string::npos; index = p_str.find ( item.str )) {
                p_str.replace ( p_str.begin () + index, p_str.begin () + index + item.str.length (), _format ( item.format, p_arg ) );
            }
        }

        //未指定位置与格式的参数按顺序输出
        if( !t_existIndex )
        {
            std::stringstream t_sstr;
            t_sstr << p_arg;
            p_str += t_sstr.str();
        }

        return p_str;
    }

    template<typename Argument, typename... Arguments>
    inline static std::string & replace( std::string & p_str, const std::vector<placeholder> & p_placeholders, const unsigned p_parameterCount, const Argument & p_arg, const Arguments & ... p_args )
    {
        return replace ( replace ( p_str, p_placeholders, p_parameterCount - sizeof...(Arguments), p_arg ), p_placeholders, p_parameterCount, p_args... );
    }

    //返回字符串实际长度（单位是一个英文字符）
    inline const unsigned getStringLength ( const std::string & p_str, const std::string & p_coding )
    {
        unsigned result = 0;
        eachString( p_str, [&result, &p_str](const unsigned int p_index, const unsigned int p_size, const unsigned int p_realLength)->bool{
            result += p_realLength;
            return true;
        }, p_coding );
        return result;
    }

    //返回字符实际长度 (单位是一个英文字符)
    inline const unsigned getCharRealLength( const char * p_char, const std::string & p_coding)
    {
        if( p_coding == "UTF-8" )
        {
            return (unsigned short)(*p_char & 0x00FF) < 0x80 ? 1 : 2;
        }

        if( p_coding == "GBK" )
        {
            unsigned short t_short = (unsigned short)(*p_char & 0x00FF);
            if( t_short >= 0x81 && t_short <= 0xFE )
            {
                unsigned short temp = *(unsigned short *)p_char;
                if( (temp >= 0xB0A1 && temp <= 0xF7FE) || (temp >= 0x8140 && temp <=0xA0FE) || (temp >= 0xAA40 && temp <= 0xFEA0) )
                {
                    return 2;
                }
            }
            return 1;
        }

        return 1;
    }

    //返回字符实际大小 (单位bit)
    inline const unsigned getCharRealSize( const char * p_char, const std::string & p_coding)
    {
        int t_length = 0;
        if( p_coding == "UTF-8" )
        {
            unsigned short temp = (unsigned short)(*p_char & 0x00FF);
            while ((temp << t_length & 0x00FF) > 1 << 7)
            {
                t_length++;
            }
            t_length = t_length < 1 ? 1 : t_length;
        }
        if( p_coding == "GBK" )
        {
            unsigned short t_short = (unsigned short)(*p_char & 0x00FF);
            if(t_short >= 0x81 && t_short <= 0xFE)
            {
                t_length = 2;
            }else
            {
                t_length = 1;
            }
            
        }
        return t_length;
    }

    //遍历字符串的每一个字符 返回字符位置 字符大小 字符长度
    inline void eachString( const std::string & p_str, std::function< bool ( const unsigned int p_index, const unsigned int p_size, const unsigned int p_realLength )> p_callBack, const std::string & p_coding )
    {
        int t_currIndex = 0;
        int t_strSize = p_str.size();
        
        for( t_currIndex = 0; t_currIndex < t_strSize; )
        {
            int t_realSize = 0;
            
            const char * t_currChar = p_str.c_str() + t_currIndex;
            
            t_realSize = getCharRealSize( t_currChar, p_coding );

            unsigned int t_realLen = getCharRealLength( t_currChar, p_coding );

            if( !p_callBack( t_currIndex, t_realSize, t_realLen ) )
            {
                break;
            }

            t_currIndex += t_realSize;
        }
    }

    //获取当前运行环境编码
    inline std::string & getSystemCodeset( void )
    {
        static std::string t_systemCodeset = "UTF-8";
        
        if( t_systemCodeset.size() <= 0 )
        {

        #if defined(_WIN32)
            LCID t_lcid = GetSystemDefaultLCID();
            switch( t_lcid )
            {
                case 0x409:
                    t_systemCodeset = "United States";
                break;
                case 0x804:
                    t_systemCodeset = "GBK";
                break;
            }
        #elif defined(__unix__) || defined(__APPLE__)
            setlocale(LC_CTYPE, "");
            t_systemCodeset = nl_langinfo(CODESET);
        #endif

            if( t_systemCodeset.size() <= 0 )
            {
                std::cout << "get system codeset faild" << std::endl;
            }
        }

        return t_systemCodeset;
    }

    //字符串长度限制
    inline std::string stringLimit( const std::string & p_str, const unsigned int p_limitLength, const bool forb, const std::string & p_coding )
    {
        std::stringstream t_result;

        unsigned t_strLen = getStringLength( p_str, p_coding );

        std::string t_tail;

        switch( p_limitLength )
        {
            case 0:
                return "";
            case 1:
                t_tail = "";
            break;
            case 2:
                t_tail = ".";
            break;
            default:
                t_tail = "..";
            break;
        }

        if( t_strLen <= p_limitLength )
        {
            if(forb)
            {
                t_result << p_str;
            }

            for( unsigned int i = 0; i < p_limitLength - t_strLen; ++i )
            {
                t_result << " ";
            }

            if(!forb)
            {
                t_result << p_str;
            }
        }
        else
        {
            size_t t_strIndex = 0, t_strRealSize = 0;
            eachString( p_str, [&t_strRealSize, &t_strIndex, t_tail, p_limitLength](const unsigned int p_index, const unsigned int p_size, const unsigned int p_realLength)->bool{
                
                if( t_strRealSize + p_realLength > p_limitLength - t_tail.length() )
                {
                    return false;
                }

                t_strRealSize += p_realLength;
                t_strIndex += p_size;
                
                return true;
            }, p_coding );
            
            t_result << p_str.substr( 0, t_strIndex );
            for( unsigned int i = t_strRealSize; i < p_limitLength - t_tail.length(); ++i )
            {
                t_result << ".";
            }
            t_result << t_tail;
        }

        return t_result.str();
    }

    //格式化字符串
    template<typename... Arguments>
    inline std::string strFormat( std::string p_str, const Arguments & ... p_args )
    {
        unsigned tParameterCount = sizeof...(Arguments);
        if(!tParameterCount) return p_str;

        std::map<std::string, std::string> tEscapes;

        std::vector<placeholder> placeholders;
        static std::regex escapeReg("\\\\.");
        auto matchBegin = std::sregex_iterator(p_str.begin(), p_str.end(), escapeReg);
        auto matchEnd = std::sregex_iterator();

        for(std::sregex_iterator item = matchBegin; item != matchEnd; ++item)
        {
            std::string key = item->str();
            auto it = tEscapes.find(key);
            if(it != tEscapes.end())
            {
                continue;
            }
            auto result = tEscapes.insert(std::pair<std::string, std::string>(key, uuid().toString()));

            it = result.first;
            for(size_t found = p_str.find(it->first); found != std::string::npos; found = p_str.find(it->first))
            {
                p_str.replace(p_str.begin() + found, p_str.begin() + found + it->first.length(), it->second);
            }
        }

        //regex:{index[:format]}
        static std::regex reg ( "\\{(\\d*)\\s*(,\\s*([^\\}]*?)\\s*)?\\}" );
        matchBegin = std::sregex_iterator ( p_str.begin (), p_str.end (), reg );

        //
        for (std::sregex_iterator item = matchBegin; item != matchEnd; ++item)
        {
            placeholder temp;
            temp.paraIndex = (unsigned)atoi ( item->str ( 1 ).c_str () );
            temp.str = item->str ( 0 );
            temp.format = item->str ( 3 );

            bool exist = false;
            for (auto item : placeholders)
            {
                if (item.str != temp.str) continue;
                exist = true;
                break;
            }
            if (exist) continue;

            //检查参数是否合法
            if (temp.paraIndex >= tParameterCount)
            {
                std::cout << "Error: Can not find the parameters with the [" << temp.str << "] matching." << std::endl;
                assert ( temp.paraIndex < tParameterCount );
            }

            placeholders.push_back ( temp );
        }

        replace ( p_str, placeholders, tParameterCount, p_args... );

        //对转义字符进行转义
        for (auto& item : tEscapes)
        {
            for (size_t found = p_str.find ( item.second ); found != std::string::npos; found = p_str.find ( item.second )) {
                p_str.replace ( p_str.begin () + found, p_str.begin () + found + item.second.length (), item.first.substr ( 1, 1 ) );
            }
        }

        return p_str;
    }
}

#endif //__STR_EXT_INL__