/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2009      Valentin Milea
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

#include "2d/CCProtectedNode.h"

#include "base/CCDirector.h"
#include "2d/CCScene.h"

#include <algorithm> // find_if, for_each

namespace cocos2d {

ProtectedNode::ProtectedNode() : _reorderProtectedChildDirty(false)
{
}

ProtectedNode::~ProtectedNode()
{
    CCLOGINFO( "deallocing ProtectedNode: %p - tag: %i", this, _tag );
    removeAllProtectedChildren();
}

ProtectedNode * ProtectedNode::create(void)
{
	ProtectedNode * ret = new (std::nothrow) ProtectedNode();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
	return ret;
}

void ProtectedNode::cleanup()
{
    Node::cleanup();
    // timers
    for (const auto & child: _protectedChildren)
        child->cleanup();
}

void ProtectedNode::addProtectedChild(cocos2d::Node *child)
{
    addProtectedChild(child, child->getLocalZOrder(), child->getTag());
}

void ProtectedNode::addProtectedChild(cocos2d::Node *child, int localZOrder)
{
    addProtectedChild(child, localZOrder, child->getTag());
}

/* "add" logic MUST only be on this method
 * If a class want's to extend the 'addChild' behavior it only needs
 * to override this method
 */
void ProtectedNode::addProtectedChild(Node *child, int zOrder, int tag)
{
    CCASSERT( child != nullptr, "Argument must be non-nil");
    CCASSERT( child->getParent() == nullptr, "child already added. It can't be added again");
    
    this->insertProtectedChild(child, zOrder);
    
    child->setTag(tag);
    
    child->setParent(this);

    if( _running )
    {
        child->onEnter();
        // prevent onEnterTransitionDidFinish to be called twice when a node is added in onEnter
        if (_isTransitionFinished) {
            child->onEnterTransitionDidFinish();
        }
    }
    
    if (_cascadeColorEnabled)
    {
        updateCascadeColor();
    }
    
    if (_cascadeOpacityEnabled)
    {
        updateCascadeOpacity();
    }
}

Node* ProtectedNode::getProtectedChildByTag(int tag)
{
    CCASSERT( tag != Node::INVALID_TAG, "Invalid tag");
    
    auto iter = std::find_if(_protectedChildren.begin(),
                             _protectedChildren.end(),
                             [tag] (const retaining_ptr<Node> & p) {
                                 return p->getTag() == tag;
                             });

    return (_protectedChildren.end() == iter ? nullptr : iter->get());
}

/* "remove" logic MUST only be on this method
 * If a class want's to extend the 'removeChild' behavior it only needs
 * to override this method
 */
void ProtectedNode::removeProtectedChild(cocos2d::Node *child, bool cleanup)
{
    auto iter = std::find_if(_protectedChildren.begin(),
                             _protectedChildren.end(),
                             [child] (const retaining_ptr<Node> & p) {
                                 return p.get() == child;
                             });

    if (_protectedChildren.end() != iter)
    {
        
        // IMPORTANT:
        //  -1st do onExit
        //  -2nd cleanup
        if (_running)
        {
            child->onExitTransitionDidStart();
            child->onExit();
        }
        
        // If you don't do cleanup, the child's actions will not get removed and the
        // its scheduledSelectors_ dict will not get released!
        if (cleanup)
        {
            child->cleanup();
        }
        
        // set parent nil at the end
        child->setParent(nullptr);
        
        _protectedChildren.erase(iter);
    }
}

void ProtectedNode::removeAllProtectedChildren()
{
    removeAllProtectedChildrenWithCleanup(true);
}

void ProtectedNode::removeAllProtectedChildrenWithCleanup(bool cleanup)
{
    // not using detachChild improves speed here

    auto       it  = _protectedChildren.rbegin();
    const auto end = _protectedChildren.rend();

    while (it != end)
    {
        auto & p = *it;

        // IMPORTANT:
        //  -1st do onExit
        //  -2nd cleanup
        if(_running)
        {
            p->onExitTransitionDidStart();
            p->onExit();
        }

        if (cleanup)
        {
            p->cleanup();
        }

        // set parent nil at the end
        p->setParent(nullptr);

        _protectedChildren.erase((++it).base());
    }
}

void ProtectedNode::removeProtectedChildByTag(int tag, bool cleanup)
{
    CCASSERT( tag != Node::INVALID_TAG, "Invalid tag");
    
    Node *child = this->getProtectedChildByTag(tag);
    
    if (child == nullptr)
    {
        CCLOG("cocos2d: removeChildByTag(tag = %d): child not found!", tag);
    }
    else
    {
        this->removeProtectedChild(child, cleanup);
    }
}

// helper used by reorderChild & add
void ProtectedNode::insertProtectedChild(cocos2d::Node *child, int z)
{
    _protectedChildren.reserve(_protectedChildren.size() + 1);
    _protectedChildren.push_back(to_retaining_ptr(child));
    child->setLocalZOrder(z);
    _reorderProtectedChildDirty = true;
}

void ProtectedNode::sortAllProtectedChildren()
{
    if( _reorderProtectedChildDirty )
    {
        sortNodes(_protectedChildren);
        _reorderProtectedChildDirty = false;
    }
}

void ProtectedNode::reorderProtectedChild(cocos2d::Node *child, int localZOrder)
{
    CCASSERT( child != nullptr, "Child must be non-nil");
    _reorderProtectedChildDirty = true;
    child->setLocalZOrder(localZOrder);
}

void ProtectedNode::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    // quick return if not visible. children won't be drawn.
    if (!_visible)
    {
        return;
    }
    
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    
    // IMPORTANT:
    // To ease the migration to v3.0, we still support the Mat4 stack,
    // but it is deprecated and your code should not rely on it
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when setting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
    
   
    sortAllChildren();
    sortAllProtectedChildren();

    auto child    = _children.cbegin();
    auto childEnd = _children.cend();

    auto protChild    = _protectedChildren.cbegin();
    auto protChildEnd = _protectedChildren.cend();

    //
    // draw children and protectedChildren zOrder < 0
    //
    for (; child != childEnd && (*child)->getLocalZOrder() < 0;
         ++child)
    {
        (*child)->visit(renderer, _modelViewTransform, flags);
    }

    for (; protChild != protChildEnd && (*protChild)->getLocalZOrder() < 0;
         ++protChild)
    {
        (*protChild)->visit(renderer, _modelViewTransform, flags);
    }
    
    //
    // draw self
    //
    if (isVisitableByVisitingCamera())
        this->draw(renderer, _modelViewTransform, flags);
    
    //
    // draw children and protectedChildren zOrder >= 0
    //
    for (; child != childEnd; ++child)
    {
        (*child)->visit(renderer, _modelViewTransform, flags);
    }

    for (; protChild != protChildEnd; ++protChild)
    {
        (*protChild)->visit(renderer, _modelViewTransform, flags);
    }

    // FIX ME: Why need to set _orderOfArrival to 0??
    // Please refer to https://github.com/cocos2d/cocos2d-x/pull/6920
    // setOrderOfArrival(0);
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void ProtectedNode::onEnter()
{
    Node::onEnter();

    for (const auto & child: _protectedChildren)
        child->onEnter();
}

void ProtectedNode::onEnterTransitionDidFinish()
{
    Node::onEnterTransitionDidFinish();

    for( const auto & child: _protectedChildren)
        child->onEnterTransitionDidFinish();
}

void ProtectedNode::onExitTransitionDidStart()
{
    Node::onExitTransitionDidStart();

    for (const auto & child: _protectedChildren)
        child->onExitTransitionDidStart();
}

void ProtectedNode::onExit()
{
    Node::onExit();

    for (const auto & child: _protectedChildren)
        child->onExit();
}

void ProtectedNode::updateDisplayedOpacity(GLubyte parentOpacity)
{
	_displayedOpacity = _realOpacity * parentOpacity/255.0;
    updateColor();
    
    if (_cascadeOpacityEnabled)
    {
        for(auto child : _children){
            child->updateDisplayedOpacity(_displayedOpacity);
        }
    }
    
    for (auto & child : _protectedChildren){
        child->updateDisplayedOpacity(_displayedOpacity);
    }
}

void ProtectedNode::updateDisplayedColor(const Color3B& parentColor)
{
	_displayedColor.r = _realColor.r * parentColor.r/255.0;
	_displayedColor.g = _realColor.g * parentColor.g/255.0;
	_displayedColor.b = _realColor.b * parentColor.b/255.0;
    updateColor();
    
    if (_cascadeColorEnabled)
    {
        for(const auto child : _children){
            child->updateDisplayedColor(_displayedColor);
        }
    }
    for (const auto & child : _protectedChildren) {
        child->updateDisplayedColor(_displayedColor);
    }
}

void ProtectedNode::disableCascadeColor()
{
    for(auto child : _children){
        child->updateDisplayedColor(Color3B::WHITE);
    }
    for (auto & child : _protectedChildren) {
        child->updateDisplayedColor(Color3B::WHITE);
    }
}

void ProtectedNode::disableCascadeOpacity()
{
    _displayedOpacity = _realOpacity;
    
    for (auto child : _children) {
        child->updateDisplayedOpacity(255);
    }
    
    for (auto & child : _protectedChildren) {
        child->updateDisplayedOpacity(255);
    }
}

void ProtectedNode::setCameraMask(unsigned short mask, bool applyChildren)
{
    Node::setCameraMask(mask, applyChildren);

    if (applyChildren)
    {
        for (auto & child : _protectedChildren)
        {
            child->setCameraMask(mask);
        }
    }
    
}

} // namespace cocos2d
