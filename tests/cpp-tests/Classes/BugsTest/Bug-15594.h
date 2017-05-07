// https://github.com/cocos2d/cocos2d-x/pull/15594
#ifndef __cocos2d_tests__Bug_15594__
#define __cocos2d_tests__Bug_15594__

#include "BugsTest.h"

class Bug15594Layer : public BugsTestBase
{
public:
    static Bug15594Layer* create()
    {
        auto ret = new Bug15594Layer;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual bool init() override;
};

#endif /* defined(__cocos2d_tests__Bug_15594__) */
