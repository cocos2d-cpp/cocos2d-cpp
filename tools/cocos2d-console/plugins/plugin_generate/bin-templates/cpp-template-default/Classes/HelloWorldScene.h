#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    HelloWorld();

    // implement the "static create()" method manually
    static HelloWorld* create()
    {
        auto ret = new HelloWorld;
        ret->autorelease();
        return ret;
    }
};

#endif // __HELLOWORLD_SCENE_H__
