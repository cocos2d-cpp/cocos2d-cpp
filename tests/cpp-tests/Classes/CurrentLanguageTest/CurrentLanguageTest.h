#ifndef _CURRENT_LANGUAGE_TEST_H_
#define _CURRENT_LANGUAGE_TEST_H_

#include "../BaseTest.h"

DEFINE_TEST_SUITE(CurrentLanguageTests);

class CurrentLanguageTest : public TestCase
{
public:
    static CurrentLanguageTest* create()
    {
        auto ret = new CurrentLanguageTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    CurrentLanguageTest();
};

#endif // _CURRENT_LANGUAGE_TEST_H_
