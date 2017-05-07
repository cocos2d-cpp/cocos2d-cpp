#ifndef _OPEN_URL_TEST_H_
#define _OPEN_URL_TEST_H_

#include "../BaseTest.h"

DEFINE_TEST_SUITE(OpenURLTests);

class OpenURLTest : public TestCase
{
public:
    static OpenURLTest* create()
    {
        auto ret = new OpenURLTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    OpenURLTest();
    ~OpenURLTest();

    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
private:
    cocos2d::Label* _label;
};

#endif // _OPEN_URL_TEST_H_
