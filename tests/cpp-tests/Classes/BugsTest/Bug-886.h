#ifndef __BUG_886_H__
#define __BUG_886_H__

#include "BugsTest.h"

class Bug886Layer : public BugsTestBase
{
public:
    static Bug886Layer* create()
    {
        auto ret = new Bug886Layer;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual bool init() override;
};

#endif // __BUG_886_H__
