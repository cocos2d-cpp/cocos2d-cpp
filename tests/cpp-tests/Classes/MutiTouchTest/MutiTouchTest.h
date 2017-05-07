#ifndef __MUTITOUCHTEST_H__
#define __MUTITOUCHTEST_H__

#include "../BaseTest.h"

DEFINE_TEST_SUITE(MutiTouchTests);

class MutiTouchTest : public TestCase
{
public:
    static MutiTouchTest* create()
    {
        auto ret = new MutiTouchTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual bool init() override;

    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
};

#endif /* __MUTITOUCHTEST_H__ */
