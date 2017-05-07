#ifndef __BUG_350_H__
#define __BUG_350_H__

#include "BugsTest.h"

class Bug350Layer : public BugsTestBase
{
public:
    static Bug350Layer* create()
    {
        auto ret = new Bug350Layer;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual bool init() override;
};

#endif // __BUG_350_H__
