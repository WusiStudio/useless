#pragma once
#ifndef __MY_APPACTION_H__
#define __MY_APPACTION_H__

#include "appaction.h"

class myAppaction : public ROOT_NAMESPACE::appaction
{
public:
    CREATEFUNC( myAppaction );

protected:
    virtual bool init( void ) override;

    virtual bool tick( const float p_tm ) override;

};

#endif //__MY_APPACTION_H__