#ifndef _JNI_TEST_H_
#define _JNI_TEST_H_

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "../BaseTest.h"

DEFINE_TEST_SUITE(JNITests);

class JNITest : public TestCase
{
public:
    static JNITest* create()
    {
        auto ret = new JNITest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    JNITest();
};
#endif
#endif // _JNI_TEST_H_
