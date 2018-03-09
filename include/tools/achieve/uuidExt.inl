#pragma once
#ifndef __UUID_EXT_INL__
#define __UUID_EXT_INL__

#include <cstring>
#include <algorithm>

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
    #include <objbase.h>
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    #include <uuid/uuid.h>
#endif

namespace ROOT_NAMESPACE
{

    inline uuid::uuid()
    {
        memset( mVal, 0, sizeof( mVal ) );
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
        GUID guid; 
        if (CoCreateGuid ( &guid )) { fprintf ( stderr, "create guid error\n" ); }
        sprintf_s ( mVal, sizeof ( mVal ), "%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X", guid.Data1, guid.Data2, guid.Data3,
                guid.Data4[0], guid.Data4[1], guid.Data4[2], 
                guid.Data4[3], guid.Data4[4], guid.Data4[5], 
                guid.Data4[6], guid.Data4[7] );
#endif
#if defined(__linux__) || defined(__APPLE__)
        uuid_t uuid; 
        uuid_generate ( uuid ); 
        snprintf ( mVal, sizeof ( mVal ), "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", 
            uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7], 
            uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15] );
#endif
    }

    inline uuid::uuid( const uuid & p_uuid )
    {
        memcpy( mVal, p_uuid.mVal, sizeof(mVal) );
    }

    inline bool uuid::operator!=( const uuid & p_uuid ) const
    {
        return !operator==( p_uuid );
    }

    inline bool uuid::operator==( const uuid & p_uuid ) const
    {
        for( size_t i = 0; i < sizeof(mVal); ++i )
        {
            if( mVal[i] != p_uuid.mVal[i] ) return false;
        }
        return true;
    }

    inline std::string uuid::toString(void) const
    {
        return std::string( mVal );
    }

    inline std::string uuid::toLowerString(void) const
    {
        std::string result( mVal );
        std::transform( result.begin(), result.end(), result.begin(), ::tolower );
        return result;
    }

}

#endif //__UUID_EXT_INL__