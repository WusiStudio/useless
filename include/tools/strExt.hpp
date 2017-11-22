#pragma once
#ifndef __STR_EXT_HPP__
#define __STR_EXT_HPP__

#include <string>
#include <vector>
#include <cstdarg>
#include <regex>
#include <algorithm>  
#include <map>
#include <cassert>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <functional>

#ifndef CHAR_SET
#define CHAR_SET "UTF-8"
#endif //CHAR_SET

namespace ROOT_NAMESPACE
{
	//int转换为string 限制最小长度
	std::string intToString( const int p_val, const unsigned p_min = 0 );

	//返回字符串实际长度（单位是一个英文字符）
	const unsigned getStringLength ( const std::string & p_str, const std::string & p_coding = CHAR_SET );

	//返回字符实际长度 (单位是一个英文字符)
	const unsigned getCharRealLength( const char * p_char, const std::string & p_coding = CHAR_SET );

	//返回字符实际大小 (单位bit)
	const unsigned getCharRealSize( const char * p_char, const std::string & p_coding = CHAR_SET );

	//遍历字符串的每一个字符 返回字符位置 字符大小 字符长度
	void eachString( const std::string & p_str, std::function< bool ( const unsigned int p_index, const unsigned int p_size, const unsigned int p_realLength )> p_callBack, const std::string & p_coding = CHAR_SET );

	//获取当前运行环境编码
	std::string & getSystemCodeset( void );

	//字符串长度限制
	std::string stringLimit( const std::string & p_str, const unsigned int p_limitLength, const bool forb = true, const std::string & p_coding = CHAR_SET );

	//格式化字符串
	template<typename... Arguments>
	std::string strFormat( std::string p_str, const Arguments & ... p_args );
}

#endif //__STR_EXT_HPP__

#include "achieve/strExt.inl"