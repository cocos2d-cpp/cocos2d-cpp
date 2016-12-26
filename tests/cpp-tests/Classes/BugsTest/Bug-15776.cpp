//
//  Bug-15776.cpp
//  cocos2d_tests
//
//  Created by Ricardo Quesada on 6/17/16.
//
//

#include "Bug-15776.h"

#include <stdexcept>

using namespace cocos2d;

bool Bug15776Layer::init()
{
    if (BugsTestBase::init())
    {
#if (defined(COCOS2D_DEBUG) && COCOS2D_DEBUG > 0)
        log("Bug15776Layer test is not executed in debug mode");
#else
        cocos2d::Image *cocos2dxImage = new cocos2d::Image();
        cocos2dxImage->initWithImageData(nullptr, 0);
        try {
            auto texture2d = Director::getInstance()
                ->getTextureCache()->addImage(cocos2dxImage, "unused");
            // Exception is expected to be thrown from std::map<>::at()
            // called from Image::isCompressed()
            // called from TextureCache::addImage()
            return false;
        } catch (std::out_of_range &) {
            log("std::out_of_range caught");
            delete cocos2dxImage;
        }
#endif
    }
    return true;
}

std::string Bug15776Layer::title() const
{
    return "Testing Issue #15776";
}

std::string Bug15776Layer::subtitle() const
{
#if (defined(COCOS2D_DEBUG) && COCOS2D_DEBUG > 0)
    return "Bug15776Layer test is not executed in debug mode";
#else
    return "std::out_of_range shall be caught. See console";
#endif
}
