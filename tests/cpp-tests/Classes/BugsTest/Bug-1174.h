#ifndef __BUG_1174_H__
#define __BUG_1174_H__

#include "BugsTest.h"

class Bug1174Layer : public BugsTestBase
{
public:
    static Bug1174Layer* create()
    {
        auto ret = new Bug1174Layer;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual bool init() override;
};

#endif // __BUG_1174_H__
