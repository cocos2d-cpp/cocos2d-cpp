#ifndef _USERDEFAULT_TEST_H_
#define _USERDEFAULT_TEST_H_

#include "../BaseTest.h"
#include "2d/CCLabel.h"

DEFINE_TEST_SUITE(UserDefaultTests);

class UserDefaultTest : public TestCase
{
public:
    static UserDefaultTest* create()
    {
        auto ret = new UserDefaultTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    UserDefaultTest();
    ~UserDefaultTest();

private:
    void doTest();
    void printValue();
    cocos2d::Label* _label;
};

#endif // _USERDEFAULT_TEST_H_
