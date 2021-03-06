//
// Bug-350 
// http://code.google.com/p/cocos2d-iphone/issues/detail?id=350
//

#include "Bug-350.h"

#include "2d/CCSprite.h"
#include "base/CCDirector.h"

using namespace cocos2d;

bool Bug350Layer::init()
{
    if (BugsTestBase::init())
    {
        auto size = Director::getInstance()->getWinSize();
        auto background = Sprite::create("Hello.png");
        background->setPosition(size.width/2, size.height/2);
        addChild(background);
        return true;
    }

    return false;
}
