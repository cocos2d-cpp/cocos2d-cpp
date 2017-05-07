#ifndef __BUG_914_H__
#define __BUG_914_H__

#include "BugsTest.h"

class Bug914Layer : public BugsTestBase
{
public:
    virtual bool init() override;

    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void restart(cocos2d::Ref* sender);

    static Bug914Layer* create()
    {
        auto ret = new Bug914Layer;
        ret->init();
        ret->autorelease();
        return ret;
    }
};

#endif // __BUG_914_H__
