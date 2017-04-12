/*
 * Copyright (c) 2012 cocos2d-x.org
 * Copyright 2012 Yannick Loriot. All rights reserved.
 * Copyright (c) 2017 Iakov Sergeev <yahont@github>
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
 */

#include "CCControlPotentiometer.h"
#include "base/CCDirector.h"

namespace cocos2d {
namespace extension {

ControlPotentiometer::ControlPotentiometer(const char* backgroundFile, const char* progressFile, const char* thumbFile)
: _value(0.0f)
, _minimumValue(0.0f)
, _maximumValue(0.0f)
, _thumbSpriteId()
, _progressTimerId()
{
    Control::init();

    auto trackSprite = make_node_ptr<Sprite>(backgroundFile);
    auto progressTimer = make_node_ptr<ProgressTimer>(make_node_ptr<Sprite>(progressFile));
    auto thumbSprite = make_node_ptr<Sprite>(thumbFile);

    thumbSprite->setPosition(progressTimer->getPosition());
    setContentSize(trackSprite->getContentSize());

    _thumbSpriteId   = addChild(std::move(thumbSprite), 2);
    _progressTimerId = addChild(std::move(progressTimer), 1);
    addChild( std::move(trackSprite) );

    // Init default values
    _minimumValue           = 0.0f;
    _maximumValue           = 1.0f;
    setValue(_minimumValue);
}

void ControlPotentiometer::setEnabled(bool enabled)
{
    Control::setEnabled(enabled);

    auto & nodeRegister = Director::getInstance()->getNodeRegister();

    if (auto thumbSprite = nodeRegister.get<Sprite>(_thumbSpriteId))
    {
        thumbSprite->setOpacity(enabled ? 255 : 128);
    }
}

void ControlPotentiometer::setValue(float value)
{
    // set new value with sentinel
    if (value < _minimumValue)
    {
        value                   = _minimumValue;
    }
	
    if (value > _maximumValue) 
    {
        value                   = _maximumValue;
    }
    
    _value                      = value;
    
    // Update thumb and progress position for new value
    float percent               = (value - _minimumValue) / (_maximumValue - _minimumValue);
 
    auto & nodeRegister = Director::getInstance()->getNodeRegister();

    if (auto progressTimer = nodeRegister.get<ProgressTimer>(_progressTimerId))
    {
        progressTimer->setPercentage(percent * 100.0f);
    }
    if (auto thumbSprite = nodeRegister.get<Sprite>(_thumbSpriteId))
    {
        thumbSprite->setRotation(percent * 360.0f);
    }
    
    sendActionsForControlEvents(Control::EventType::VALUE_CHANGED);    
}

float ControlPotentiometer::getValue()
{
    return _value;
}

void ControlPotentiometer::setMinimumValue(float minimumValue)
{
    _minimumValue       = minimumValue;
    
    if (_minimumValue >= _maximumValue)
    {
        _maximumValue   = _minimumValue + 1.0f;
    }
    
    setValue(_maximumValue);
}

float ControlPotentiometer::getMinimumValue()
{
    return _minimumValue;
}

void ControlPotentiometer::setMaximumValue(float maximumValue)
{
    _maximumValue       = maximumValue;
    
    if (_maximumValue <= _minimumValue)
    {
        _minimumValue   = _maximumValue - 1.0f;
    }
    
    setValue(_minimumValue);
}

float ControlPotentiometer::getMaximumValue()
{
    return _maximumValue;
}

bool ControlPotentiometer::isTouchInside(Touch * touch)
{
    Vec2 touchLocation = this->getTouchLocation(touch);
    
    auto & nodeRegister = Director::getInstance()->getNodeRegister();

    if (auto progressTimer = nodeRegister.get<ProgressTimer>(_progressTimerId))
    {
        float distance = distanceBetweenPointAndPoint(progressTimer->getPosition(), touchLocation);
        return distance < std::min(getContentSize().width / 2, getContentSize().height / 2);
    }

    return false;
}

bool ControlPotentiometer::onTouchBegan(Touch *pTouch, Event* /*pEvent*/)
{
    if (!this->isTouchInside(pTouch) || !this->isEnabled() || !isVisible())
    {
        return false;
    }
    
    _previousLocation    = this->getTouchLocation(pTouch);
    
    this->potentiometerBegan(_previousLocation);
    
    return true;
}

void ControlPotentiometer::onTouchMoved(Touch *pTouch, Event* /*pEvent*/)
{
    Vec2 location    = this->getTouchLocation(pTouch);

    this->potentiometerMoved(location);
}

void ControlPotentiometer::onTouchEnded(Touch* /*pTouch*/, Event* /*pEvent*/)
{
    this->potentiometerEnded(Vec2::ZERO);
}

float ControlPotentiometer::distanceBetweenPointAndPoint(Vec2 point1, Vec2 point2)
{
    float dx = point1.x - point2.x;
    float dy = point1.y - point2.y;
    return sqrt(dx*dx + dy*dy);
}

float ControlPotentiometer::angleInDegreesBetweenLineFromPoint_toPoint_toLineFromPoint_toPoint(
    Vec2 beginLineA, 
    Vec2 endLineA,
    Vec2 beginLineB,
    Vec2 endLineB)
{
    float a = endLineA.x - beginLineA.x;
    float b = endLineA.y - beginLineA.y;
    float c = endLineB.x - beginLineB.x;
    float d = endLineB.y - beginLineB.y;
    
    float atanA = atan2(a, b);
    float atanB = atan2(c, d);
    
    // convert radiants to degrees
    return (atanA - atanB) * 180 / M_PI;
}

void ControlPotentiometer::potentiometerBegan(Vec2 /*location*/)
{
    setSelected(true);

    auto & nodeRegister = Director::getInstance()->getNodeRegister();

    if (auto thumbSprite = nodeRegister.get<Sprite>(_thumbSpriteId))
    {
        thumbSprite->setColor(Color3B::GRAY);
    }
}

void ControlPotentiometer::potentiometerMoved(Vec2 location)
{
    auto progressTimer = Director::getInstance()->getNodeRegister().get<ProgressTimer>(_progressTimerId);

    if (progressTimer == nullptr)
        return;

    float angle = angleInDegreesBetweenLineFromPoint_toPoint_toLineFromPoint_toPoint(
        progressTimer->getPosition(),
        location,
        progressTimer->getPosition(),
        _previousLocation
    );
    
    // fix value, if the 12 o'clock position is between location and previousLocation
    if (angle > 180)
    {
        angle -= 360;
    }
    else if (angle < -180)
    {
        angle += 360;
    }

    setValue(_value + angle / 360.0f * (_maximumValue - _minimumValue));
    
    _previousLocation    = location;
}

void ControlPotentiometer::potentiometerEnded(Vec2 /*location*/)
{
    auto & nodeRegister = Director::getInstance()->getNodeRegister();

    if (auto thumbSprite = nodeRegister.get<Sprite>(_thumbSpriteId))
    {
        thumbSprite->setColor(Color3B::WHITE);
    }

    setSelected(false);
}

} // namespace extension
} // namespace cocos2d
