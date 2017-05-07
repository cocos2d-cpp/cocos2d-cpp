#ifndef __PERFORMANCE_TEXTURE_TEST_H__
#define __PERFORMANCE_TEXTURE_TEST_H__

#include "BaseTest.h"

DEFINE_TEST_SUITE(PerformceTextureTests);

class TexturePerformceTest : public TestCase
{
public:
    static TexturePerformceTest* create()
    {
        auto ret = new TexturePerformceTest;
        ret->autorelease();
        return ret;
    }

    virtual void performTests();
    void performTestsPNG(const char* filename, const char* fileType, const char* resolution, const char* remark);

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

#endif
