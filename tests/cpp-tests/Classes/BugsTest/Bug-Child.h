//
//  Bug-Child.h
//  cocos2d_tests
//
//  Created by NiTe Luo on 5/12/14.
//
//

#ifndef __Bug_Child__
#define __Bug_Child__

#include "BugsTest.h"

namespace cocos2d {
class Menu;
class Sprite;
}

class BugChild : public BugsTestBase
{
public:
    static BugChild* create()
    {
        auto ret = new BugChild;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual bool init() override;
    
    void switchChild(cocos2d::Ref* sender);
    
protected:
    
    cocos2d::Sprite* parent1;
    cocos2d::Sprite* parent2;
    
    cocos2d::Sprite* child;
    
    cocos2d::Menu* menu;
};

class BugCameraMask : public BugsTestBase
{
public:
    static BugCameraMask* create()
    {
        auto ret = new BugCameraMask;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    virtual bool init() override;
    
    void switchSpriteFlag(cocos2d::Ref* sender);
    void updateSpriteMaskLabel();
    Node* _sprite;
    cocos2d::Label* _spriteMaskLabel;
};

#endif /* defined(__Bug_Child__) */
