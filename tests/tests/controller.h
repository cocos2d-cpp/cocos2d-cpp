#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "cocos2d.h"
#include "tests.h"

using namespace cocos2d;

class TestController : public CCLayer
{
public:
    TestController();
    ~TestController();

    void menuCallback(CCObject * pSender);
    void closeCallback(CCObject * pSender);

    virtual void ccTouchesBegan(CCSet *pTouches, UIEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, UIEvent *pEvent);

private:

    CCPoint m_tBeginPos;
    CCMenuItemLabel * m_pMenuItems[TESTS_COUNT];
    CCMenu* m_pItmeMenu;
};

#endif
