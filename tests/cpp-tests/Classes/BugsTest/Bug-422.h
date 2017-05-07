#ifndef __BUG_422_H__
#define __BUG_422_H__

#include "BugsTest.h"

class Bug422Layer : public BugsTestBase
{
public:
    static Bug422Layer* create()
    {
        auto ret = new Bug422Layer;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual bool init() override;

    void reset();
    void check(Node* target);
    void menuCallback(cocos2d::Ref* sender);
};

#endif // __BUG_422_H__
