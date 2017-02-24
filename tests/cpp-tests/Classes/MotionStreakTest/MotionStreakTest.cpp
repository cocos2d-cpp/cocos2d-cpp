#include "MotionStreakTest.h"
#include "../testResource.h"

#include "2d/CCMenu.h"
#include "2d/CCMenuItem.h"
#include "2d/CCMotionStreak.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"

using namespace cocos2d;

enum {
	kTagLabel = 1,
	kTagSprite1 = 2,
	kTagSprite2 = 3,
};

MotionStreakTests::MotionStreakTests()
{
    ADD_TEST_CASE(MotionStreakTest1);
    ADD_TEST_CASE(MotionStreakTest2);
    ADD_TEST_CASE(Issue1358);
    ADD_TEST_CASE(Issue12226);
}

//------------------------------------------------------------------
//
// MotionStreakTest1
//
//------------------------------------------------------------------

void MotionStreakTest1::onEnter()
{
    MotionStreakTest::onEnter();

    auto s = Director::getInstance()->getWinSize();
  
    // the root object just rotates around
    _root = Sprite::create(s_pathR1);
    addChild(_root, 1);
    _root->setPosition(Vec2(s.width/2, s.height/2));
  
    // the target object is offset from root, and the streak is moved to follow it
    _target = Sprite::create(s_pathR1);
    _root->addChild(_target);
    _target->setPosition(Vec2(s.width/4, 0));

    // create the streak object and add it to the scene
    _streak = MotionStreak::create(2, 3, 32, Color3B::GREEN, s_streak);
    addChild(_streak);
    // schedule an update on each frame so we can syncronize the streak with the target
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, 0, &MotionStreakTest1::onUpdate)
            .delay(5.0f)
            .interval(5.0f)
            .paused(isPaused())
    );
  
    auto a1 = RotateBy::create(2, 360);

    auto action1 = RepeatForever::create(a1);
    auto motion = MoveBy::create(2, Vec2(100,0) );
    auto motion_reverse = motion->reverse();
    _root->runAction(
        RepeatForever::create(
            Sequence::create(
                to_action_ptr(motion),
                to_action_ptr(motion_reverse)
            )
        )
    );
    _root->runAction( action1 );

    auto colorAction = RepeatForever::create(
        Sequence::create(
            to_action_ptr(TintTo::create(0.2f, 255, 0, 0)),
            to_action_ptr(TintTo::create(0.2f, 0, 255, 0)),
            to_action_ptr(TintTo::create(0.2f, 0, 0, 255)),
            to_action_ptr(TintTo::create(0.2f, 0, 255, 255)),
            to_action_ptr(TintTo::create(0.2f, 255, 255, 0)),
            to_action_ptr(TintTo::create(0.2f, 255, 0, 255)),
            to_action_ptr(TintTo::create(0.2f, 255, 255, 255))
        )
    );

    _streak->runAction(colorAction);
}

void MotionStreakTest1::onUpdate(float /*delta*/)
{
    _streak->setPosition( _target->convertToWorldSpace(Vec2::ZERO) );
}

std::string MotionStreakTest1::title() const
{
    return "MotionStreak test 1";
}

//------------------------------------------------------------------
//
// MotionStreakTest2
//
//------------------------------------------------------------------

void MotionStreakTest2::onEnter()
{
    MotionStreakTest::onEnter();

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesMoved = CC_CALLBACK_2(MotionStreakTest2::onTouchesMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto s = Director::getInstance()->getWinSize();
        
    // create the streak object and add it to the scene
    _streak = MotionStreak::create(3, 3, 64, Color3B::WHITE, s_streak );
    addChild(_streak);
    
    _streak->setPosition( Vec2(s.width/2, s.height/2) );
}

void MotionStreakTest2::onTouchesMoved(const std::vector<Touch*>& touches, Event*)
{
    auto touchLocation = touches[0]->getLocation();
    
    _streak->setPosition( touchLocation );
}

std::string MotionStreakTest2::title() const
{
    return "MotionStreak test";
}

//------------------------------------------------------------------
//
// Issue1358
//
//------------------------------------------------------------------

void Issue1358::onEnter()
{
    MotionStreakTest::onEnter();
    
    // ask director the the window size
    auto size = Director::getInstance()->getWinSize();
    
    _streak = MotionStreak::create(2.0f, 1.0f, 50.0f, Color3B(255, 255, 0), "Images/Icon.png");
    addChild(_streak);
    
    
    _center  = Vec2(size.width/2, size.height/2);
    _radius = size.width/3;
    _angle = 0.0f;
    
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, 0, &Issue1358::update)
            .delay(5.0f)
            .interval(5.0f)
            .paused(isPaused())
    );
}

void Issue1358::update(float)
{
    _angle += 1.0f;
    _streak->setPosition(Vec2(_center.x + cosf(_angle/180 * M_PI)*_radius,
                            _center.y + sinf(_angle/ 180 * M_PI)*_radius));
}

std::string Issue1358::title() const
{
    return "Issue 1358";
}

std::string Issue1358::subtitle() const
{
    return "The tail should use the texture";
}

//------------------------------------------------------------------
//
// Issue12226
//
//------------------------------------------------------------------

void Issue12226::onEnter()
{
    MotionStreakTest::onEnter();

    // ask director the the window size
    auto size = Director::getInstance()->getWinSize();

    auto radius = size.width/3;

    auto outer = Sprite::create("Images/grossini.png");
    outer->setPosition(size/2);
    addChild(outer);


    _streak = MotionStreak::create(1.0f, 3, radius * 1.5f, Color3B(0xA0, 0xA0, 0xA0), "ccb/particle-smoke.png");
//    motionStreak->setOpacity(0x70);
    _streak->setPosition(outer->getPosition());

    this->addChild(_streak, outer->getLocalZOrder() - 1);

    outer->setUserData(_streak);

    const uint32_t length = (radius * 0.95);

    std::function<void(float)> updateMotionStreak = [=](float /*dt*/) {

        Vec2 position = Vec2(outer->getPositionX() + length * cosf(-1 * CC_DEGREES_TO_RADIANS(outer->getRotation() + 90.0f)),
                             outer->getPositionY() + length * sinf(-1 * CC_DEGREES_TO_RADIANS(outer->getRotation() + 90.0f)));

        _streak->setPosition(position);
    };

    Director::getInstance()->getScheduler().schedule(
        TimedJob(outer, 0, updateMotionStreak) 
            .delay(1 / 240.0f)
            .interval(1 / 240.0f)
            .paused( outer->isPaused() )
    );

    auto rot = RotateBy::create(2, 360);
    auto forever = RepeatForever::create(rot);
    outer->runAction(forever);

}

std::string Issue12226::title() const
{
    return "Github Issue 12226";
}

std::string Issue12226::subtitle() const
{
    return "Image should look without artifacts";
}

//------------------------------------------------------------------
//
// MotionStreakTest
//
//------------------------------------------------------------------

MotionStreakTest::MotionStreakTest(void)
{
}

MotionStreakTest::~MotionStreakTest(void)
{
}

std::string MotionStreakTest::title() const
{
    return "No title";
}

std::string MotionStreakTest::subtitle() const
{
    return "";
}

void MotionStreakTest::onEnter()
{
    TestCase::onEnter();

    auto s = Director::getInstance()->getWinSize();

    MenuItemToggle::items_container menuItems;
    menuItems.reserve(2);

    menuItems.push_back(
        to_node_ptr(
            static_cast<MenuItem*>(MenuItemFont::create(
                    "Use High Quality Mode"
            ))
        )
    );

    menuItems.push_back(
        to_node_ptr(
            static_cast<MenuItem*>(MenuItemFont::create(
                    "Use Fast Mode"
            ))
        )
    );

    auto itemMode = MenuItemToggle::createWithCallback(
        CC_CALLBACK_1(MotionStreakTest::modeCallback, this),
        std::move(menuItems)
    );

    auto menuMode = Menu::create(itemMode, nullptr);
    addChild(menuMode);

    menuMode->setPosition(Vec2(s.width/2, s.height/4));
}

void MotionStreakTest::modeCallback(Ref *)
{
    bool fastMode = _streak->isFastMode();
    _streak->setFastMode(! fastMode);
}
