#ifndef _CURL_TEST_H_
#define _CURL_TEST_H_

#include "../BaseTest.h"

DEFINE_TEST_SUITE(CurlTests);

class CurlTest : public TestCase
{
public:
    static CurlTest* create()
    {
        auto ret = new CurlTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    CurlTest();
    ~CurlTest();

    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

private:
    cocos2d::Label* _label;
};

#endif // _CURL_TEST_H_
