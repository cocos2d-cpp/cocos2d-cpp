#ifndef _PADDLE_H_
#define _PADDLE_H_

#include "cocos2d.h" // TODO remove

typedef enum tagPaddleState 
{
    kPaddleStateGrabbed,
    kPaddleStateUngrabbed
} PaddleState; 

class Paddle : public cocos2d::Sprite
{
    PaddleState        _state;

public:
    Paddle(void);
    virtual ~Paddle(void);

    cocos2d::Rect getRect();
    bool initWithTexture(const cocos2d::Texture2D* aTexture) override;
    virtual void onEnter() override;
    virtual void onExit() override;
    bool containsTouchLocation(cocos2d::Touch* touch);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    Paddle* clone() const;

    static Paddle* createWithTexture(const cocos2d::Texture2D* aTexture);
};

#endif
