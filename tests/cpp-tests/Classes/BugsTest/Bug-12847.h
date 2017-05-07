#ifndef __cocos2d_tests__Bug_12847__
#define __cocos2d_tests__Bug_12847__

#include "BugsTest.h"

namespace cocos2d {
class Sprite;
}

class Bug12847Layer : public BugsTestBase
{
public:
    static Bug12847Layer* create()
    {
        auto ret = new Bug12847Layer;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    virtual bool init() override;
    virtual void update(float dt) override;
    
protected:
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    
    cocos2d::Sprite* sprite1;
    cocos2d::Sprite* sprite2;
    cocos2d::Sprite* sprite3;
    cocos2d::Sprite* sprite4;
};

#endif /* defined(__cocos2d_tests__Bug_12847__) */
