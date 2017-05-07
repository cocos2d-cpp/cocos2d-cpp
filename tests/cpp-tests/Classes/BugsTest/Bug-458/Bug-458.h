#ifndef __BUG_458_H__
#define __BUG_458_H__

#include "../BugsTest.h"

class Bug458Layer : public BugsTestBase
{
public:
    static Bug458Layer* create()
    {
        auto ret = new Bug458Layer;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual bool init() override;
    void selectAnswer(cocos2d::Ref* sender);
};

#endif // __BUG_458_H__
