#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    static HelloWorld* create()
    {
        auto ret = new HelloWorld;
        ret->autorelease();
        return ret;
    }
};

#endif // __HELLOWORLD_SCENE_H__
