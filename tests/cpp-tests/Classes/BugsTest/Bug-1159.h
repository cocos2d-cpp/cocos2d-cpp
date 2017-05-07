#ifndef __BUG_1159_H__
#define __BUG_1159_H__

#include "BugsTest.h"

class Bug1159Layer : public BugsTestBase
{
public:
    virtual bool init() override;
    virtual void onExit() override;

    void callBack(cocos2d::Ref* sender);

    static Bug1159Layer* create()
    {
        auto ret = new Bug1159Layer;
        ret->init();
        ret->autorelease();
        return ret;
    }
};

#endif // __BUG_1159_H__
