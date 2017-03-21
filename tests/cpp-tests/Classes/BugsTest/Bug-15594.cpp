//
//  Bug-15594.cpp
//  cocos2d_tests
//
//  Created by Ricardo Quesada on 5/24/16.
//
//

// https://github.com/cocos2d/cocos2d-x/pull/15594

#include "Bug-15594.h"

#include "3d/CCSprite3D.h"
#include "3d/CCAnimation3D.h"
#include "3d/CCAnimate3D.h"
#include "base/CCDirector.h"

using namespace cocos2d;

bool Bug15594Layer::init()
{
    if (BugsTestBase::init())
    {
        auto sprite3d = Sprite3D::create("Images/bugs/bug15594.c3t", "Images/bugs/bug15594.jpg");
        addChild(sprite3d);
        auto size = Director::getInstance()->getWinSize();
        sprite3d->setPosition(size/2);

        auto animation = Animation3D::create("Images/bugs/bug15594.c3t");
        auto animate = std::make_unique<Animate3D>(animation);
        auto repeate = std::make_unique<RepeatForever>(std::move(animate));
        sprite3d->runAction(std::move(repeate));
        return true;
    }

    return false;
}
