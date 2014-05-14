#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "cocos2d.h"

USING_NS_CC;

class TestController : public Layer
{
public:
    TestController();
    ~TestController();

    void menuCallback(Ref * sender);
    void closeCallback(Ref * sender);

    bool onTouchBegan(Touch* touches, Event  *event);
    void onTouchMoved(Touch* touches, Event  *event);

    void onMouseScroll(Event *event);
    void addConsoleAutoTest();
    void autorun();
    void startAutoRun();
    ssize_t readline(int fd, char* ptr, size_t maxlen);
private:
    Vector2 _beginPos;
    Menu* _itemMenu;
};

#endif
