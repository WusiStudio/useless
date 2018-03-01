#pragma once
#ifndef __NODE_H__
#define __NODE_H__

#include "imemory/object.hpp"
#include "glm/vec3.hpp"

namespace ROOT_NAMESPACE
{
    class node : public object
    {
    ATTIRBUTE( glm::vec3, Position );
    public:
        CREATEFUNC( node );
    protected:
        node( void );
        virtual ~node( void );

        virtual bool init( void );

        virtual bool tick( const float p_tm );
        virtual bool keyboardDown( const char p_key );
        virtual bool keyboardUp( const char p_key );

        virtual bool mouseMove( const glm::ivec2 & p_pos );
        virtual bool mouseDown( const char p_key );
    private:
        virtual bool draw( void );
    };
}

#endif //__NODE_H__