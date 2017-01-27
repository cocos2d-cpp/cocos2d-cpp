/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2016 Chukong Technologies Inc.

http://www.cocos2d-x.org

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

#ifndef __CCLAYER_H__
#define __CCLAYER_H__

#include "2d/CCNode.h"
#include "base/CCEventKeyboard.h"
#include "base/CCEventTouch.h"
#include "base/CCProtocols.h"
#include "base/CCTouch.h"
#include "renderer/CCCustomCommand.h"


namespace cocos2d {

/**
 * @addtogroup _2d
 * @{
 */

class TouchScriptHandlerEntry;

class Event;
class EventListenerKeyboard;
class EventListenerAcceleration;

class Touch;

//
// Layer
//
/** @class Layer
 * @brief Layer is a subclass of Node that implements the TouchEventsDelegate protocol.

All features from Node are valid, plus the following new features:
- It can receive iPhone Touches
- It can receive Accelerometer input
*/
class CC_DLL Layer : public Node
{
public:    
    /** Creates a fullscreen black layer.
     *
     * @return An autoreleased Layer object.
     */
    static Layer *create();

    /** Callback function for touch began.
     *
     * @param touch Touch information.
     * @param unused_event Event information.
     * @return if return false, onTouchMoved, onTouchEnded, onTouchCancelled will never called.
     * @js NA
     */
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    /** Callback function for touch moved.
    *
    * @param touch Touch information.
    * @param unused_event Event information.
    * @js NA
    */
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    /** Callback function for touch ended.
    *
    * @param touch Touch information.
    * @param unused_event Event information.
    * @js NA
    */
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    /** Callback function for touch cancelled.
    *
    * @param touch Touch information.
    * @param unused_event Event information.
    * @js NA
    */
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);

    /** Callback function for multiple touches began.
    *
    * @param touches Touches information.
    * @param unused_event Event information.
    * @js NA
    */
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    /** Callback function for multiple touches moved.
    *
    * @param touches Touches information.
    * @param unused_event Event information.
    * @js NA
    */
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    /** Callback function for multiple touches ended.
    *
    * @param touches Touches information.
    * @param unused_event Event information.
    * @js NA
    */
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
    /** Callback function for multiple touches cancelled.
    *
    * @param touches Touches information.
    * @param unused_event Event information.
    * @js NA
    */
    virtual void onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event);

    /** Callback function for acceleration.
     * @param acc Acceleration information.
     * @param unused_event Event information.
     * @js NA
     */
    virtual void onAcceleration(Acceleration* acc, Event* unused_event);
    
    /** Callback function for key pressed.
     * @param keyCode KeyCode information.
     * @param event Event information.
     * @js NA
     */
    virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

    /** Callback function for key released.
    * @param keyCode KeyCode information.
    * @param event Event information.
    * @js NA
    */
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    // Overrides
    virtual std::string getDescription() const override;

protected:
    Layer();
    virtual ~Layer();

    virtual bool init() override;

protected:
    bool _touchEnabled;
    bool _accelerometerEnabled;
    bool _keyboardEnabled;
    EventListener* _touchListener;
    EventListenerKeyboard* _keyboardListener;
    EventListenerAcceleration* _accelerationListener;

    Touch::DispatchMode _touchMode;
    bool _swallowsTouches;

private:
    Layer(const Layer &) = delete;
    const Layer & operator=(const Layer &) = delete;

};


/** @class __LayerRGBA
 * @brief LayerRGBA is a subclass of Layer that implements the RGBAProtocol protocol using a solid color as the background.
 
 All features from Layer are valid, plus the following new features that propagate into children that conform to the RGBAProtocol:
 - opacity
 - RGB colors
 @since 2.1
 @js NA
 */
class CC_DLL __LayerRGBA : public Layer, public __RGBAProtocol
{
public:
    CREATE_FUNC(__LayerRGBA);
    
    
    //
    // Overrides
    //
    virtual GLubyte getOpacity() const override { return Layer::getOpacity(); }
    virtual GLubyte getDisplayedOpacity() const override { return Layer::getDisplayedOpacity(); }
    virtual void setOpacity(GLubyte opacity) override { Layer::setOpacity(opacity); }
    virtual void updateDisplayedOpacity(GLubyte parentOpacity) override { Layer::updateDisplayedOpacity(parentOpacity); }
    virtual bool isCascadeOpacityEnabled() const override { return Layer::isCascadeOpacityEnabled(); }
    virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled) override { Layer::setCascadeOpacityEnabled(cascadeOpacityEnabled); }

    virtual const Color3B& getColor() const override { return Layer::getColor(); }
    virtual const Color3B& getDisplayedColor() const override { return Layer::getDisplayedColor(); }
    virtual void setColor(const Color3B& color) override { Layer::setColor(color); }
    virtual void updateDisplayedColor(const Color3B& parentColor) override { Layer::updateDisplayedColor(parentColor); }
    virtual bool isCascadeColorEnabled() const override { return Layer::isCascadeOpacityEnabled(); }
    virtual void setCascadeColorEnabled(bool cascadeColorEnabled) override { Layer::setCascadeColorEnabled(cascadeColorEnabled); }

    virtual void setOpacityModifyRGB(bool bValue) override { Layer::setOpacityModifyRGB(bValue); }
    virtual bool isOpacityModifyRGB() const override { return Layer::isOpacityModifyRGB(); }

protected:
    __LayerRGBA();
    virtual ~__LayerRGBA() {}

private:
    __LayerRGBA(const __LayerRGBA &) = delete;
    const __LayerRGBA & operator=(const __LayerRGBA &) = delete;
};

//
// LayerColor
//
/** @class LayerColor
 * @brief LayerColor is a subclass of Layer that implements the RGBAProtocol protocol.

All features from Layer are valid, plus the following new features:
- opacity
- RGB colors
*/
class CC_DLL LayerColor : public Layer
{
public:
    /** Creates a fullscreen black layer.
     *
     * @return An autoreleased LayerColor object.
     */
    static LayerColor* create();
    /** Creates a Layer with color, width and height in Points.
     *
     * @param color The color of layer.
     * @param width The width of layer.
     * @param height The height of layer.
     * @return An autoreleased LayerColor object.
     */
    static LayerColor * create(const Color4B& color, GLfloat width, GLfloat height);
    /** Creates a Layer with color. Width and height are the window size.
     *
     * @param color The color of layer.
     * @return An autoreleased LayerColor object.
     */
    static LayerColor * create(const Color4B& color);

    /** Change width in Points.
     * 
     * @param w The width of layer.
     */
    void changeWidth(GLfloat w);
    /** Change height in Points.
     *
     * @param h The height of layer.
     */
    void changeHeight(GLfloat h);
    /** Change width and height in Points.
     * 
     * @param w The width of layer.
     * @param h The Height of layer.
    @since v0.8
    */
    void changeWidthAndHeight(GLfloat w ,GLfloat h);

    //
    // Overrides
    //
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

    virtual void setContentSize(const Size & var) override;

    const BlendFunc& getBlendFunc() const;
    void setBlendFunc(const BlendFunc& blendFunc);

    virtual std::string getDescription() const override;
    
protected:
    LayerColor();
    virtual ~LayerColor();
    
    bool init() override;
    bool initWithColor(const Color4B& color, GLfloat width, GLfloat height);
    bool initWithColor(const Color4B& color);

protected:
    void onDraw(const Mat4& transform, uint32_t flags);

    virtual void updateColor() override;

    BlendFunc _blendFunc;
    Vec2 _squareVertices[4];
    Color4F  _squareColors[4];
    CustomCommand _customCommand;
    Vec3 _noMVPVertices[4];
private:
    LayerColor(const LayerColor &) = delete;
    const LayerColor & operator=(const LayerColor &) = delete;

};

//
// LayerGradient
//
/** @class LayerGradient
 * @brief LayerGradient is a subclass of LayerColor that draws gradients across the background.

All features from LayerColor are valid, plus the following new features:
- direction
- final color
- interpolation mode

Color is interpolated between the startColor and endColor along the given
vector (starting at the origin, ending at the terminus).  If no vector is
supplied, it defaults to (0, -1) -- a fade from top to bottom.

If 'compressedInterpolation' is disabled, you will not see either the start or end color for
non-cardinal vectors; a smooth gradient implying both end points will be still
be drawn, however.

If ' compressedInterpolation' is enabled (default mode) you will see both the start and end colors of the gradient.

@since v0.99.5
*/
class CC_DLL LayerGradient : public LayerColor
{
public:
    /** Creates a fullscreen black layer.
     *
     * @return An autoreleased LayerGradient object.
     */
    static LayerGradient* create();

    /** Creates a full-screen Layer with a gradient between start and end.
     *
     * @param start The start color.
     * @param end The end color.
     * @return An autoreleased LayerGradient object.
     */
    static LayerGradient* create(const Color4B& start, const Color4B& end);

    /** Creates a full-screen Layer with a gradient between start and end in the direction of v.
     *
     * @param start The start color.
     * @param end The end color.
     * @param v The direction of gradient color.
     * @return An autoreleased LayerGradient object.
     */
    static LayerGradient* create(const Color4B& start, const Color4B& end, const Vec2& v);
    
    /** Whether or not the interpolation will be compressed in order to display all the colors of the gradient both in canonical and non canonical vectors.
     Default: true.
     *
     * @param compressedInterpolation The interpolation will be compressed if true.
     */
    void setCompressedInterpolation(bool compressedInterpolation);
    /** Get the compressedInterpolation
     *
     * @return The interpolation will be compressed if true.
     */
    bool isCompressedInterpolation() const;

    /** Sets the start color of the gradient.
     * 
     * @param startColor The start color.
     */
    void setStartColor( const Color3B& startColor );
    /** Returns the start color of the gradient.
     *
     * @return The start color.
     */
    const Color3B& getStartColor() const;

    /** Sets the end color of the gradient.
     *
     * @param endColor The end color.
     */
    void setEndColor( const Color3B& endColor );
    /** Returns the end color of the gradient.
     *
     * @return The end color.
     */
    const Color3B& getEndColor() const;

    /** Returns the start opacity of the gradient.
     *
     * @param startOpacity The start opacity, from 0 to 255.
     */
    void setStartOpacity( GLubyte startOpacity );
    /** Returns the start opacity of the gradient.
     *
     * @return The start opacity.
     */
    GLubyte getStartOpacity() const;

    /** Returns the end opacity of the gradient.
     *
     * @param endOpacity The end opacity, from 0 to 255.
     */
    void setEndOpacity( GLubyte endOpacity );
    /** Returns the end opacity of the gradient.
     *
     * @return The end opacity.
     */
    GLubyte getEndOpacity() const;

    /** Sets the directional vector that will be used for the gradient.
    The default value is vertical direction (0,-1). 
     *
     * @param alongVector The direction of gradient.
     */
    void setVector(const Vec2& alongVector);
    /** Returns the directional vector used for the gradient.
     *
     * @return The direction of gradient.
     */
    const Vec2& getVector() const;

    virtual std::string getDescription() const override;
    
protected:
    LayerGradient();
    virtual ~LayerGradient();
    
    virtual bool init() override;
    /** Initializes the Layer with a gradient between start and end.
     * @js init
     * @lua init
     */
    bool initWithColor(const Color4B& start, const Color4B& end);
    
    /** Initializes the Layer with a gradient between start and end in the direction of v.
     * @js init
     * @lua init
     */
    bool initWithColor(const Color4B& start, const Color4B& end, const Vec2& v);

protected:
    virtual void updateColor() override;

    Color3B _startColor;
    Color3B _endColor;
    GLubyte _startOpacity;
    GLubyte _endOpacity;
    Vec2   _alongVector;
    bool    _compressedInterpolation;
};


/** @class LayerMultiplex
 * @brief MultipleLayer is a Layer with the ability to multiplex it's children.
Features:
- It supports one or more children
- Only one children will be active a time
*/
class CC_DLL LayerMultiplex : public Layer
{
public:
    /** Creates a LayerMultiplex with an array of layers.
     @since v2.1
     * @js NA
     *
     * @param arrayOfLayers An array of layers.
     * @return An autoreleased LayerMultiplex object.
     */
    static LayerMultiplex* createWithArray(std::vector<node_ptr<Layer>> &&);

    /** Add a certain layer to LayerMultiplex.
     *
     * @param layer A layer need to be added to the LayerMultiplex.
     */
    void addLayer(Layer* layer);

    /** Switches to a certain layer indexed by n.
     The current (old) layer will be removed from it's parent with 'cleanup=true'.
     *
     * @param n The layer indexed by n will display.
     */
    void switchTo(size_t n);

    /** release the current layer and switches to another layer indexed by n.
    The current (old) layer will be removed from it's parent with 'cleanup=true'.
     *
     * @param n The layer indexed by n will display.
     */
    void switchToAndReleaseMe(size_t n);

    virtual std::string getDescription() const override;
    
protected:
    /**
     * @js ctor
     */
    LayerMultiplex();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~LayerMultiplex();
    
    virtual bool init() override;
    
    /** initializes a MultiplexLayer with an array of layers
     @since v2.1
     */
    bool initWithArray(std::vector<node_ptr<Layer>> &&);

protected:
    unsigned int                 _enabledLayer;
    std::vector<node_ptr<Layer>> _layers;

private:
    LayerMultiplex(const LayerMultiplex &) = delete;
    const LayerMultiplex & operator=(const LayerMultiplex &) = delete;
};


// end of _2d group
/// @}

} // namespace cocos2d

#endif // __CCLAYER_H__

