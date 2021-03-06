/****************************************************************************
Copyright (c) 2013-2016 Chukong Technologies Inc.
Copyright (c) 2017      Iakov Sergeev <yahont@github>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "ui/UISlider.h"

#include "2d/CCCamera.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCTouch.h"
#include "base/CCDirector.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIHelper.h"

namespace cocos2d {

namespace ui {
    
static const int BASEBAR_RENDERER_Z = (-2);
static const int PROGRESSBAR_RENDERER_Z = (-2);
static const int SLIDBALL_RENDERER_Z = (-1);

static inline Sprite* getSlidBallSprite(NodeId nodeId)
{
    auto & nodes = Director::getInstance()->getNodeRegister();
    auto slidBallSprite = nodes.get<Sprite>(nodeId);

    assert(slidBallSprite != nullptr);

    if (slidBallSprite == nullptr)
    {
        throw std::runtime_error("No sprite for slidBall id");
    }

    return slidBallSprite;
}
    
IMPLEMENT_CLASS_GUI_INFO(Slider)
    
Slider::Slider():
_barRenderer(nullptr),
_progressBarRenderer(nullptr),
_barTextureSize(Size::ZERO),
_progressBarTextureSize(Size::ZERO),
_slidBallNormalRendererId(),
_slidBallPressedRendererId(),
_slidBallDisabledRendererId(),
_slidBallRenderer(nullptr),
_barLength(0.0),
_percent(0),
_maxPercent(100),
_scale9Enabled(false),
_prevIgnoreSize(true),
_zoomScale(0.1f),
_sliderBallNormalTextureScaleX(1.0),
_sliderBallNormalTextureScaleY(1.0),
_isSliderBallPressedTextureLoaded(false),
_isSliderBallDisabledTexturedLoaded(false),
_capInsetsBarRenderer(Rect::ZERO),
_capInsetsProgressBarRenderer(Rect::ZERO),
_sliderEventListener(nullptr),
_eventCallback(nullptr),
_barTexType(TextureResType::LOCAL),
_progressBarTexType(TextureResType::LOCAL),
_ballNTexType(TextureResType::LOCAL),
_ballPTexType(TextureResType::LOCAL),
_ballDTexType(TextureResType::LOCAL),
_barRendererAdaptDirty(true),
_progressBarRendererDirty(true),
_textureFile(""),
_progressBarTextureFile(""),
_slidBallNormalTextureFile(""),
_slidBallPressedTextureFile(""),
_slidBallDisabledTextureFile("")
{
    Widget::init();
    setTouchEnabled(true);
}

Slider::Slider(const std::string& barTextureFileName,
               const std::string& normalBallTextureFileName,
               TextureResType resType)
    : Slider()
{
    Widget::init();
    loadBarTexture(barTextureFileName, resType);
    loadSlidBallTextureNormal(normalBallTextureFileName, resType);
}

Slider::~Slider()
{
    _sliderEventListener = nullptr;
}

Slider* Slider::create()
{
    Slider* slider = new Slider();
    slider->autorelease();
    return slider;
}
    
Slider* Slider::create(const std::string& barTextureFileName,
                       const std::string& normalBallTextureFileName,
                       TextureResType resType)
{
    Slider* slider = new Slider(barTextureFileName, normalBallTextureFileName, resType);
    slider->autorelease();
    return slider;
}

void Slider::initRenderer()
{
    _barRenderer = Scale9Sprite::create();
    _progressBarRenderer = Scale9Sprite::create();
    _barRenderer->setScale9Enabled(false);
    _progressBarRenderer->setScale9Enabled(false);
    
    _progressBarRenderer->setAnchorPoint(Vec2(0.0f, 0.5f));
    
    addProtectedChild(_barRenderer, BASEBAR_RENDERER_Z, -1);
    addProtectedChild(_progressBarRenderer, PROGRESSBAR_RENDERER_Z, -1);
    
    _slidBallRenderer = Node::create();

    auto slidBallPressed = make_node_ptr<Sprite>();
    slidBallPressed->setVisible(false);
    _slidBallPressedRendererId = _slidBallRenderer->addChild( std::move(slidBallPressed) );

    _slidBallNormalRendererId = _slidBallRenderer->addChild( make_node_ptr<Sprite>() );

    auto slidBallDisabled = make_node_ptr<Sprite>();
    slidBallDisabled->setVisible(false);
    _slidBallDisabledRendererId = _slidBallRenderer->addChild( std::move(slidBallDisabled) );
    
    _slidBallRenderer->setCascadeColorEnabled(true);
    _slidBallRenderer->setCascadeOpacityEnabled(true);
    
    addProtectedChild(_slidBallRenderer, SLIDBALL_RENDERER_Z, -1);
}

void Slider::loadBarTexture(const std::string& fileName, TextureResType texType)
{
    _textureFile = fileName;
    _barTexType = texType;
    if (fileName.empty())
    {
        _barRenderer->resetRender();
    }
    else
    {
        switch (_barTexType)
        {
        case TextureResType::LOCAL:
            _barRenderer->initWithFile(fileName);
            break;
        case TextureResType::PLIST:
            _barRenderer->initWithSpriteFrame(
                _director->getSpriteFrameCache().getSpriteFrameByName(fileName)
            );
            break;
        default:
            break;
        }
    }
    //FIXME: https://github.com/cocos2d/cocos2d-x/issues/12249
    if (!_ignoreSize && _customSize.equals(Size::ZERO)) {
        _customSize = _barRenderer->getContentSize();
    }
    this->setupBarTexture();
}
void Slider::loadBarTexture(SpriteFrame* spriteframe)
{
    _barRenderer->initWithSpriteFrame(spriteframe);
    this->setupBarTexture();
}

void Slider::setupBarTexture()
{
    this->updateChildrenDisplayedRGBA();
    _barRendererAdaptDirty = true;
    _progressBarRendererDirty = true;
    updateContentSizeWithTextureSize(_barRenderer->getContentSize());
    _barTextureSize = _barRenderer->getContentSize();
}

void Slider::loadProgressBarTexture(const std::string& fileName, TextureResType texType)
{
    _progressBarTextureFile = fileName;
    _progressBarTexType = texType;
    if (fileName.empty())
    {
        _progressBarRenderer->resetRender();
    }
    else
    {
        switch (_progressBarTexType)
        {
        case TextureResType::LOCAL:
            _progressBarRenderer->initWithFile(fileName);
            break;
        case TextureResType::PLIST:
            _progressBarRenderer->initWithSpriteFrame(
                _director->getSpriteFrameCache().getSpriteFrameByName(fileName)
            );
            break;
        default:
            break;
        }
    }
    this->setupProgressBarTexture();
}

void Slider::loadProgressBarTexture(SpriteFrame* spriteframe)
{
    _progressBarRenderer->initWithSpriteFrame(spriteframe);
    this->setupProgressBarTexture();
}

void Slider::setupProgressBarTexture()
{
    this->updateChildrenDisplayedRGBA();
    _progressBarRenderer->setAnchorPoint(Vec2(0.0f, 0.5f));
    _progressBarTextureSize = _progressBarRenderer->getContentSize();
    _progressBarRendererDirty = true;
}

void Slider::setScale9Enabled(bool able)
{
    if (_scale9Enabled == able)
    {
        return;
    }
    
    _scale9Enabled = able;
    _barRenderer->setScale9Enabled(_scale9Enabled);
    _progressBarRenderer->setScale9Enabled(_scale9Enabled);
    
    if (_scale9Enabled)
    {
        bool ignoreBefore = _ignoreSize;
        ignoreContentAdaptWithSize(false);
        _prevIgnoreSize = ignoreBefore;
    }
    else
    {
        ignoreContentAdaptWithSize(_prevIgnoreSize);
    }
    setCapInsetsBarRenderer(_capInsetsBarRenderer);
    setCapInsetProgressBarRenderer(_capInsetsProgressBarRenderer);
    _barRendererAdaptDirty = true;
    _progressBarRendererDirty = true;
}
    
bool Slider::isScale9Enabled()const
{
    return _scale9Enabled;
}

void Slider::ignoreContentAdaptWithSize(bool ignore)
{
    if (!_scale9Enabled || (_scale9Enabled && !ignore))
    {
        Widget::ignoreContentAdaptWithSize(ignore);
        _prevIgnoreSize = ignore;
    }
}

void Slider::setCapInsets(const Rect &capInsets)
{
    setCapInsetsBarRenderer(capInsets);
    setCapInsetProgressBarRenderer(capInsets);
}

void Slider::setCapInsetsBarRenderer(const Rect &capInsets)
{
    _capInsetsBarRenderer = ui::Helper::restrictCapInsetRect(capInsets, _barRenderer->getContentSize());
    if (!_scale9Enabled)
    {
        return;
    }
    _barRenderer->setCapInsets(_capInsetsBarRenderer);
}
    
const Rect& Slider::getCapInsetsBarRenderer()const
{
    return _capInsetsBarRenderer;
}

void Slider::setCapInsetProgressBarRenderer(const Rect &capInsets)
{
    _capInsetsProgressBarRenderer = ui::Helper::restrictCapInsetRect(capInsets, _progressBarRenderer->getContentSize());

    if (!_scale9Enabled)
    {
        return;
    }
    _progressBarRenderer->setCapInsets(_capInsetsProgressBarRenderer);
}
    
const Rect& Slider::getCapInsetsProgressBarRenderer()const
{
    return _capInsetsProgressBarRenderer;
}

void Slider::loadSlidBallTextures(const std::string& normal,
                                  const std::string& pressed,
                                  const std::string& disabled,
                                  TextureResType texType)
{
    loadSlidBallTextureNormal(normal, texType);
    loadSlidBallTexturePressed(pressed,texType);
    loadSlidBallTextureDisabled(disabled,texType);
}

void Slider::loadSlidBallTextureNormal(const std::string& normal, TextureResType texType)
{
    _slidBallNormalTextureFile = normal;
    _ballNTexType = texType;

    auto slidBallNormal = getSlidBallSprite(_slidBallNormalRendererId);

    if ( ! normal.empty())
    {
        switch (_ballNTexType)
        {
        case TextureResType::LOCAL:
            slidBallNormal->setTexture(_slidBallNormalTextureFile);
            break;
        case TextureResType::PLIST:
            slidBallNormal->setSpriteFrame(
                _director->getSpriteFrameCache().getSpriteFrameByName(_slidBallNormalTextureFile)
            );
            break;
        default:
            break;
        }
    }
    this->updateChildrenDisplayedRGBA();
}
void Slider::loadSlidBallTextureNormal(SpriteFrame* spriteframe)
{
    auto slidBallNormal = getSlidBallSprite(_slidBallNormalRendererId);
    slidBallNormal->setSpriteFrame(spriteframe);
    this->updateChildrenDisplayedRGBA();
}

void Slider::loadSlidBallTexturePressed(const std::string& pressed,TextureResType texType)
{
    _slidBallPressedTextureFile = pressed;
    _isSliderBallPressedTextureLoaded = !pressed.empty();
    _ballPTexType = texType;

    auto slidBallPressed = getSlidBallSprite(_slidBallPressedRendererId);

    if (! pressed.empty())
    {
        switch (_ballPTexType)
        {
        case TextureResType::LOCAL:
            slidBallPressed->setTexture(pressed);
            break;
        case TextureResType::PLIST:
            slidBallPressed->setSpriteFrame(
                _director->getSpriteFrameCache().getSpriteFrameByName(pressed)
            );
            break;
        default:
            break;
        }
    }

    this->updateChildrenDisplayedRGBA();
}


void Slider::loadSlidBallTexturePressed(SpriteFrame* spriteframe)
{
    auto slidBallPressed = getSlidBallSprite(_slidBallPressedRendererId);
    slidBallPressed->setSpriteFrame(spriteframe);
    this->updateChildrenDisplayedRGBA();
}

void Slider::loadSlidBallTextureDisabled(const std::string& disabled,TextureResType texType)
{
    _slidBallDisabledTextureFile = disabled;
    _isSliderBallDisabledTexturedLoaded = !disabled.empty();
    _ballDTexType = texType;

    auto slidBallDisabled = getSlidBallSprite(_slidBallDisabledRendererId);

    if (! disabled.empty())
    {
        switch (_ballDTexType)
        {
        case TextureResType::LOCAL:
            slidBallDisabled->setTexture(disabled);
            break;
        case TextureResType::PLIST:
            slidBallDisabled->setSpriteFrame(
                _director->getSpriteFrameCache().getSpriteFrameByName(disabled)
            );
            break;
        default:
            break;
        }
    }
    this->updateChildrenDisplayedRGBA();
}

void Slider::loadSlidBallTextureDisabled(SpriteFrame* spriteframe)
{
    auto slidBallDisabled = getSlidBallSprite(_slidBallDisabledRendererId);
    slidBallDisabled->setSpriteFrame(spriteframe);
    this->updateChildrenDisplayedRGBA();
}

void Slider::setMaxPercent(int percent)
{
    _maxPercent = percent;
}

int Slider::getMaxPercent()const
{
    return _maxPercent;
}

void Slider::setPercent(int percent)
{
    if (percent > _maxPercent)
    {
        percent = _maxPercent;
    }
    if (percent < 0)
    {
        percent = 0;
    }
    _percent = percent;
    float res = 1.0 * percent / _maxPercent;
    float dis = _barLength * res;
    _slidBallRenderer->setPosition(dis, _contentSize.height / 2.0f);
    if (_scale9Enabled)
    {
        _progressBarRenderer->setPreferredSize(Size(dis,_contentSize.height));
    }
    else
    {
        Sprite* spriteRenderer = _progressBarRenderer->getSprite();
        
        if (nullptr != spriteRenderer) {
            Rect rect = spriteRenderer->getTextureRect();
            rect.size.width = _progressBarTextureSize.width * res;
            spriteRenderer->setTextureRect(rect, spriteRenderer->isTextureRectRotated(), rect.size);
        }
    }
}
    
bool Slider::hitTest(const cocos2d::Vec2 &pt, const Camera *camera, Vec3* /*p*/) const
{
    auto slidBallNormal = getSlidBallSprite(_slidBallNormalRendererId);

    Rect rect;
    rect.size = slidBallNormal->getContentSize();
    auto w2l = slidBallNormal->getWorldToNodeTransform();

    Rect sliderBarRect;
    sliderBarRect.size = this->_barRenderer->getContentSize();
    auto barW2l = this->_barRenderer->getWorldToNodeTransform();
    return isScreenPointInRect(pt, camera, w2l, rect, nullptr) || isScreenPointInRect(pt, camera, barW2l, sliderBarRect, nullptr);
}

bool Slider::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    bool pass = Widget::onTouchBegan(touch, unusedEvent);
    if (_hitted)
    {
        setPercent(getPercentWithBallPos(_touchBeganPosition));
        percentChangedEvent(EventType::ON_SLIDEBALL_DOWN);
    }
    return pass;
}

void Slider::onTouchMoved(Touch *touch, Event* /*unusedEvent*/)
{
    _touchMovePosition = touch->getLocation();
    setPercent(getPercentWithBallPos(_touchMovePosition));
    percentChangedEvent(EventType::ON_PERCENTAGE_CHANGED);
}

void Slider::onTouchEnded(Touch *touch, Event *unusedEvent)
{
    Widget::onTouchEnded(touch, unusedEvent);
    percentChangedEvent(EventType::ON_PERCENTAGE_CHANGED);
    percentChangedEvent(EventType::ON_SLIDEBALL_UP);
}

void Slider::onTouchCancelled(Touch *touch, Event *unusedEvent)
{
    Widget::onTouchCancelled(touch, unusedEvent);
    percentChangedEvent(EventType::ON_SLIDEBALL_CANCEL);
}

float Slider::getPercentWithBallPos(const Vec2 &pt) const
{
    Vec3 p;
    Widget::hitTest(pt, _hittedByCamera, &p);
    return ((p.x/_barLength) * static_cast<float>(_maxPercent));
}

void Slider::addEventListener(const ccSliderCallback& callback)
{
    _eventCallback = callback;
}

void Slider::percentChangedEvent(EventType event)
{
    this->retain();

    if (_eventCallback)
    {
        _eventCallback(this,event);
    }
    if (_ccEventCallback)
    {
        _ccEventCallback(this, static_cast<int>(EventType::ON_PERCENTAGE_CHANGED));
    }
    this->release();
}

int Slider::getPercent()const
{
    return _percent;
}

void Slider::onSizeChanged()
{
    Widget::onSizeChanged();
    _barRendererAdaptDirty = true;
    _progressBarRendererDirty = true;
}
    
void Slider::adaptRenderers()
{
    if (_barRendererAdaptDirty)
    {
        barRendererScaleChangedWithSize();
        _barRendererAdaptDirty = false;
    }
    if (_progressBarRendererDirty)
    {
        progressBarRendererScaleChangedWithSize();
        _progressBarRendererDirty = false;
    }
}

Size Slider::getVirtualRendererSize() const
{
    return _barRenderer->getContentSize();
}

Node* Slider::getVirtualRenderer()
{
    return _barRenderer;
}

void Slider::barRendererScaleChangedWithSize()
{
    if (_unifySize)
    {
        _barLength = _contentSize.width;
        _barRenderer->setPreferredSize(_contentSize);
    }
    else if (_ignoreSize)
    {
        
        _barRenderer->setScale(1.0f);
        _barLength = _contentSize.width;
    }
    else
    {
        _barLength = _contentSize.width;
        if (_scale9Enabled)
        {
            _barRenderer->setPreferredSize(_contentSize);
            _barRenderer->setScale(1.0f);
        }
        else
        {
            Size btextureSize = _barTextureSize;
            if (btextureSize.width <= 0.0f || btextureSize.height <= 0.0f)
            {
                _barRenderer->setScale(1.0f);
            }
            else
            {
                float bscaleX = _contentSize.width / btextureSize.width;
                float bscaleY = _contentSize.height / btextureSize.height;
                _barRenderer->setScaleX(bscaleX);
                _barRenderer->setScaleY(bscaleY);
            }
        }
    }
    _barRenderer->setPosition(_contentSize.width / 2.0f, _contentSize.height / 2.0f);
    setPercent(_percent);
}

void Slider::progressBarRendererScaleChangedWithSize()
{
    if (_unifySize)
    {
        _progressBarRenderer->setPreferredSize(_contentSize);
    }
    else if (_ignoreSize)
    {
        if (!_scale9Enabled)
        {
            Size ptextureSize = _progressBarTextureSize;
            float pscaleX = _contentSize.width / ptextureSize.width;
            float pscaleY = _contentSize.height / ptextureSize.height;
            _progressBarRenderer->setScaleX(pscaleX);
            _progressBarRenderer->setScaleY(pscaleY);
        }
    }
    else
    {
        if (_scale9Enabled)
        {
            _progressBarRenderer->setPreferredSize(_contentSize);
            _progressBarRenderer->setScale(1.0);
        }
        else
        {
            Size ptextureSize = _progressBarTextureSize;
            if (ptextureSize.width <= 0.0f || ptextureSize.height <= 0.0f)
            {
                _progressBarRenderer->setScale(1.0f);
                return;
            }
            float pscaleX = _contentSize.width / ptextureSize.width;
            float pscaleY = _contentSize.height / ptextureSize.height;
            _progressBarRenderer->setScaleX(pscaleX);
            _progressBarRenderer->setScaleY(pscaleY);
        }
    }
    _progressBarRenderer->setPosition(0.0f, _contentSize.height / 2.0f);
    setPercent(_percent);
}

void Slider::onPressStateChangedToNormal()
{
    auto slidBallNormal = getSlidBallSprite(_slidBallNormalRendererId);

    slidBallNormal->setVisible(true);
    getSlidBallSprite(_slidBallPressedRendererId )->setVisible(false);
    getSlidBallSprite(_slidBallDisabledRendererId)->setVisible(false);

    slidBallNormal->setGLProgramState(this->getNormalGLProgramState(slidBallNormal->getTexture()));
    slidBallNormal->setScale(_sliderBallNormalTextureScaleX, _sliderBallNormalTextureScaleY);
}

void Slider::onPressStateChangedToPressed()
{
    auto slidBallNormal = getSlidBallSprite(_slidBallNormalRendererId);

    slidBallNormal->setGLProgramState(this->getNormalGLProgramState(slidBallNormal->getTexture()));
    
    if (!_isSliderBallPressedTextureLoaded)
    {
        slidBallNormal->setScale(_sliderBallNormalTextureScaleX + _zoomScale,
                                 _sliderBallNormalTextureScaleY + _zoomScale);
    }
    else
    {
        slidBallNormal->setVisible(false);
        getSlidBallSprite(_slidBallPressedRendererId)->setVisible(true);
        getSlidBallSprite(_slidBallDisabledRendererId)->setVisible(false);
    }
}

void Slider::onPressStateChangedToDisabled()
{
    auto slidBallNormal = getSlidBallSprite(_slidBallNormalRendererId);

    if (!_isSliderBallDisabledTexturedLoaded)
    {
        slidBallNormal->setGLProgramState(this->getGrayGLProgramState(slidBallNormal->getTexture()));
        slidBallNormal->setVisible(true);
    }
    else
    {
        slidBallNormal->setVisible(false);
        getSlidBallSprite(_slidBallDisabledRendererId)->setVisible(true);
    }
    
    slidBallNormal->setScale(_sliderBallNormalTextureScaleX, _sliderBallNormalTextureScaleY);
    getSlidBallSprite(_slidBallPressedRendererId)->setVisible(false);
}
    
    
void Slider::setZoomScale(float scale)
{
    _zoomScale = scale;
}

float Slider::getZoomScale()const
{
    return _zoomScale;
}


std::string Slider::getDescription() const
{
    return "Slider";
}

Widget* Slider::createCloneInstance() const
{
    return Slider::create();
}

void Slider::copySpecialProperties(const Widget *widget)
{
    auto slider = dynamic_cast<const Slider*>(widget);

    if (slider)
    {
        _prevIgnoreSize = slider->_prevIgnoreSize;
        setScale9Enabled(slider->_scale9Enabled);
        auto barSprite = slider->_barRenderer->getSprite();
        if (nullptr != barSprite)
        {
            loadBarTexture(barSprite->getSpriteFrame());
        }
        auto progressSprite = slider->_progressBarRenderer->getSprite();
        if (nullptr != progressSprite)
        {
            loadProgressBarTexture(progressSprite->getSpriteFrame());
        }

        loadSlidBallTextureNormal(
            getSlidBallSprite(slider->_slidBallNormalRendererId)->getSpriteFrame()
        );
        loadSlidBallTexturePressed(
            getSlidBallSprite(slider->_slidBallPressedRendererId)->getSpriteFrame()
        );
        loadSlidBallTextureDisabled(
            getSlidBallSprite(slider->_slidBallDisabledRendererId)->getSpriteFrame()
        );
        setPercent(slider->getPercent());
        setMaxPercent(slider->getMaxPercent());
        _isSliderBallPressedTextureLoaded = slider->_isSliderBallPressedTextureLoaded;
        _isSliderBallDisabledTexturedLoaded = slider->_isSliderBallDisabledTexturedLoaded;
        _sliderEventListener = slider->_sliderEventListener;
        _eventCallback = slider->_eventCallback;
        _ccEventCallback = slider->_ccEventCallback;
    }
}

}

} // namespace cocos2d
