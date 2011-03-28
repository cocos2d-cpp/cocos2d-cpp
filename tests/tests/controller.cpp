#include "controller.h"
#include "testResource.h"

#define LINE_SPACE          40

static CCPoint s_tCurPos = CCPointZero;

static TestScene* CreateTestScene(int nIdx)
{
    TestScene* pScene = NULL;

    switch (nIdx)
    {
    case TEST_ACTIONS:
        pScene = new ActionsTestScene(); break;
    case TEST_TRANSITIONS:
        pScene = new TransitionsTestScene(); break;
    case TEST_PROGRESS_ACTIONS:
        pScene = new ProgressActionsTestScene(); break;
    case TEST_EFFECTS:
        pScene = new EffectTestScene(); break;
    case TEST_CLICK_AND_MOVE:
        pScene = new ClickAndMoveTestScene(); break;
    case TEST_ROTATE_WORLD:
        pScene = new RotateWorldTestScene(); break;
    case TEST_PARTICLE:
        pScene = new ParticleTestScene(); break;
    case TEST_EASE_ACTIONS:
        pScene = new EaseActionsTestScene(); break;
    case TEST_MOTION_STREAK:
        pScene = new MotionStreakTestScene(); break;
    case TEST_DRAW_PRIMITIVES:
        pScene = new DrawPrimitivesTestScene(); break;
    case TEST_COCOSNODE:
        pScene = new CocosNodeTestScene(); break;
    case TEST_TOUCHES:
        CCDirector::sharedDirector()->setDeviceOrientation(CCDeviceOrientationPortrait);
        pScene = new PongScene(); break;
    case TEST_MENU:
        pScene = new MenuTestScene(); break;
    case TEST_ACTION_MANAGER:
        pScene = new ActionManagerTestScene(); break;
    case TEST_LAYER:
        pScene = new LayerTestScene(); break;
    case TEST_SCENE:
        pScene = new SceneTestScene(); break;
    case TEST_PARALLAX:
        pScene = new ParallaxTestScene(); break;
    case TEST_TILE_MAP:
        pScene = new TileMapTestScene(); break;
    case TEST_INTERVAL:
        pScene = new IntervalTestScene(); break;
    case TEST_CHIPMUNK:
#if (CC_TARGET_PLATFORM != CC_PLATFORM_AIRPLAY)
		CCDirector::sharedDirector()->setDeviceOrientation(CCDeviceOrientationPortrait);
        pScene = new ChipmunkTestScene(); break;
#endif
    case TEST_ATLAS:
        pScene = new AtlasTestScene(); break;
    case TEST_SPRITE:
        pScene = new SpriteTestScene(); break;
    case TEST_SCHEDULER:
        pScene = new SchedulerTestScene(); break;
    case TEST_RENDERTEXTURE:
        pScene = new RenderTextureScene(); break;
    case TEST_BOX2D:
        pScene = new Box2DTestScene(); break;
    case TEST_BOX2DBED:
        pScene = new Box2dTestBedScene(); break;
    case TEST_EFFECT_ADVANCE:
        pScene = new EffectAdvanceScene(); break;
    case TEST_HIRES:
        pScene = new HiResTestScene(); break;
#ifndef _WIN32
	case TEST_ACCELEROMRTER:
        pScene = new AccelerometerTestScene(); break;
#endif
    case TEST_KEYPAD:
        pScene = new KeypadTestScene(); break;
	case TEST_COCOSDENSHION:
#if (CC_TARGET_PLATFORM != CC_PLATFORM_AIRPLAY)
		pScene = new CocosDenshionTestScene(); break;
#endif
    case TEST_PERFORMANCE:
        pScene = new PerformanceTestScene(); break;
    default:
        break;
    }

    return pScene;
}

TestController::TestController()
: m_tBeginPos(CCPointZero)
{
    CCDirector::sharedDirector()->setDeviceOrientation(CCDeviceOrientationLandscapeLeft);

    // add close menu
    CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(s_pPathClose, s_pPathClose, this, menu_selector(TestController::closeCallback) );
    CCMenu* pMenu =CCMenu::menuWithItems(pCloseItem, NULL);

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    pMenu->setPosition( CCPointZero );
    pCloseItem->setPosition(CCPointMake( s.width - 30, s.height - 30));

    // add menu items for tests
    m_pItmeMenu = CCMenu::menuWithItems(NULL);
    for (int i = 0; i < TESTS_COUNT; ++i)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_AIRPLAY)
		CCLabelBMFont* label = CCLabelBMFont::bitmapFontAtlasWithString(g_aTestNames[i].c_str(),  "fonts/arial16.fnt");
#else
        CCLabelTTF* label = CCLabelTTF::labelWithString(g_aTestNames[i].c_str(), "Arial", 24);
#endif		
        CCMenuItemLabel* pMenuItem = CCMenuItemLabel::itemWithLabel(label, this, menu_selector(TestController::menuCallback));

        m_pItmeMenu->addChild(pMenuItem, i + 10000);
        pMenuItem->setPosition( CCPointMake( s.width / 2, (s.height - (i + 1) * LINE_SPACE) ));

        // record the pointer of the menu item
        m_pMenuItems[i] = pMenuItem;
    }

    m_pItmeMenu->setContentSize(CCSizeMake(s.width, (TESTS_COUNT + 1) * (LINE_SPACE)));
    m_pItmeMenu->setPosition(s_tCurPos);
    addChild(m_pItmeMenu);

    setIsTouchEnabled(true);

    addChild(pMenu, 1);
}

TestController::~TestController()
{
}

void TestController::menuCallback(CCObject * pSender)
{
    // get the userdata, it's the index of the menu item clicked
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int nIdx = pMenuItem->getZOrder() - 10000;

    // create the test scene and run it
    TestScene* pScene = CreateTestScene(nIdx);
    if (pScene)
    {
        pScene->runThisTest();
        pScene->release();
    }
}

void TestController::closeCallback(CCObject * pSender)
{
    CCDirector::sharedDirector()->end();
}

void TestController::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    m_tBeginPos = touch->locationInView( touch->view() );	
    m_tBeginPos = CCDirector::sharedDirector()->convertToGL( m_tBeginPos );
}

void TestController::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    float nMoveY = touchLocation.y - m_tBeginPos.y;

    CCPoint curPos  = m_pItmeMenu->getPosition();
    CCPoint nextPos = ccp(curPos.x, curPos.y + nMoveY);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (nextPos.y < 0.0f)
    {
        m_pItmeMenu->setPosition(CCPointZero);
        return;
    }

    if (nextPos.y > ((TESTS_COUNT + 1)* LINE_SPACE - winSize.height))
    {
        m_pItmeMenu->setPosition(ccp(0, ((TESTS_COUNT + 1)* LINE_SPACE - winSize.height)));
        return;
    }

    m_pItmeMenu->setPosition(nextPos);
    m_tBeginPos = touchLocation;
    s_tCurPos   = nextPos;
}
