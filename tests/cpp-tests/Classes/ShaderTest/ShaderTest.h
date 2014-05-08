#ifndef _SHADER_TEST_H_
#define _SHADER_TEST_H_

#include "../testBasic.h"
#include "extensions/cocos-ext.h"
#include "../BaseTest.h"

#include "2d/CCGLProgramState.h"

USING_NS_CC_EXT;

class ShaderTestDemo : public BaseTest
{
public:
    ShaderTestDemo(void);

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void restartCallback(Ref* sender);
    void nextCallback(Ref* sender);
    void backCallback(Ref* sender);

    CREATE_FUNC(ShaderTestDemo);
};

class ShaderMonjori : public ShaderTestDemo
{
public:
    ShaderMonjori();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init();
};

class ShaderMandelbrot : public ShaderTestDemo
{
public:
    ShaderMandelbrot();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init();
};

class ShaderJulia : public ShaderTestDemo
{
public:
    ShaderJulia();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init();
};

class ShaderHeart : public ShaderTestDemo
{
public:
    ShaderHeart();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init();
};

class ShaderFlower : public ShaderTestDemo
{
public:
    ShaderFlower();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init();
};

class ShaderPlasma : public ShaderTestDemo
{
public:
    ShaderPlasma();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init();
};

class SpriteBlur;
class ShaderBlur : public ShaderTestDemo
{
public:
    ShaderBlur();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init();
    ControlSlider* createSliderCtl();
    void sliderAction(Ref* sender, Control::EventType controlEvent);
protected:
    SpriteBlur* _blurSprite;
    ControlSlider* _sliderCtl;
};

class ShaderRetroEffect : public ShaderTestDemo
{
public:
    ShaderRetroEffect();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    bool init();
    void update(float dt);
protected:
    Label* _label;
    float           _accum;
};

class ShaderNode : public Node
{
public:
    ShaderNode();
    ~ShaderNode();

    bool initWithVertex(const char *vert, const char *frag);
    void loadShaderVertex(const char *vert, const char *frag);

    virtual void update(float dt);
    virtual void setPosition(const Vector2 &newPosition);
    virtual void draw(Renderer *renderer, const Matrix &transform, bool transformUpdated) override;

    static ShaderNode* shaderNodeWithVertex(const char *vert, const char *frag);

protected:
    void onDraw(const Matrix &transform, bool transformUpdated);

    Vector2 _center;
    Vector2 _resolution;
    float      _time;
    std::string _vertFileName;
    std::string _fragFileName;
    CustomCommand _customCommand;
    GLProgramState *_glProgramState;
};

class ShaderTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

//CCLayer* nextAction();

#endif
