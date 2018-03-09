#pragma once
#ifndef __UUID_EXT_HPP__
#define __UUID_EXT_HPP__
#include <stdio.h> 
#include <string>


namespace ROOT_NAMESPACE
{
    class uuid
    {
    public:
        uuid();

        uuid( const uuid & p_uuid );

        bool operator!=( const uuid & p_uuid ) const;

        bool operator==( const uuid & p_uuid ) const;

        std::string toString(void) const;

        std::string toLowerString(void) const;
    private:
        char mVal[33];
    };
}

#endif //__UUID_EXT_HPP__

#include "achieve/uuidExt.inl"