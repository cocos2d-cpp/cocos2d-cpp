/*
 * Copyright (c) 2012 cocos2d-x.org
 * http://www.cocos2d-x.org
 *
 * Copyright 2011 Yannick Loriot.
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
 *
 * converted to c++ / cocos2d-x by Angus C
 */

#include "CCControl.h"
#include "base/CCDirector.h"
#include "2d/CCMenu.h"
#include "base/CCTouch.h"
#include "CCInvocation.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerTouch.h"

namespace cocos2d {
namespace extension {

Control::Control()
: _enabled(false)
, _selected(false)
, _highlighted(false)
, _hasVisibleParents(false)
, _isOpacityModifyRGB(false)
, _state(State::NORMAL)
{
}

Control::~Control()
{
}

Control* Control::create()
{
    Control* pRet = new (std::nothrow) Control();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
}

bool Control::init()
{
    if (Layer::init())
    {
        // Initialise instance variables
        _state=Control::State::NORMAL;
        setEnabled(true);
        setSelected(false);
        setHighlighted(false);

        auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);
        touchListener->onTouchBegan = CC_CALLBACK_2(Control::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(Control::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(Control::onTouchEnded, this);
        touchListener->onTouchCancelled = CC_CALLBACK_2(Control::onTouchCancelled, this);
        
        dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
        return true;
    }
    else
    {
        return false;
    }
}

void Control::sendActionsForControlEvents(EventType controlEvents)
{
	retain();
    // For each control events
    for (int i = 0; i < kControlEventTotalNumber; i++)
    {
        // If the given controlEvents bitmask contains the current event
        if (((int)controlEvents & (1 << i)))
        {
            // Call invocations
            const auto & invocationList = this->dispatchListforControlEvent((Control::EventType)(1<<i));

            for(const auto &invocation : invocationList) {
                invocation->invoke(this);
            }
        }
    }
	release();
}
void Control::addTargetWithActionForControlEvents(Ref* target, Handler action, EventType controlEvents)
{
    // For each control events
    for (int i = 0; i < kControlEventTotalNumber; i++)
    {
        // If the given controlEvents bitmask contains the current event
        if (((int)controlEvents & (1 << i)))
        {
            this->addTargetWithActionForControlEvent(target, action, (EventType)(1<<i));
        }
    }
}



/**
 * Adds a target and action for a particular event to an internal dispatch 
 * table.
 * The action message may optionally include the sender and the event as 
 * parameters, in that order.
 * When you call this method, target is not retained.
 *
 * @param target The target object that is, the object to which the action 
 * message is sent. It cannot be nil. The target is not retained.
 * @param action A selector identifying an action message. It cannot be nullptr.
 * @param controlEvent A control event for which the action message is sent.
 * See "CCControlEvent" for constants.
 */
void Control::addTargetWithActionForControlEvent(Ref* target, Handler action, EventType controlEvent)
{    
    // Create the invocation object
    auto invocation = to_retaining_ptr( Invocation::create(target, action, controlEvent));

    // Add the invocation into the dispatch list for the given control event
    auto & eventInvocationList = this->dispatchListforControlEvent(controlEvent);
    eventInvocationList.push_back( std::move( invocation));
}

void Control::removeTargetWithActionForControlEvents(Ref* target, Handler action, EventType controlEvents)
{
     // For each control events
    for (int i = 0; i < kControlEventTotalNumber; i++)
    {
        // If the given controlEvents bitmask contains the current event
        if (((int)controlEvents & (1 << i)))
        {
            this->removeTargetWithActionForControlEvent(target, action, (EventType)(1 << i));
        }
    }
}

void Control::removeTargetWithActionForControlEvent(Ref* target, Handler action, EventType controlEvent)
{
    // Retrieve all invocations for the given control event
    auto& eventInvocationList = this->dispatchListforControlEvent(controlEvent);
    
    //remove all invocations if the target and action are null
    //TODO: should the invocations be deleted, or just removed from the array? Won't that cause issues if you add a single invocation for multiple events?

    if (!target && !action)
    {
        //remove objects
        eventInvocationList.clear();
    } 
    else
    {
        eventInvocationList.erase(
            std::remove_if(
                eventInvocationList.begin(), eventInvocationList.end(),
                [&target, &action](const retaining_ptr<Invocation> & p) {
                    bool shouldBeRemoved = true;
                    if (target)
                    {
                        shouldBeRemoved = (target == p->getTarget());
                    }
                    if (action)
                    {
                        shouldBeRemoved &= (action == p->getAction());
                    }
                    return shouldBeRemoved;
                }
            ),
            eventInvocationList.end()
        );
    }
}


//CRGBA protocol
void Control::setOpacityModifyRGB(bool bOpacityModifyRGB)
{
    _isOpacityModifyRGB=bOpacityModifyRGB;
    
    for(auto & child : getChildren())
    {
        child->setOpacityModifyRGB(bOpacityModifyRGB);
    }
}

bool Control::isOpacityModifyRGB() const
{
    return _isOpacityModifyRGB;
}


Vec2 Control::getTouchLocation(Touch* touch)
{
    Vec2 touchLocation = touch->getLocation();            // Get the touch position
    touchLocation = this->convertToNodeSpace(touchLocation);  // Convert to the node space of this class
    
    return touchLocation;
}

bool Control::onTouchBegan(Touch* /*touch*/, Event* /*event*/) {
    return false;
}

void Control::onTouchMoved(Touch* /*touch*/, Event* /*event*/)
{}

void Control::onTouchEnded(Touch* /*touch*/, Event* /*event*/)
{}

void Control::onTouchCancelled(Touch* /*touch*/, Event* /*event*/)
{}


bool Control::isTouchInside(Touch* touch)
{
    Vec2 touchLocation = touch->getLocation(); // Get the touch position
    touchLocation = this->getParent()->convertToNodeSpace(touchLocation);
    Rect bBox = getBoundingBox();
    return bBox.containsPoint(touchLocation);
}

std::vector<retaining_ptr<Invocation>> & Control::dispatchListforControlEvent(EventType controlEvent)
{
    return _dispatchTable[(int)controlEvent];
}

void Control::needsLayout()
{
}

void Control::setEnabled(bool bEnabled)
{
    _enabled = bEnabled;
    if(_enabled) {
        _state = Control::State::NORMAL;
    } else {
        _state = Control::State::DISABLED;
    }

    this->needsLayout();
}

bool Control::isEnabled() const
{
    return _enabled;
}

void Control::setSelected(bool bSelected)
{
    _selected = bSelected;
    this->needsLayout();
}

bool Control::isSelected() const
{
    return _selected;
}

void Control::setHighlighted(bool bHighlighted)
{
    _highlighted = bHighlighted;
    this->needsLayout();
}

bool Control::isHighlighted() const
{
    return _highlighted;
}

bool Control::hasVisibleParents() const
{
    auto parent = this->getParent();
    for( auto c = parent; c != nullptr; c = c->getParent() )
    {
        if( !c->isVisible() )
        {
            return false;
        }
    }
    return true;
}

Control::EventType operator|(Control::EventType a, Control::EventType b) {
    return static_cast<Control::EventType>(static_cast<int>(a) | static_cast<int>(b));
}

} // namespace extension
} // namespace cocos2d
