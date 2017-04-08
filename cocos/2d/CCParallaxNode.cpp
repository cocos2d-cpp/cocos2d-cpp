/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
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

#include "2d/CCParallaxNode.h"
#include "base/CCDirector.h"

namespace cocos2d {

ParallaxNode::ParallaxNode()
{
    _parallaxArray.reserve(5);        
    _lastPosition.set(-100.0f, -100.0f);
}

ParallaxNode * ParallaxNode::create()
{
    ParallaxNode *ret = new (std::nothrow) ParallaxNode();
    ret->autorelease();
    return ret;
}

NodeId ParallaxNode::addChild(Node *child, int z, const Vec2& ratio, const Vec2& offset)
{
    return addChild(to_node_ptr(child), z, ratio, offset);
}

NodeId ParallaxNode::addChild(node_ptr<Node> child, int z, const Vec2& ratio, const Vec2& offset)
{
    _parallaxArray.emplace_back(ratio, offset, to_node_ptr(child.get()));

    Vec2 pos = this->absolutePosition();
    pos.x = -pos.x + pos.x * ratio.x + offset.x;
    pos.y = -pos.y + pos.y * ratio.y + offset.y;
    child->setPosition(pos);

    return Node::addChild(std::move(child), z, child->getName());
}

NodeId ParallaxNode::addChild(node_ptr<Node>, int, int)
{
    CCASSERT(false, "ParallaxNode: use addChild:z:parallaxRatio:positionOffset instead");
    return NodeId();
}

NodeId ParallaxNode::addChild(node_ptr<Node>, int, const std::string&)
{
    CCASSERT(false, "ParallaxNode: use addChild:z:parallaxRatio:positionOffset instead");
    return NodeId();
}

node_ptr<Node> ParallaxNode::removeChild(NodeId id, bool cleanup)
{
    if (auto child = Director::getInstance()->getNodeRegister().getNode(id))
    {
        auto it = std::find_if(_parallaxArray.begin(), _parallaxArray.end(),
                               [child] (auto const& v) {
                                   return v._child.get() == child;
                               });

        if (_parallaxArray.end() != it)
        {
            _parallaxArray.erase(it);
        }

        return Node::removeChild(id, cleanup);
    }

    return node_ptr<Node>();
}

void ParallaxNode::removeAllChildrenWithCleanup(bool cleanup)
{
    _parallaxArray.clear();
    Node::removeAllChildrenWithCleanup(cleanup);
}

Vec2 ParallaxNode::absolutePosition()
{
    Vec2 ret = _position;
    Node *cn = this;
    while (cn->getParent() != nullptr)
    {
        cn = cn->getParent();
        ret = ret + cn->getPosition();
    }
    return ret;
}

/*
The positions are updated at visit because:
- using a timer is not guaranteed that it will called after all the positions were updated
- overriding "draw" will only precise if the children have a z > 0
*/
void ParallaxNode::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    //    Vec2 pos = position_;
    //    Vec2    pos = [self convertToWorldSpace:Vec2::ZERO];
    Vec2 pos = this->absolutePosition();
    if( ! pos.equals(_lastPosition) )
    {
        for (size_t i = 0, size = _parallaxArray.size(); i < size; i++) 
        {
            PointObject & point = _parallaxArray[i];
            float x = -pos.x + pos.x * point._ratio.x + point._offset.x;
            float y = -pos.y + pos.y * point._ratio.y + point._offset.y;            
            point._child->setPosition(x, y);
        }
        _lastPosition = pos;
    }
    Node::visit(renderer, parentTransform, parentFlags);
}

} // namespace cocos2d
