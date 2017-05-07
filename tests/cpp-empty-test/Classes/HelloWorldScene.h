#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    virtual bool init() override;

    static cocos2d::Scene* scene();

    // a selector callback
    void menuCloseCallback(Ref* sender);

    // implement the "static create()" method manually
    static HelloWorld* create()
    {
        auto ret = new HelloWorld;
        ret->init();
        ret->autorelease();
        return ret;
    }
};

#endif // __HELLOWORLD_SCENE_H__
