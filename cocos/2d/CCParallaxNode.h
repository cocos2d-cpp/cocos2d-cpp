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
#ifndef __CCPARALLAX_NODE_H__
#define __CCPARALLAX_NODE_H__

#include "2d/CCNode.h"

#include <vector>

namespace cocos2d {

/**
 * @addtogroup _2d
 * @{
 */

/** @class ParallaxNode
 * @brief ParallaxNode: A node that simulates a parallax scroller

The children will be moved faster / slower than the parent according the parallax ratio.

*/
class CC_DLL ParallaxNode : public Node
{
public:
    /** Create a Parallax node. 
     *
     * @return An autoreleased ParallaxNode object.
     */
    static ParallaxNode * create();

    // prevents compiler warning: "Included function hides overloaded virtual functions"
    using Node::addChild;

    /** Adds a child to the container with a local z-order, parallax ratio and position offset.
     *
     * @param child A child node.
     * @param z Z order for drawing priority.
     * @param parallaxRatio A given parallax ratio.
     * @param positionOffset A given position offset.
     */
    CC_DEPRECATED_ATTRIBUTE NodeId addChild(Node * child, int z, const Vec2& parallaxRatio, const Vec2& positionOffset);
    NodeId addChild(node_ptr<Node> child, int z, const Vec2& parallaxRatio, const Vec2& positionOffset);

    virtual NodeId addChild(node_ptr<Node> child, int zOrder, int tag) override;
    virtual NodeId addChild(node_ptr<Node> child, int zOrder, const std::string &name) override;

    CC_DEPRECATED_ATTRIBUTE node_ptr<Node> removeChild(Node* child, bool cleanup = true) override
    {
        if (child != nullptr)
            return removeChild(child->getNodeId(), cleanup);
        return node_ptr<Node>();
    }
    virtual node_ptr<Node> removeChild(NodeId id, bool cleanup) override;
    virtual void removeAllChildrenWithCleanup(bool cleanup) override;
    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;

protected:
    /** Adds a child to the container with a z-order, a parallax ratio and a position offset
     It returns self, so you can chain several addChilds.
     @since v0.8
     * @js ctor
     */
    ParallaxNode();

    Vec2 absolutePosition();

private:

    struct PointObject
    {
        PointObject(Vec2 ratio, Vec2 offset, node_ptr<Node> child)
            : _ratio(ratio), _offset(offset), _child(std::move(child))
            {}
        Vec2           _ratio;
        Vec2           _offset;
        node_ptr<Node> _child; // weak ref
    };

private:
protected:

    Vec2    _lastPosition;

    std::vector<PointObject> _parallaxArray;

private:
    ParallaxNode(const ParallaxNode &) = delete;
    const ParallaxNode & operator=(const ParallaxNode &) = delete;
};

// end of _2d group
/// @}

} // namespace cocos2d

#endif //__CCPARALLAX_NODE_H__


