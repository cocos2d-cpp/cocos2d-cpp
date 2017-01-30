#include "MutiTouchTest.h"

#include "2d/CCDrawNode.h"
#include "2d/CCLabel.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"

#include <unordered_map>

using namespace cocos2d;

MutiTouchTests::MutiTouchTests()
{
    ADD_TEST_CASE(MutiTouchTest);
}

static const Color3B* s_TouchColors[5] = {
    &Color3B::YELLOW,
    &Color3B::BLUE,
    &Color3B::GREEN,
    &Color3B::RED,
    &Color3B::MAGENTA
};

class TouchPoint : public Node
{
public:
    TouchPoint(const Vec2 &touchPoint, const Color3B &touchColor)
    {
        DrawNode* drawNode = DrawNode::create();
        auto s = Director::getInstance()->getWinSize();
        Color4F color(touchColor.r/255.0f, touchColor.g/255.0f, touchColor.b/255.0f, 1.0f);
        drawNode->drawLine(Vec2(0, touchPoint.y), Vec2(s.width, touchPoint.y), color);
        drawNode->drawLine(Vec2(touchPoint.x, 0), Vec2(touchPoint.x, s.height), color);
        drawNode->drawDot(touchPoint, 3, color);
        addChild(drawNode);
    }

    static TouchPoint* touchPointWithParent(Node* pParent, const Vec2 &touchPoint, const Color3B &touchColor)
    {
        auto pRet = new (std::nothrow) TouchPoint(touchPoint, touchColor);
        pRet->setContentSize(pParent->getContentSize());
        pRet->setAnchorPoint(Vec2(0.0f, 0.0f));
        pRet->autorelease();
        return pRet;
    }
};

bool MutiTouchTest::init()
{
    if (TestCase::init())
    {
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesBegan = CC_CALLBACK_2(MutiTouchTest::onTouchesBegan, this);
        listener->onTouchesMoved = CC_CALLBACK_2(MutiTouchTest::onTouchesMoved, this);
        listener->onTouchesEnded = CC_CALLBACK_2(MutiTouchTest::onTouchesEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        auto title = Label::createWithSystemFont("Please touch the screen!", "", 24);
        title->setPosition(VisibleRect::top()+Vec2(0, -40));
        addChild(title);
        
        return true;
    }
    return false;
}

static std::unordered_map<int, node_ptr<TouchPoint>> s_map;

void MutiTouchTest::onTouchesBegan(const std::vector<Touch*>& touches, Event*)
{
    for ( auto &item: touches )
    {
        auto touch = item;
        auto location = touch->getLocation();
        auto touchPoint = to_node_ptr(TouchPoint::touchPointWithParent(this, location, *s_TouchColors[touch->getID()%5]));

        addChild(touchPoint.get());
        s_map[ touch->getID() ] = std::move(touchPoint);
    }
}

void MutiTouchTest::onTouchesMoved(const std::vector<Touch*>& touches, Event  * /*event*/)
{
    for( auto &item: touches)
    {
        auto touch = item;
        auto location = touch->getLocation();

        auto & pTP = s_map[touch->getID()];

        if (pTP)
        {
            removeChild(pTP.get(), true);
        }
        
        pTP = to_node_ptr(
            TouchPoint::touchPointWithParent(
                this,
                location,
                *s_TouchColors[touch->getID()%5]
            ));

        addChild(pTP.get());
    }
}

void MutiTouchTest::onTouchesEnded(const std::vector<Touch*>& touches, Event  * /*event*/)
{
    for ( auto &item: touches )
    {
        auto touch = item;
        auto pTP = s_map.find(touch->getID());
        if (pTP != s_map.end())
        {
            removeChild(pTP->second.get(), true);
            s_map.erase(pTP);
        }
    }
}

void MutiTouchTest::onTouchesCancelled(const std::vector<Touch*>& touches, Event  *event)
{
    onTouchesEnded(touches, event);
}
