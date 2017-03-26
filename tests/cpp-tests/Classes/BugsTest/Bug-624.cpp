//
// Bug-624
// http://code.google.com/p/cocos2d-iphone/issues/detail?id=624
//

#include "Bug-624.h"

#include "2d/CCLabel.h"
#include "2d/CCTransition.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerAcceleration.h"
#include "platform/CCDevice.h"

using namespace cocos2d;

////////////////////////////////////////////////////////
//
// Bug624Layer
//
////////////////////////////////////////////////////////
Bug624Layer::~Bug624Layer()
{
    Device::setAccelerometerEnabled(false);
}

bool Bug624Layer::init()
{
    if(BugsTestBase::init())
    {
        auto size = Director::getInstance()->getWinSize();
        auto label = Label::createWithTTF("Layer1", "fonts/Marker Felt.ttf", 36.0f);

        label->setPosition(size.width/2, size.height/2);
        addChild(label);
        
        Device::setAccelerometerEnabled(true);
        auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(Bug624Layer::onAcceleration,  this));
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

        Director::getInstance()->getScheduler().schedule(
            TimedJob(this, &Bug624Layer::switchLayer, 0)
                .delay(5.0f)
                .interval(5.0f)
                .paused(isPaused())
        );

        return true;
    }
    
    return false;
}

void Bug624Layer::switchLayer(float)
{
    Director::getInstance()->getScheduler().unscheduleTimedJob(this, 0);

    auto scene = Scene::create();    
    scene->addChild(Bug624Layer2::create(), 0);
    Director::getInstance()->replaceScene( to_node_ptr(TransitionFade::create(2.0f, scene, Color3B::WHITE)) );
}

void Bug624Layer::onAcceleration(Acceleration*, Event*)
{    
    log("Layer1 accel");
}

////////////////////////////////////////////////////////
//
// Bug624Layer2
//
////////////////////////////////////////////////////////
Bug624Layer2::~Bug624Layer2()
{
    Device::setAccelerometerEnabled(false);
}

bool Bug624Layer2::init()
{
    if(BugsTestBase::init())
    {
        auto size = Director::getInstance()->getWinSize();
        auto label = Label::createWithTTF("Layer2", "fonts/Marker Felt.ttf", 36.0f);

        label->setPosition(size.width/2, size.height/2);
        addChild(label);
        
        Device::setAccelerometerEnabled(true);
        auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(Bug624Layer2::onAcceleration, this));
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        Director::getInstance()->getScheduler().schedule(
            TimedJob(this, &Bug624Layer2::switchLayer, 0)
                .delay(5.0f)
                .interval(5.0f)
                .paused(isPaused())
        );

        return true;
    }

    return false;
}

void Bug624Layer2::switchLayer(float)
{
    Director::getInstance()->getScheduler().unscheduleTimedJob(this, 0);

    auto scene = Scene::create();    
    scene->addChild(Bug624Layer::create(), 0);
    Director::getInstance()->replaceScene( to_node_ptr(TransitionFade::create(2.0f, scene, Color3B::RED)) );
}

void Bug624Layer2::onAcceleration(Acceleration*, Event*)
{    
    log("Layer2 accel");
}
