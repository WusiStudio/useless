#pragma once
#ifndef __UUID_EXT_HPP__
#define __UUID_EXT_HPP__
#include <stdio.h> 
#include <string>


namespace ws
{
    class uuid
    {
    public:
        uuid();

        uuid( const uuid & p_uuid );

        bool operator!=( const uuid & p_uuid );

        bool operator==( const uuid & p_uuid );

        std::string toString(void);

        std::string toLowerString(void);
    private:
        char mVal[33];
    };
}

#endif //__UUID_EXT_HPP__

#include "uuidExt.inl"