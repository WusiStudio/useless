#pragma once
#ifndef __UUID_EXT_HPP__
#define __UUID_EXT_HPP__
#include <stdio.h> 
#include <string>
#include <cstring>
#include <algorithm>

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
    #include <objbase.h>
#elif defined(__linux__) || defined(__unix__)
    #include <uuid/uuid.h>
#endif


namespace ws
{
    class uuid
    {
    public:
        uuid()
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
#if defined(__linux__)
			uuid_t uuid; 
			uuid_generate ( uuid ); 
			snprintf ( mVal, sizeof ( mVal ), "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", 
				uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7], 
				uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15] );
#endif
        }

        uuid( const uuid & p_uuid )
        {
            memcpy( mVal, p_uuid.mVal, sizeof(mVal) );
        }

        bool operator!=( const uuid & p_uuid )
        {
            return !operator==( p_uuid );
        }

        bool operator==( const uuid & p_uuid )
        {
            for( int i = 0; i < sizeof(mVal); ++i )
            {
                if( mVal[i] != p_uuid.mVal[i] ) return false;
            }
            return true;
        }

        std::string toString(void)
        {
            return std::string( mVal );
        }

        std::string toLowerString(void)
        {
            std::string result( mVal );
            std::transform( result.begin(), result.end(), result.begin(), ::tolower );
            return result;
        }
    private:
        char mVal[33];
    };
}

#endif //__UUID_EXT_HPP__