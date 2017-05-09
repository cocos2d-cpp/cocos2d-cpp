/*
 * Copyright (c) 2012 cocos2d-x.org
 * http://www.cocos2d-x.org
 *
 * Copyright 2011 Yannick Loriot. All rights reserved.
 * http://yannickloriot.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Converted to c++ / cocos2d-x by Angus C
 */


#ifndef __CCCONTROL_SLIDER_H__
#define __CCCONTROL_SLIDER_H__

#include "CCControl.h"
#include "CCInvocation.h"
#include "extensions/ExtensionExport.h"

namespace cocos2d {
namespace extension {

class CC_EX_DLL ControlSlider: public Control
{
public:
    /**
     * Creates slider with a background filename, a progress filename and a
     * thumb image filename.
     */
    static ControlSlider* create(const char* bgFile, const char* progressFile, const char* thumbFile);

    /**
     * Creates a slider with a given background sprite and a progress bar and a
     * thumb item.
     *
     * @see initWithSprites
     */
    static ControlSlider* create(Sprite * backgroundSprite, Sprite* pogressSprite, Sprite* thumbSprite);

    /**
     * Creates slider with a background filename, a progress filename, a thumb
     * and a selected thumb image filename.
     */
    static ControlSlider* create(const char* bgFile, const char* progressFile, const char* thumbFile,
            const char* selectedThumbSpriteFile);

    /**
     * Creates a slider with a given background sprite and a progress bar, a thumb
     * and a selected thumb .
     *
     * @see initWithSprites
     */
    static ControlSlider* create(Sprite * backgroundSprite, Sprite* pogressSprite, Sprite* thumbSprite,
            Sprite* selectedThumbSprite);
    /**
     * @js ctor
     * @lua new
     */
    ControlSlider();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~ControlSlider();

    /**
    * Initializes a slider with a background sprite, a progress bar and a thumb
    * item.
    *
    * @param backgroundSprite          Sprite, that is used as a background.
    * @param progressSprite            Sprite, that is used as a progress bar.
    * @param thumbSprite               Sprite, that is used as a thumb.
    */
    virtual bool initWithSprites(Sprite * backgroundSprite, Sprite* progressSprite, Sprite* thumbSprite);

    /**
    * Initializes a slider with a background sprite, a progress bar and a thumb
    * item.
    *
    * @param backgroundSprite          Sprite, that is used as a background.
    * @param progressSprite            Sprite, that is used as a progress bar.
    * @param thumbSprite               Sprite, that is used as a thumb.
    * @param selectedThumbSprite       Sprite, that is used as a selected thumb.
    */
    virtual bool initWithSprites(Sprite * backgroundSprite, Sprite* progressSprite, Sprite* thumbSprite,
            Sprite* selectedThumbSprite);

    virtual void needsLayout() override;

    virtual void setMaximumValue(float val);
    virtual void setEnabled(bool enabled) override;
    virtual bool isTouchInside(Touch * touch) override;
    Vec2 locationFromTouch(Touch* touch);
    virtual void setValue(float val);
    virtual void setMinimumValue(float val);

protected:
    void sliderBegan(Vec2 location);
    void sliderMoved(Vec2 location);
    void sliderEnded(Vec2 location);

    virtual bool onTouchBegan(Touch* touch, Event* pEvent) override;
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;

    /** Returns the value for the given location. */
    float valueForLocation(Vec2 location);

public:
    virtual float getValue(void) const { return _value; }
    virtual float getMinimumValue(void) const { return _minimumValue; }
    virtual float getMaximumValue(void) const { return _minimumValue; }

    virtual float getMinimumAllowedValue() const { return _minimumAllowedValue; }
    virtual void  setMinimumAllowedValue(float var){ _minimumAllowedValue = var; }
    
    virtual float getMaximumAllowedValue() const { return _maximumAllowedValue; }
    virtual void  setMaximumAllowedValue(float var){ _maximumAllowedValue = var; }

    virtual Sprite* getThumbSprite() const { return _thumbSprite; }
    virtual void setThumbSprite(Sprite* var)
    {
        if (_thumbSprite != var)
        {
            CC_SAFE_RETAIN(var);
            CC_SAFE_RELEASE(_thumbSprite);
            _thumbSprite = var;
        }
    }

    virtual Sprite* getSelectedThumbSprite() const { return _selectedThumbSprite; }
    virtual void setSelectedThumbSprite(Sprite* var)
    {
        if (_selectedThumbSprite != var)
        {
            CC_SAFE_RETAIN(var);
            CC_SAFE_RELEASE(_selectedThumbSprite);
            _selectedThumbSprite = var;
        }
    }

    virtual Sprite* getProgressSprite() const { return _progressSprite; }
    virtual void setProgressSprite(Sprite* var)
    {
        if (_progressSprite != var)
        {
            CC_SAFE_RETAIN(var);
            CC_SAFE_RELEASE(_progressSprite);
            _progressSprite = var;
        }
    }

    virtual Sprite* getBackgroundSprite() const { return _backgroundSprite; }
    virtual void setBackgroundSprite(Sprite* var)
    {
        if (_backgroundSprite != var)
        {
            CC_SAFE_RETAIN(var);
            CC_SAFE_RELEASE(_backgroundSprite);
            _backgroundSprite = var;
        }
    }

private:

    /** Contains the receiver's current value. */
    float _value;
    /** Contains the minimum value of the receiver.
     * The default value of this property is 0.0. */
    float _minimumValue;
    /** Contains the maximum value of the receiver.
     * The default value of this property is 1.0. */
    float _maximumValue;

    float _minimumAllowedValue;
    float _maximumAllowedValue;

    Sprite* _thumbSprite;
    Sprite* _selectedThumbSprite;
    Sprite* _progressSprite;
    Sprite* _backgroundSprite;
};

} // namespace extension
} // namespace cocos2d

#endif
