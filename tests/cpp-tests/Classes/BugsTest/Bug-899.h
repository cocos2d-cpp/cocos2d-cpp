#ifndef __BUG_899_H__
#define __BUG_899_H__

#include "BugsTest.h"

class Bug899Layer : public BugsTestBase
{
public:
    static Bug899Layer* create()
    {
        auto ret = new Bug899Layer;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual bool init() override;
};

#endif // __BUG_899_H__
