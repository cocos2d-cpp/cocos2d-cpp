#include "PerformanceLabelTest.h"
#include "Profile.h"

using namespace cocos2d;

#define DELAY_TIME              1
#define STAT_TIME               3

enum {
    kMaxNodes = 200,
    kInitNodeCount = 20,
    kNodesIncrease = 10,
};

enum {
    kTagInfoLayer = 1,
    kTagMainLayer,
    kTagAutoTestMenu,
    kTagMenuLayer = (kMaxNodes + 1000),
};

enum {
    kCaseLabelTTFUpdate = 0,
    kCaseLabelBMFontUpdate,
    kCaseLabelUpdate,
    kCaseLabelBMFontBigLabels,
    kCaseLabelBigLabels,
    
    kCaseCount
};

static int _curTestCase = kCaseLabelTTFUpdate;
static int _autoTestNodeCounts[] = {
    50, 100
};

#define LongSentencesExample "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\
Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\
Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."

PerformceLabelTests::PerformceLabelTests()
{
    _curTestCase = kCaseLabelTTFUpdate;
    addTestCase("LabelTTF Performance Test", [](){ return LabelMainScene::create(); });
    addTestCase("Label with BMFont Performance Test", [](){ return LabelMainScene::create(); });
    addTestCase("Label Performance Test", [](){ return LabelMainScene::create(); });
    addTestCase("Label with BMFont large text Performance", [](){ return LabelMainScene::create(); });
    addTestCase("Label large text Performance", [](){ return LabelMainScene::create(); });
}

////////////////////////////////////////////////////////
//
// LabelMainScene
//
////////////////////////////////////////////////////////
bool LabelMainScene::init()
{
    //std::srandom(0);
    if (!TestCase::init())
    {
        return false;
    }

    auto s = Director::getInstance()->getWinSize();

    _lastRenderedCount = 0;
    _quantityNodes = 0;
    _accumulativeTime = 0.0f;

    _labelContainer = Layer::create();
    addChild(_labelContainer);

    MenuItemFont::setFontSize(65);
    auto decrease = MenuItemFont::create(" - ", CC_CALLBACK_1(LabelMainScene::onDecrease, this));
    decrease->setColor(Color3B(0,200,20));
    auto increase = MenuItemFont::create(" + ", CC_CALLBACK_1(LabelMainScene::onIncrease, this));
    increase->setColor(Color3B(0,200,20));

    auto menu = Menu::create(decrease, increase, nullptr);
    menu->alignItemsHorizontally();
    menu->setPosition(Vec2(s.width/2, s.height-65));
    addChild(menu, 1);

    auto infoLabel = Label::createWithTTF("0 nodes", "fonts/Marker Felt.ttf", 30);
    infoLabel->setColor(Color3B(0,200,20));
    infoLabel->setPosition(Vec2(s.width/2, s.height-90));
    addChild(infoLabel, 1, kTagInfoLayer);

    while (_quantityNodes < kInitNodeCount)
        onIncrease(this);

    return true;
}

std::string LabelMainScene::title() const
{
    switch (_curTestCase)
    {
    case kCaseLabelTTFUpdate:
        return "Testing LabelTTF Update";
    case kCaseLabelBMFontUpdate:
        return "Testing Label with BMFont Update";
    case kCaseLabelUpdate:
        return "Testing Label Update";
    case kCaseLabelBMFontBigLabels:
        return "Testing Label with BMFont Big Labels";
    case kCaseLabelBigLabels:
        return "Testing Label Big Labels";
    default:
        break;
    }
    return "No title";
}

LabelMainScene::~LabelMainScene()
{
    
}

void LabelMainScene::updateNodes()
{
    if( _quantityNodes != _lastRenderedCount )
    {
        auto infoLabel = (Label *) getChildByTag(kTagInfoLayer);
        char str[16] = {0};
        sprintf(str, "%u nodes", _quantityNodes);
        infoLabel->setString(str);

        _lastRenderedCount = _quantityNodes;
    }
}

void LabelMainScene::onIncrease(Ref*)
{    
    if( _quantityNodes >= kMaxNodes)
        return;

    auto size = Director::getInstance()->getWinSize();
    switch (_curTestCase)
    {
    case kCaseLabelTTFUpdate:
        for( int i=0;i< kNodesIncrease;i++)
        {
            auto label = Label::createWithSystemFont("LabelTTF", "Marker Felt", 30);
            label->setPosition(Vec2((size.width/2 + rand() % 50), ((int)size.height/2 + rand() % 50)));
            _labelContainer->addChild(label, 1, _quantityNodes);

            _quantityNodes++;
        }
        break;
    case kCaseLabelBMFontUpdate:
        for( int i=0;i< kNodesIncrease;i++)
        {
            auto label = Label::createWithBMFont("fonts/bitmapFontTest3.fnt","Label with BMFont");
            label->setPosition(Vec2((size.width/2 + rand() % 50), ((int)size.height/2 + rand() % 50)));
            _labelContainer->addChild(label, 1, _quantityNodes);

            _quantityNodes++;
        }
        break;
    case kCaseLabelUpdate:
        {
            TTFConfig ttfConfig("fonts/arial.ttf", 60, GlyphCollection::DYNAMIC);
            for( int i=0;i< kNodesIncrease;i++)
            {
                auto label = Label::createWithTTF(ttfConfig, "Label", TextHAlignment::LEFT);
                label->setPosition(Vec2((size.width/2 + rand() % 50), ((int)size.height/2 + rand() % 50)));
                _labelContainer->addChild(label, 1, _quantityNodes);

                _quantityNodes++;
            }
            break;
        }        
    case kCaseLabelBMFontBigLabels:
        for( int i=0;i< kNodesIncrease;i++)
        {
            auto label = Label::createWithBMFont("fonts/bitmapFontTest3.fnt", LongSentencesExample);
            label->setPosition(Vec2((size.width/2 + rand() % 50), ((int)size.height/2 + rand() % 50)));
            _labelContainer->addChild(label, 1, _quantityNodes);

            _quantityNodes++;
        }
        break;
    case kCaseLabelBigLabels:
        {
            TTFConfig ttfConfig("fonts/arial.ttf", 60, GlyphCollection::DYNAMIC);
            for( int i=0;i< kNodesIncrease;i++)
            {
                auto label = Label::createWithTTF(ttfConfig, LongSentencesExample, TextHAlignment::CENTER, size.width);
                label->setPosition(Vec2((rand() % 50), rand()%((int)size.height/3)));
                _labelContainer->addChild(label, 1, _quantityNodes);

                _quantityNodes++;
            }
            break;
        }        
    default:
        break;
    }

    updateNodes();
}

void LabelMainScene::onDecrease(Ref*)
{
    if( _quantityNodes <= 0 )
        return;

    for( int i = 0;i < kNodesIncrease;i++)
    {
        _quantityNodes--;
        _labelContainer->removeChildByTag(_quantityNodes);
    }

    updateNodes();
}

void LabelMainScene::updateText(float dt)
{
    if (isStating)
    {
        totalStatTime += dt;
        statCount++;
        
        auto curFrameRate = Director::getInstance()->getFrameRate();
        if (maxFrameRate < 0 || curFrameRate > maxFrameRate)
            maxFrameRate = curFrameRate;
        
        if (minFrameRate < 0 || curFrameRate < minFrameRate)
            minFrameRate = curFrameRate;
    }

    if(_curTestCase > kCaseLabelUpdate)
        return;

    _accumulativeTime += dt;
    char text[20];
    sprintf(text,"%.2f",_accumulativeTime);

    auto& children = _labelContainer->getChildren();
    switch (_curTestCase)
    {
    case kCaseLabelTTFUpdate:
        for(auto & child : children) {
            Label* label = dynamic_cast<Label*>(child.get());
            label->setString(text);
        }
        break;
    case kCaseLabelBMFontUpdate:
        for(auto &child : children) {
            auto label = dynamic_cast<Label*>(child.get());
            label->setString(text);
        }
        break;
    case kCaseLabelUpdate:
        for(const auto &child : children) {
            Label* label = dynamic_cast<Label*>(child.get());
            label->setString(text);
        }
        break;
    default:
        break;
    }
}

void LabelMainScene::onEnter()
{
    Scene::onEnter();
    
    auto director = Director::getInstance();
    auto & sched = director->getScheduler();
    sched.schedule(TimedJob(this, &LabelMainScene::updateText));

    if (this->isAutoTesting()) {
        Profile::getInstance()->testCaseBegin("LabelTest",
                                              genStrVector("Type", "LabelCount", nullptr),
                                              genStrVector("Avg", "Min", "Max", nullptr));
        autoTestIndex = 0;
        doAutoTest();
    }
}

void LabelMainScene::onExit()
{
    auto director = Director::getInstance();
    auto & sched = director->getScheduler();
    sched.unscheduleAllForTarget(this);

    Scene::onExit();
}

void LabelMainScene::doAutoTest()
{
    isStating = false;
    statCount = 0;
    totalStatTime = 0.0f;
    minFrameRate = -1.0f;
    maxFrameRate = -1.0f;
    
    // remove all labels
    _labelContainer->removeAllChildren();
    _quantityNodes = 0;

    // add labels
    int newCount = _autoTestNodeCounts[autoTestIndex];
    while (_quantityNodes < newCount) {
        onIncrease(this);
    }

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &LabelMainScene::beginStat, 1)
            .delay(DELAY_TIME)
            .interval(DELAY_TIME)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &LabelMainScene::endStat, 2)
            .delay(DELAY_TIME + STAT_TIME)
            .interval(DELAY_TIME + STAT_TIME)
            .paused(isPaused())
    );
}

void LabelMainScene::beginStat(float /*dt*/)
{
    Director::getInstance()->getScheduler().unscheduleTimedJob(this, 1);
    isStating = true;
}

void LabelMainScene::endStat(float /*dt*/)
{
    Director::getInstance()->getScheduler().unscheduleTimedJob(this, 2);
    isStating = false;
    
    // record test data
    std::string tf;
    switch (_curTestCase)
    {
        case kCaseLabelTTFUpdate:
            tf = "LabelTTF";
            break;
        case kCaseLabelBMFontUpdate:
            tf = "Label with BMFont";
            break;
        case kCaseLabelUpdate:
            tf = "Label";
            break;
        case kCaseLabelBMFontBigLabels:
            tf = "Label with BMFont Big Labels";
            break;
        case kCaseLabelBigLabels:
            tf = "Label Big Labels";
            break;
        default:
            tf = "unknown";
            break;
    }
    auto avgStr = genStr("%.2f", (float) statCount / totalStatTime);
    Profile::getInstance()->addTestResult(genStrVector(tf.c_str(), genStr("%d", _quantityNodes).c_str(), nullptr),
                                          genStrVector(avgStr.c_str(), genStr("%.2f", minFrameRate).c_str(),
                                                       genStr("%.2f", maxFrameRate).c_str(), nullptr));

    // check the auto test is end or not
    int autoTestCount = sizeof(_autoTestNodeCounts) / sizeof(int);
    if (autoTestIndex >= (autoTestCount - 1))
    {
        // auto test end
        Profile::getInstance()->testCaseEnd();
        _curTestCase++;
        setAutoTesting(false);
        return;
    }

    autoTestIndex++;
    doAutoTest();
}

void LabelMainScene::nextTestCallback(cocos2d::Ref* sender)
{
    _curTestCase = (_curTestCase + 1) % kCaseCount;
    TestCase::nextTestCallback(sender);
}

void LabelMainScene::priorTestCallback(cocos2d::Ref* sender)
{
    if (_curTestCase > 0)
    {
        _curTestCase -= 1;
    } 
    else
    {
        _curTestCase = kCaseCount - 1;
    }
    TestCase::priorTestCallback(sender);
}
