/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2009      Valentin Milea
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

#ifndef __CCNODE_H__
#define __CCNODE_H__

#include <cstdint>
#include "base/ccMacros.h"
#include "base/CCProtocols.h"
#include "base/CCRef.h"
#include "math/CCAffineTransform.h"
#include "math/CCMath.h"
#include "2d/CCComponentContainer.h"
#include "2d/CCComponent.h"

#if CC_USE_PHYSICS
#include "physics/CCPhysicsBody.h"
#endif

#include <vector>
#include <algorithm> // stable_sort

namespace cocos2d {

class GridBase;
class Touch;
class Action;
class LabelProtocol;
class Scheduler;
class Component;
class ComponentContainer;
class EventDispatcher;
class Scene;
class Renderer;
class Director;
class GLProgram;
class GLProgramState;
class Material;
class Camera;
class PhysicsBody;

/**
 * @addtogroup _2d
 * @{
 */

enum {
    kNodeOnEnter,
    kNodeOnExit,
    kNodeOnEnterTransitionDidFinish,
    kNodeOnExitTransitionDidStart,
    kNodeOnCleanup
};

class EventListener;

class Node;

struct node_ptr_deleter {
public:
    void operator()(Node * p) const;
};

template<typename T>
using node_ptr = std::unique_ptr<T, node_ptr_deleter>;

template<typename T>
node_ptr<T> to_node_ptr(T * ptr)
{
    static_assert(std::is_base_of<Node, T>::value,
                  "node_ptr is for Node-derived types only");
    
    if (ptr != nullptr)
    {
        ptr->retain();
    }

    return node_ptr<T>(ptr);
}

template<typename T, typename... Args>
node_ptr<T> make_node_ptr(Args&&... args)
{
    static_assert(std::is_base_of<Node, T>::value,
                  "node_ptr is for Node-derived types only");

    return to_node_ptr( T::create(std::forward<Args>(args)...) );
}

/** @class Node
* @brief Node is the base element of the Scene Graph. Elements of the Scene Graph must be Node objects or subclasses of it.
 The most common Node objects are: Scene, Layer, Sprite, Menu, Label.

 The main features of a Node are:
 - They can contain other Node objects (`addChild`, `getChildByTag`, `removeChild`, etc)
 - They can schedule periodic callback (`schedule`, `unschedule`, etc)
 - They can execute actions (`runAction`, `stopAction`, etc)

 Subclassing a Node usually means (one/all) of:
 - overriding init to initialize resources and schedule callbacks
 - create callbacks to handle the advancement of time
 - overriding `draw` to render the node

 Properties of Node:
 - position (default: x=0, y=0)
 - scale (default: x=1, y=1)
 - rotation (in degrees, clockwise) (default: 0)
 - anchor point (default: x=0, y=0)
 - contentSize (default: width=0, height=0)
 - visible (default: true)

 Limitations:
 - A Node is a "void" object. If you want to draw something on the screen, you should use a Sprite instead. Or subclass Node and override `draw`.

 */

class NodeRegister;

class NodeId {

    friend class NodeRegister;

public:

    bool operator==(NodeId const& r) const
    {
        return index == r.index && signature == r.signature;
    }

private:

    using index_type     = uint16_t;
    using signature_type = uint16_t;

    index_type     index;
    signature_type signature;
};

class CC_DLL Node : public Ref
{
public:
    
    using children_container = std::vector<node_ptr<Node>>;
    using children_iterator  = children_container::iterator;

public:
    /** Default tag used for all the nodes */
    static const int INVALID_TAG = -1;

    enum {
        FLAGS_TRANSFORM_DIRTY = (1 << 0),
        FLAGS_CONTENT_SIZE_DIRTY = (1 << 1),
        FLAGS_RENDER_AS_3D = (1 << 3),

        FLAGS_DIRTY_MASK = (FLAGS_TRANSFORM_DIRTY | FLAGS_CONTENT_SIZE_DIRTY),
    };

    const NodeId & getNodeId() const
    {
        return _id;
    }
    NodeId & getNodeId()
    {
        return _id;
    }

    /// @{
    /// @name Constructor, Destructor and Initializers

    /**
     * Allocates and initializes a node.
     * @return A initialized node which is marked as "autorelease".
     */
    static Node * create();

    /**
     * Gets the description string. It makes debugging easier.
     * @return A string
     */
    virtual std::string getDescription() const;

    /// @} end of initializers



    /// @{
    /// @name Setters & Getters for Graphic Properties

    /**
     LocalZOrder is the 'key' used to sort the node relative to its siblings.

     The Node's parent will sort all its children based on the LocalZOrder value.
     If two nodes have the same LocalZOrder, then the node that was added first to the children's array will be in front of the other node in the array.
     
     Also, the Scene Graph is traversed using the "In-Order" tree traversal algorithm ( http://en.wikipedia.org/wiki/Tree_traversal#In-order )
     And Nodes that have LocalZOrder values < 0 are the "left" subtree
     While Nodes with LocalZOrder >=0 are the "right" subtree.
     
     @see `setGlobalZOrder`
     @see `setPositionZ`
     *
     * @param localZOrder The local Z order value.
     */
    virtual void setLocalZOrder(int localZOrder);

    /* 
     Helper function used by `setLocalZOrder`. Don't use it unless you know what you are doing.
     @js NA
     */
    virtual void _setLocalZOrder(int z);

    /**
     * Gets the local Z order of this node.
     *
     * @see `setLocalZOrder(int)`
     *
     * @return The local (relative to its siblings) Z order.
     */

    virtual int getLocalZOrder() const { return _localZOrder; }

    /**
     Defines the order in which the nodes are renderer.
     Nodes that have a Global Z Order lower, are renderer first.
     
     In case two or more nodes have the same Global Z Order, the order is not guaranteed.
     The only exception if the Nodes have a Global Z Order == 0. In that case, the Scene Graph order is used.
     
     By default, all nodes have a Global Z Order = 0. That means that by default, the Scene Graph order is used to render the nodes.
     
     Global Z Order is useful when you need to render nodes in an order different than the Scene Graph order.
     
     Limitations: Global Z Order can't be used by Nodes that have SpriteBatchNode as one of their ancestors.
     And if ClippingNode is one of the ancestors, then "global Z order" will be relative to the ClippingNode.

     @see `setLocalZOrder()`
     @see `setPositionZ()`

     @since v3.0
     *
     * @param globalZOrder The global Z order value.
     */
    virtual void setGlobalZOrder(float globalZOrder);
    /**
     * Returns the Node's Global Z Order.
     *
     * @see `setGlobalZOrder(int)`
     *
     * @return The node's global Z order
     */
    virtual float getGlobalZOrder() const { return _globalZOrder; }

    /**
     * Sets the scale (x) of the node.
     *
     * It is a scaling factor that multiplies the width of the node and its children.
     *
     * @param scaleX   The scale factor on X axis.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setScaleX(float scaleX);
    /**
     * Returns the scale factor on X axis of this node
     *
     * @see setScaleX(float)
     *
     * @return The scale factor on X axis.
     */
    virtual float getScaleX() const;


    /**
     * Sets the scale (y) of the node.
     *
     * It is a scaling factor that multiplies the height of the node and its children.
     *
     * @param scaleY   The scale factor on Y axis.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setScaleY(float scaleY);
    /**
     * Returns the scale factor on Y axis of this node
     *
     * @see `setScaleY(float)`
     *
     * @return The scale factor on Y axis.
     */
    virtual float getScaleY() const;

    /**
     * Changes the scale factor on Z axis of this node
     *
     * The Default value is 1.0 if you haven't changed it before.
     *
     * @param scaleZ   The scale factor on Z axis.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setScaleZ(float scaleZ);
    /**
     * Returns the scale factor on Z axis of this node
     *
     * @see `setScaleZ(float)`
     *
     * @return The scale factor on Z axis.
     */
    virtual float getScaleZ() const;


    /**
     * Sets the scale (x,y,z) of the node.
     *
     * It is a scaling factor that multiplies the width, height and depth of the node and its children.
     *
     * @param scale     The scale factor for both X and Y axis.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setScale(float scale);
    /**
     * Gets the scale factor of the node,  when X and Y have the same scale factor.
     *
     * @warning Assert when `_scaleX != _scaleY`
     * @see setScale(float)
     *
     * @return The scale factor of the node.
     */
    virtual float getScale() const;

     /**
     * Sets the scale (x,y) of the node.
     *
     * It is a scaling factor that multiplies the width and height of the node and its children.
     *
     * @param scaleX     The scale factor on X axis.
     * @param scaleY     The scale factor on Y axis.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setScale(float scaleX, float scaleY);

    /**
     * Sets the position (x,y) of the node in its parent's coordinate system.
     *
     * Usually we use `Vec2(x,y)` to compose Vec2 object.
     * This code snippet sets the node in the center of screen.
     @code
     Size size = Director::getInstance()->getWinSize();
     node->setPosition(size.width/2, size.height/2);
     @endcode
     *
     * @param position  The position (x,y) of the node in OpenGL coordinates.
     */
    virtual void setPosition(const Vec2 &position);

    /** Sets the position (x,y) using values between 0 and 1.
     The positions in pixels is calculated like the following:
     @code
     // pseudo code
     void setNormalizedPosition(Vec2 pos) {
       Size s = getParent()->getContentSize();
       _position = pos * s;
     }
     @endcode
     *
     * @param position The normalized position (x,y) of the node, using value between 0 and 1.
     */
    virtual void setPositionNormalized(const Vec2 &position);

    /**
     * Gets the position (x,y) of the node in its parent's coordinate system.
     *
     * @see setPosition(const Vec2&)
     *
     * @return The position (x,y) of the node in OpenGL coordinates.
     */
    virtual const Vec2& getPosition() const;

    /** Returns the normalized position.
     * 
     * @return The normalized position.
     */
    virtual const Vec2& getPositionNormalized() const;

    /**
     * Sets the position (x,y) of the node in its parent's coordinate system.
     *
     * Passing two numbers (x,y) is much efficient than passing Vec2 object.
     * This method is bound to Lua and JavaScript.
     * Passing a number is 10 times faster than passing a object from Lua to c++.
     *
     @code
     // sample code in Lua
     local pos  = node::getPosition()  -- returns Vec2 object from C++.
     node:setPosition(x, y)            -- pass x, y coordinate to C++.
     @endcode
     *
     * @param x     X coordinate for position.
     * @param y     Y coordinate for position.
     */
    virtual void setPosition(float x, float y);
    /**
     * Gets position in a more efficient way, returns two number instead of a Vec2 object.
     *
     * @see `setPosition(float, float)`
     * In js,out value not return.
     *
     * @param x To receive x coordinate for position.
     * @param y To receive y coordinate for position.
     */
    virtual void getPosition(float* x, float* y) const;
    /**
     * Gets/Sets x or y coordinate individually for position.
     * These methods are used in Lua and Javascript Bindings
     */
    /** Sets the x coordinate of the node in its parent's coordinate system.
     *
     * @param x The x coordinate of the node.
     */
    virtual void  setPositionX(float x);
    /** Gets the x coordinate of the node in its parent's coordinate system.
     *
     * @return The x coordinate of the node.
     */
    virtual float getPositionX(void) const;
    /** Sets the y coordinate of the node in its parent's coordinate system.
     *
     * @param y The y coordinate of the node.
     */
    virtual void  setPositionY(float y);
    /** Gets the y coordinate of the node in its parent's coordinate system.
     *
     * @return The y coordinate of the node.
     */
    virtual float getPositionY(void) const;

    /**
     * Sets the position (X, Y, and Z) in its parent's coordinate system.
     * 
     * @param position The position (X, Y, and Z) in its parent's coordinate system.
     * @js NA
     */
    virtual void setPosition3D(const Vec3& position);
    /**
     * Returns the position (X,Y,Z) in its parent's coordinate system.
     *
     * @return The position (X, Y, and Z) in its parent's coordinate system.
     * @js NA
     */
    virtual Vec3 getPosition3D() const;

    /**
     * Sets the 'z' coordinate in the position. It is the OpenGL Z vertex value.
     *
     * The OpenGL depth buffer and depth testing are disabled by default. You need to turn them on.
     * In order to use this property correctly.
     *
     * `setPositionZ()` also sets the `setGlobalZValue()` with the positionZ as value.
     *
     * @see `setGlobalZValue()`
     *
     * @param positionZ  OpenGL Z vertex of this node.
     */
    virtual void setPositionZ(float positionZ);

    /**
     * Gets position Z coordinate of this node.
     *
     * @see setPositionZ(float)
     *
     * @return The position Z coordinate of this node.
     */
    virtual float getPositionZ() const;

    /**
     * Changes the X skew angle of the node in degrees.
     *
     * The difference between `setRotationalSkew()` and `setSkew()` is that the first one simulate Flash's skew functionality
     * while the second one uses the real skew function.
     *
     * This angle describes the shear distortion in the X direction.
     * Thus, it is the angle between the Y coordinate and the left edge of the shape
     * The default skewX angle is 0. Positive values distort the node in a CW direction.
     *
     * @param skewX The X skew angle of the node in degrees.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setSkewX(float skewX);
    /**
     * Returns the X skew angle of the node in degrees.
     *
     * @see `setSkewX(float)`
     *
     * @return The X skew angle of the node in degrees.
     */
    virtual float getSkewX() const;


    /**
     * Changes the Y skew angle of the node in degrees.
     *
     * The difference between `setRotationalSkew()` and `setSkew()` is that the first one simulate Flash's skew functionality
     * while the second one uses the real skew function.
     *
     * This angle describes the shear distortion in the Y direction.
     * Thus, it is the angle between the X coordinate and the bottom edge of the shape.
     * The default skewY angle is 0. Positive values distort the node in a CCW direction.
     *
     * @param skewY    The Y skew angle of the node in degrees.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setSkewY(float skewY);
    /**
     * Returns the Y skew angle of the node in degrees.
     *
     * @see `setSkewY(float)`
     *
     * @return The Y skew angle of the node in degrees.
     */
    virtual float getSkewY() const;


    /**
     * Sets the anchor point in percent.
     *
     * anchorPoint is the point around which all transformations and positioning manipulations take place.
     * It's like a pin in the node where it is "attached" to its parent.
     * The anchorPoint is normalized, like a percentage. (0,0) means the bottom-left corner and (1,1) means the top-right corner.
     * But you can use values higher than (1,1) and lower than (0,0) too.
     * The default anchorPoint is (0.5,0.5), so it starts in the center of the node.
     * @note If node has a physics body, the anchor must be in the middle, you can't change this to other value.
     *
     * @param anchorPoint   The anchor point of node.
     */
    virtual void setAnchorPoint(const Vec2& anchorPoint);
    /**
     * Returns the anchor point in percent.
     *
     * @see `setAnchorPoint(const Vec2&)`
     *
     * @return The anchor point of node.
     */
    virtual const Vec2& getAnchorPoint() const;
    /**
     * Returns the anchorPoint in absolute pixels.
     *
     * @warning You can only read it. If you wish to modify it, use anchorPoint instead.
     * @see `getAnchorPoint()`
     *
     * @return The anchor point in absolute pixels.
     */
    virtual const Vec2& getAnchorPointInPoints() const;


    /**
     * Sets the untransformed size of the node.
     *
     * The contentSize remains the same no matter the node is scaled or rotated.
     * All nodes has a size. Layer and Scene has the same size of the screen.
     *
     * @param contentSize   The untransformed size of the node.
     */
    virtual void setContentSize(const Size& contentSize);
    /**
     * Returns the untransformed size of the node.
     *
     * @see `setContentSize(const Size&)`
     *
     * @return The untransformed size of the node.
     */
    virtual const Size& getContentSize() const;


    /**
     * Sets whether the node is visible.
     *
     * The default value is true, a node is default to visible.
     *
     * @param visible   true if the node is visible, false if the node is hidden.
     */
    virtual void setVisible(bool visible);
    /**
     * Determines if the node is visible.
     *
     * @see `setVisible(bool)`
     *
     * @return true if the node is visible, false if the node is hidden.
     */
    virtual bool isVisible() const;


    /**
     * Sets the rotation (angle) of the node in degrees.
     *
     * 0 is the default rotation angle.
     * Positive values rotate node clockwise, and negative values for anti-clockwise.
     *
     * @param rotation     The rotation of the node in degrees.
     */
    virtual void setRotation(float rotation);
    /**
     * Returns the rotation of the node in degrees.
     *
     * @see `setRotation(float)`
     *
     * @return The rotation of the node in degrees.
     */
    virtual float getRotation() const;

    /**
     * Sets the rotation (X,Y,Z) in degrees.
     * Useful for 3d rotations.
     *
     * @warning The physics body doesn't support this.
     *
     * @param rotation The rotation of the node in 3d.
     * @js NA
     */
    virtual void setRotation3D(const Vec3& rotation);
    /**
     * Returns the rotation (X,Y,Z) in degrees.
     * 
     * @return The rotation of the node in 3d.
     * @js NA
     */
    virtual Vec3 getRotation3D() const;
    
    /**
     * Set rotation by quaternion. You should make sure the quaternion is normalized.
     *
     * @param quat The rotation in quaternion, note that the quat must be normalized.
     * @js NA
     */
    virtual void setRotationQuat(const Quaternion& quat);
    
    /**
     * Return the rotation by quaternion, Note that when _rotationZ_X == _rotationZ_Y, the returned quaternion equals to RotationZ_X * RotationY * RotationX,
     * it equals to RotationY * RotationX otherwise.
     *
     * @return The rotation in quaternion.
     * @js NA
     */
    virtual Quaternion getRotationQuat() const;

    /**
     * Sets the X rotation (angle) of the node in degrees which performs a horizontal rotational skew.
     *
     * The difference between `setRotationalSkew()` and `setSkew()` is that the first one simulate Flash's skew functionality,
     * while the second one uses the real skew function.
     *
     * 0 is the default rotation angle.
     * Positive values rotate node clockwise, and negative values for anti-clockwise.
     *
     * @param rotationX    The X rotation in degrees which performs a horizontal rotational skew.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setRotationSkewX(float rotationX);

    /**
     * Gets the X rotation (angle) of the node in degrees which performs a horizontal rotation skew.
     *
     * @see `setRotationSkewX(float)`
     *
     * @return The X rotation in degrees.
     */
    virtual float getRotationSkewX() const;

    /**
     * Sets the Y rotation (angle) of the node in degrees which performs a vertical rotational skew.
     *
     * The difference between `setRotationalSkew()` and `setSkew()` is that the first one simulate Flash's skew functionality,
     * while the second one uses the real skew function.
     *
     * 0 is the default rotation angle.
     * Positive values rotate node clockwise, and negative values for anti-clockwise.
     *
     * @param rotationY    The Y rotation in degrees.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setRotationSkewY(float rotationY);

    /**
     * Gets the Y rotation (angle) of the node in degrees which performs a vertical rotational skew.
     *
     * @see `setRotationSkewY(float)`
     *
     * @return The Y rotation in degrees.
     */
    virtual float getRotationSkewY() const;

    /**
     * Sets whether the anchor point will be (0,0) when you position this node.
     *
     * This is an internal method, only used by Layer and Scene. Don't call it outside framework.
     * The default value is false, while in Layer and Scene are true.
     *
     * @param ignore    true if anchor point will be (0,0) when you position this node.
     */
    virtual void setIgnoreAnchorPointForPosition(bool ignore);
    
    /**
     * Gets whether the anchor point will be (0,0) when you position this node.
     *
     * @see `setIgnoreAnchorPointForPosition(bool)`
     *
     * @return true if the anchor point will be (0,0) when you position this node.
     */
    virtual bool isIgnoreAnchorPointForPosition() const;

    /// @}  end of Setters & Getters for Graphic Properties


    /// @{
    /// @name Children and Parent

    // Adds a child to the container with a local z-order.
    // If the child is added to a 'running' node, then 'onEnter' and 'onEnterTransitionDidFinish' will be called immediately.
    // localZOrder is a Z order for drawing priority. Please refer to `setLocalZOrder(int)`.
    // tag is an integer to identify the node easily. Please refer to `setTag(int)`.
    // name is a string to identify the node easily. Please refer to `setName(int)`.
    CC_DEPRECATED_ATTRIBUTE void addChild(Node * child);
    virtual void addChild(node_ptr<Node> child);

    CC_DEPRECATED_ATTRIBUTE void addChild(Node * child, int localZOrder);
    virtual void addChild(node_ptr<Node> child, int localZOrder);
    
    CC_DEPRECATED_ATTRIBUTE void addChild(Node* child, int localZOrder, int tag);
    virtual void addChild(node_ptr<Node> child, int localZOrder, int tag);

    CC_DEPRECATED_ATTRIBUTE void addChild(Node* child, int localZOrder, const std::string &name);
    virtual void addChild(node_ptr<Node> child, int localZOrder, const std::string &name);

    /**
     * Gets a child from the container with its tag.
     *
     * @param tag   An identifier to find the child node.
     *
     * @return a Node object whose tag equals to the input parameter.
     *
     * Please use `getChildByName()` instead.
     */
     virtual Node * getChildByTag(int tag) const;
    
     /**
     * Gets a child from the container with its tag that can be cast to Type T.
     *
     * @param tag   An identifier to find the child node.
     *
     * @return a Node with the given tag that can be cast to Type T.
    */
    template <typename T>
    T getChildByTag(int tag) const { return static_cast<T>(getChildByTag(tag)); }
    
    /**
     * Gets a child from the container with its name.
     *
     * @param name   An identifier to find the child node.
     *
     * @return a Node object whose name equals to the input parameter.
     *
     * @since v3.2
     */
    virtual Node* getChildByName(const std::string& name) const;
    /**
     * Gets a child from the container with its name that can be cast to Type T.
     *
     * @param name   An identifier to find the child node.
     *
     * @return a Node with the given name that can be cast to Type T.
    */
    template <typename T>
    T getChildByName(const std::string& name) const { return static_cast<T>(getChildByName(name)); }
    /** Search the children of the receiving node to perform processing for nodes which share a name.
     *
     * @param name The name to search for, supports c++11 regular expression.
     * Search syntax options:
     * `//`: Can only be placed at the begin of the search string. This indicates that it will search recursively.
     * `..`: The search should move up to the node's parent. Can only be placed at the end of string.
     * `/` : When placed anywhere but the start of the search string, this indicates that the search should move to the node's children.
     *
     * @code
     * enumerateChildren("//MyName", ...): This searches the children recursively and matches any node with the name `MyName`.
     * enumerateChildren("[[:alnum:]]+", ...): This search string matches every node of its children.
     * enumerateChildren("A[[:digit:]]", ...): This searches the node's children and returns any child named `A0`, `A1`, ..., `A9`.
     * enumerateChildren("Abby/Normal", ...): This searches the node's grandchildren and returns any node whose name is `Normal`
     * and whose parent is named `Abby`.
     * enumerateChildren("//Abby/Normal", ...): This searches recursively and returns any node whose name is `Normal` and whose
     * parent is named `Abby`.
     * @endcode
     *
     * @warning Only support alpha or number for name, and not support unicode.
     *
     * @param callback A callback function to execute on nodes that match the `name` parameter. The function takes the following arguments:
     *  `node` 
     *      A node that matches the name
     *  And returns a boolean result. Your callback can return `true` to terminate the enumeration.
     *
     * @since v3.2
     */
    virtual void enumerateChildren(const std::string &name, std::function<bool(Node* node)> callback) const;
    /**
     * Returns the array of the node's children.
     *
     * @return the array the node's children.
     */
    virtual children_container & getChildren()
    {
        return _children;
    }
    virtual const children_container & getChildren() const
    {
        
        return _children;
    }
    
    /**
     * Sets the parent node.
     *
     * @param parent    A pointer to the parent node.
     */
    virtual void setParent(Node* parent);
    /**
     * Returns a pointer to the parent node.
     *
     * @see `setParent(Node*)`
     *
     * @returns A pointer to the parent node.
     */
    virtual Node* getParent() { return _parent; }
    virtual const Node* getParent() const { return _parent; }


    ////// REMOVES //////

    /**
     * Removes this node itself from its parent node with a cleanup.
     * If the node orphan, then nothing happens.
     * @see `removeFromParentAndCleanup(bool)`
     */
    virtual void removeFromParent();
    /**
     * Removes this node itself from its parent node.
     * If the node orphan, then nothing happens.
     * @param cleanup   true if all actions and callbacks on this node should be removed, false otherwise.
     * @js removeFromParent
     * @lua removeFromParent
     */
    virtual void removeFromParentAndCleanup(bool cleanup);

    /**
     * Removes a child from the container. It will also cleanup all running actions depending on the cleanup parameter.
     *
     * @param child     The child node which will be removed.
     * @param cleanup   True if all running actions and callbacks on the child node will be cleanup, false otherwise.
     */
    CC_DEPRECATED_ATTRIBUTE virtual void removeChild(Node* child, bool cleanup = true) { removeChild(child->getNodeId(), cleanup); }
    virtual void removeChild(NodeId const& id, bool cleanup = true);

    /**
     * Removes a child from the container by tag value. It will also cleanup all running actions depending on the cleanup parameter.
     *
     * @param tag       An integer number that identifies a child node.
     * @param cleanup   True if all running actions and callbacks on the child node will be cleanup, false otherwise.
     *
     * Please use `removeChildByName` instead.
     */
     virtual void removeChildByTag(int tag, bool cleanup = true);
    /**
     * Removes a child from the container by tag value. It will also cleanup all running actions depending on the cleanup parameter.
     *
     * @param name       A string that identifies a child node.
     * @param cleanup   True if all running actions and callbacks on the child node will be cleanup, false otherwise.
     */
    virtual void removeChildByName(const std::string &name, bool cleanup = true);
    /**
     * Removes all children from the container with a cleanup.
     *
     * @see `removeAllChildrenWithCleanup(bool)`
     */
    virtual void removeAllChildren();
    /**
     * Removes all children from the container, and do a cleanup to all running actions depending on the cleanup parameter.
     *
     * @param cleanup   True if all running actions on all children nodes should be cleanup, false otherwise.
     * @js removeAllChildren
     * @lua removeAllChildren
     */
    virtual void removeAllChildrenWithCleanup(bool cleanup);

    /**
     * Reorders a child according to a new z value.
     *
     * @param child     An already added child node. It MUST be already added.
     * @param localZOrder Z order for drawing priority. Please refer to setLocalZOrder(int).
     */
    virtual void reorderChild(Node * child, int localZOrder);

    /**
     * Sorts the children array once before drawing, instead of every time when a child is added or reordered.
     * This approach can improves the performance massively.
     * @note Don't call this manually unless a child added needs to be removed in the same frame.
     */
    virtual void sortAllChildren();

    /**
    * Sorts helper function
    *
    */
    template<typename T> inline
    static void sortNodes(T & nodes)
    {
        using pointer = typename T::value_type;
        using value_type =
            typename std::remove_reference<decltype(**std::begin(nodes))>::type;
        static_assert(std::is_base_of<Node, value_type>::value,
                      "Node::sortNodes: Only accept derived of Node!");

        std::stable_sort(std::begin(nodes), std::end(nodes),
                         [](const pointer & n1, const pointer & n2) {
                             return n1->_localZOrder < n2->_localZOrder;
                         });
    }

    /// @} end of Children and Parent
    
    /// @{
    /// @name Tag & User data

    /**
     * Returns a tag that is used to identify the node easily.
     *
     * @return An integer that identifies the node.
     *
     * Please use `getTag()` instead.
     */
     virtual int getTag() const;
    /**
     * Changes the tag that is used to identify the node easily.
     *
     * Please refer to getTag for the sample code.
     *
     * @param tag   A integer that identifies the node.
     *
     * Please use `setName()` instead.
     */
     virtual void setTag(int tag);
    
    /** Returns a string that is used to identify the node.
     * @return A string that identifies the node.
     * 
     * @since v3.2
     */
    virtual const std::string& getName() const;
    /** Changes the name that is used to identify the node easily.
     * @param name A string that identifies the node.
     *
     * @since v3.2
     */
    virtual void setName(const std::string& name);

    
    /**
     * Returns a custom user data pointer.
     *
     * You can set everything in UserData pointer, a data block, a structure or an object.
     *
     * @return A custom user data pointer.
     * @lua NA
     */
    virtual void* getUserData() { return _userData; }
    /**
    * @lua NA
    */
    virtual const void* getUserData() const { return _userData; }

    /**
     * Sets a custom user data pointer.
     *
     * You can set everything in UserData pointer, a data block, a structure or an object, etc.
     * @warning Don't forget to release the memory manually,
     *          especially before you change this data pointer, and before this node is autoreleased.
     *
     * @param userData  A custom user data pointer.
     * @lua NA
     */
    virtual void setUserData(void *userData);

    /**
     * Returns a user assigned Object.
     *
     * Similar to userData, but instead of holding a void* it holds an object.
     *
     * @return A user assigned Object.
     * @lua NA
     */
    virtual Ref* getUserObject() { return _userObject; }
    /**
    * @lua NA
    */
    virtual const Ref* getUserObject() const { return _userObject; }

    /**
     * Returns a user assigned Object.
     *
     * Similar to UserData, but instead of holding a void* it holds an object.
     * The UserObject will be retained once in this method,
     * and the previous UserObject (if existed) will be released.
     * The UserObject will be released in Node's destructor.
     *
     * @param userObject    A user assigned Object.
     */
    virtual void setUserObject(Ref *userObject);

    /// @} end of Tag & User Data


    /// @{
    /// @name GLProgram
    /**
     * Return the GLProgram (shader) currently used for this node.
     *
     * @return The GLProgram (shader) currently used for this node.
     */
    GLProgram* getGLProgram() const;

    /**
     * Sets the shader program for this node
     *
     * Since v2.0, each rendering node must set its shader program.
     * It should be set in initialize phase.
     @code
     node->setGLrProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
     @endcode
     *
     * @param glprogram The shader program.
     */
    virtual void setGLProgram(GLProgram *glprogram);
    
    /**
     * Return the GLProgramState currently used for this node.
     *
     * @return The GLProgramState currently used for this node.
     */
    GLProgramState *getGLProgramState() const;
    /**
     * Set the GLProgramState for this node.
     *
     * @param glProgramState The GLProgramState for this node.
     */
    virtual void setGLProgramState(GLProgramState *glProgramState);
    
    /// @} end of Shader Program


    /**
     * Returns whether or not the node is "running".
     *
     * If the node is running it will accept event callbacks like onEnter(), onExit(), update().
     *
     * @return Whether or not the node is running.
     */
    bool isPaused() const;

    /// @}  end Script Bindings


    /// @{
    /// @name Event Callbacks

    /**
     * Event callback that is invoked every time when Node enters the 'stage'.
     * If the Node enters the 'stage' with a transition, this event is called when the transition starts.
     * During onEnter you can't access a "sister/brother" node.
     * If you override onEnter, you shall call its parent's one, e.g., Node::onEnter().
     */
    virtual void onEnter();

    /** Event callback that is invoked when the Node enters in the 'stage'.
     * If the Node enters the 'stage' with a transition, this event is called when the transition finishes.
     * If you override onEnterTransitionDidFinish, you shall call its parent's one, e.g. Node::onEnterTransitionDidFinish()
     */
    virtual void onEnterTransitionDidFinish();

    /**
     * Event callback that is invoked every time the Node leaves the 'stage'.
     * If the Node leaves the 'stage' with a transition, this event is called when the transition finishes.
     * During onExit you can't access a sibling node.
     * If you override onExit, you shall call its parent's one, e.g., Node::onExit().
     */
    virtual void onExit();

    /**
     * Event callback that is called every time the Node leaves the 'stage'.
     * If the Node leaves the 'stage' with a transition, this callback is called when the transition starts.
     */
    virtual void onExitTransitionDidStart();

    /// @} end of event callbacks.


    /**
     * Stops all running actions and schedulers
     */
    virtual void cleanup();

    /**
     * Override this method to draw your own node.
     * The following GL states will be enabled by default:
     * - `glEnableClientState(GL_VERTEX_ARRAY);`
     * - `glEnableClientState(GL_COLOR_ARRAY);`
     * - `glEnableClientState(GL_TEXTURE_COORD_ARRAY);`
     * - `glEnable(GL_TEXTURE_2D);`
     * AND YOU SHOULD NOT DISABLE THEM AFTER DRAWING YOUR NODE
     * But if you enable any other GL state, you should disable it after drawing your node.
     * 
     * @param renderer A given renderer.
     * @param transform A transform matrix.
     * @param flags Renderer flag.
     */
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
    virtual void draw() final;

    /**
     * Visits this node's children and draw them recursively.
     *
     * @param renderer A given renderer.
     * @param parentTransform A transform matrix.
     * @param parentFlags Renderer flag.
     */
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
    virtual void visit() final;


    /** Returns the Scene that contains the Node.
     It returns `nullptr` if the node doesn't belong to any Scene.
     This function recursively calls parent->getScene() until parent is a Scene object. The results are not cached. It is that the user caches the results in case this functions is being used inside a loop.
     *
     * @return The Scene that contains the node.
     */
    virtual Scene* getScene() const;

    /**
     * Returns an AABB (axis-aligned bounding-box) in its parent's coordinate system.
     *
     * @return An AABB (axis-aligned bounding-box) in its parent's coordinate system
     */
    virtual Rect getBoundingBox() const;

    /** Set event dispatcher for scene.
     *
     * @param dispatcher The event dispatcher of scene.
     */
    virtual void setEventDispatcher(EventDispatcher* dispatcher);

    /** Get the event dispatcher of scene.
     *
     * @return The event dispatcher of scene.
     */
    virtual EventDispatcher* getEventDispatcher() const { return _eventDispatcher; };

    /// @{
    /// @name Actions

    /**
     * Executes an action, and returns the action that is executed.
     *
     * This node becomes the action's target. Refer to Action::getTarget().
     * @warning Actions don't retain their target.
     *
     * @param action An Action pointer.
     */
    virtual void runAction(std::unique_ptr<Action> action);

    /**
     * Stops and removes all actions from the running action list .
     */
    void stopAllActions();

    /**
     * Removes an action from the running action list by its tag.
     *
     * @param tag   A tag that indicates the action to be removed.
     */
    size_t stopActionByTag(int tag);
    
    /**
     * Removes all actions from the running action list by its tag.
     *
     * @param tag   A tag that indicates the action to be removed.
     */
    void stopAllActionsByTag(int tag);

    /**
     * Removes all actions from the running action list by its flags.
     *
     * @param flags   A flag field that removes actions based on bitwise AND.
     */
    void stopActionsByFlags(unsigned int flags);

    /**
     * Gets an action from the running action list by its tag.
     *
     * @see `setTag(int)`, `getTag()`.
     *
     * @return The action object with the given tag.
     */
    Action* getActionByTag(int tag);

    /**
     * Returns the numbers of actions that are running plus the ones that are schedule to run (actions in actionsToAdd and actions arrays).
     *
     * Composable actions are counted as 1 action. Example:
     *    If you are running 1 Sequence of 7 actions, it will return 1.
     *    If you are running 7 Sequences of 2 actions, it will return 7.
     *
     * @return The number of actions that are running plus the ones that are schedule to run.
     */
    ssize_t getNumberOfRunningActions() const;

    /// @} end of Actions

    /// @{
    /// @name Scheduler

    /**
     * Resumes all scheduled selectors, actions and event listeners.
     * This method is called internally by onEnter.
     */
    virtual void resume(void);
    /**
     * Pauses all scheduled selectors, actions and event listeners.
     * This method is called internally by onExit.
     */
    virtual void pause(void);

    /**
     * @param delta In seconds.
     */
    virtual void update(float delta);

    /// @} end of Scheduler

    /// @{
    /// @name Transformations

    /**
     * Calls children's updateTransform() method recursively.
     *
     * This method is moved from Sprite, so it's no longer specific to Sprite.
     * As the result, you apply SpriteBatchNode's optimization on your customed Node.
     * e.g., `batchNode->addChild(myCustomNode)`, while you can only addChild(sprite) before.
     */
    virtual void updateTransform();

    /**
     * Returns the matrix that transform the node's (local) space coordinates into the parent's space coordinates.
     * The matrix is in Pixels.
     *
     * @return The transformation matrix.
     */
    virtual const Mat4& getNodeToParentTransform() const;
    virtual AffineTransform getNodeToParentAffineTransform() const;

    /**
     * Returns the matrix that transform the node's (local) space coordinates into the parent's space coordinates.
     * The matrix is in Pixels.
     * Note: If ancestor is not a valid ancestor of the node, the API would return the same value as @see getNodeToWorldTransform
     *
     * @param ancestor The parent's node pointer.
     * @since v3.7
     * @return The transformation matrix.
     */
    virtual Mat4 getNodeToParentTransform(Node* ancestor) const;

    /**
     * Returns the affine transform matrix that transform the node's (local) space coordinates into the parent's space coordinates.
     * The matrix is in Pixels.
     *
     * Note: If ancestor is not a valid ancestor of the node, the API would return the same value as @see getNodeToWorldAffineTransform
     *
     * @param ancestor The parent's node pointer.
     * @since v3.7
     * @return The affine transformation matrix.
     */
    virtual AffineTransform getNodeToParentAffineTransform(Node* ancestor) const;

    /** 
     * Sets the transformation matrix manually.
     *
     * @param transform A given transformation matrix.
     */
    virtual void setNodeToParentTransform(const Mat4& transform);

    /**
     * Returns the matrix that transforms parent's space coordinates to the node's (local) space coordinates.
     * The matrix is in Pixels.
     *
     * @return The transformation matrix.
     */
    virtual const Mat4& getParentToNodeTransform() const;
    virtual AffineTransform getParentToNodeAffineTransform() const;

    /**
     * Returns the world affine transform matrix. The matrix is in Pixels.
     *
     * @return transformation matrix, in pixels.
     */
    virtual Mat4 getNodeToWorldTransform() const;
    virtual AffineTransform getNodeToWorldAffineTransform() const;

    /**
     * Returns the inverse world affine transform matrix. The matrix is in Pixels.
     *
     * @return The transformation matrix.
     */
    virtual Mat4 getWorldToNodeTransform() const;
    virtual AffineTransform getWorldToNodeAffineTransform() const;

    /// @} end of Transformations


    /// @{
    /// @name Coordinate Converters

    /**
     * Converts a Vec2 to node (local) space coordinates. The result is in Points.
     *
     * @param worldPoint A given coordinate.
     * @return A point in node (local) space coordinates.
     */
    Vec2 convertToNodeSpace(const Vec2& worldPoint) const;

    /**
     * Converts a Vec2 to world space coordinates. The result is in Points.
     *
     * @param nodePoint A given coordinate.
     * @return A point in world space coordinates.
     */
    Vec2 convertToWorldSpace(const Vec2& nodePoint) const;

    /**
     * Converts a Vec2 to node (local) space coordinates. The result is in Points.
     * treating the returned/received node point as anchor relative.
     *
     * @param worldPoint A given coordinate.
     * @return A point in node (local) space coordinates, anchor relative.
     */
    Vec2 convertToNodeSpaceAR(const Vec2& worldPoint) const;

    /**
     * Converts a local Vec2 to world space coordinates.The result is in Points.
     * treating the returned/received node point as anchor relative.
     *
     * @param nodePoint A given coordinate.
     * @return A point in world space coordinates, anchor relative.
     */
    Vec2 convertToWorldSpaceAR(const Vec2& nodePoint) const;

    /**
     * convenience methods which take a Touch instead of Vec2.
     *
     * @param touch A given touch.
     * @return A point in world space coordinates.
     */
    Vec2 convertTouchToNodeSpace(Touch * touch) const;

    /**
     * converts a Touch (world coordinates) into a local coordinate. This method is AR (Anchor Relative).
     *
     * @param touch A given touch.
     * @return A point in world space coordinates, anchor relative.
     */
    Vec2 convertTouchToNodeSpaceAR(Touch * touch) const;

    /**
     *  Sets an additional transform matrix to the node.
     *
     *  In order to remove it, call it again with the argument `nullptr`.
     *
     * @note The additional transform will be concatenated at the end of getNodeToParentTransform.
     *        It could be used to simulate `parent-child` relationship between two nodes (e.g. one is in BatchNode, another isn't).
     *
     * @param additionalTransform An additional transform matrix.
     */
    void setAdditionalTransform(const Mat4* additionalTransform);
    void setAdditionalTransform(const Mat4& additionalTransform);
    void setAdditionalTransform(const AffineTransform& additionalTransform);

    /// @} end of Coordinate Converters

      /// @{
    /// @name component functions
    /**
     * Gets a component by its name.
     *
     * @param name A given name of component.
     * @return The Component by name.
     */
    Component* getComponent(const std::string& name);

    /**
     * Adds a component.
     *
     * @param component A given component.
     * @return True if added success.
     */
    virtual bool addComponent(Component *component);

    /**
     * Removes a component by its name.
     *
     * @param name A given name of component.
     * @return True if removed success.
     */
    virtual bool removeComponent(const std::string& name);

    /** 
     * Removes a component by its pointer.
     *
     * @param component A given component.
     * @return True if removed success.
     */
    virtual bool removeComponent(Component *component);
    /**
     * Removes all components
     */
    virtual void removeAllComponents();
    /// @} end of component functions
    
    // overrides
    /**
     * Return the node's opacity.
     * @return A GLubyte value.
     */
    virtual GLubyte getOpacity() const;
    /**
     * Return the node's display opacity.
     * The difference between opacity and displayedOpacity is:
     * The displayedOpacity is what's the final rendering opacity of node.
     * @return A GLubyte value.
     */
    virtual GLubyte getDisplayedOpacity() const;
    /**
     * Change node opacity.
     * @param opacity A GLubyte opacity value.
     */
    virtual void setOpacity(GLubyte opacity);
    /**
     * Update the displayed opacity of node with it's parent opacity;
     * @param parentOpacity The opacity of parent node.
     */
    virtual void updateDisplayedOpacity(GLubyte parentOpacity);
    /**
     * Whether cascadeOpacity is enabled or not.
     * @return A boolean value.
     */
    virtual bool isCascadeOpacityEnabled() const;
    /**
     * Change node's cascadeOpacity property.
     * @param cascadeOpacityEnabled True to enable cascadeOpacity, false otherwise.
     */
    virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled);

    /**
     * Query node's color value.
     * @return A Color3B color value.
     */
    virtual const Color3B& getColor() const;
    /**
     * Query node's displayed color.
     * @return A Color3B color value.
     */
    virtual const Color3B& getDisplayedColor() const;
    /**
     * Change the color of node.
     * @param color A Color3B color value.
     */
    virtual void setColor(const Color3B& color);
    /**
     * Update node's displayed color with its parent color.
     * @param parentColor A Color3B color value.
     */
    virtual void updateDisplayedColor(const Color3B& parentColor);
    /**
     * Query whether cascadeColor is enabled or not.
     * @return Whether cascadeColor is enabled or not.
     */
    virtual bool isCascadeColorEnabled() const;
    /**
     * If you want node's color affect the children node's color, then set it to true.
     * Otherwise, set it to false.
     * @param cascadeColorEnabled A boolean value.
     */
    virtual void setCascadeColorEnabled(bool cascadeColorEnabled);

    /**
     *  If you want the opacity affect the color property, then set to true.
     * @param value A boolean value.
     */
    virtual void setOpacityModifyRGB(bool value);
    /**
     * If node opacity will modify the RGB color value, then you should override this method and return true.
     * @return A boolean value, true indicates that opacity will modify color; false otherwise.
     */
    virtual bool isOpacityModifyRGB() const;

    /**
     * Set the callback of event onEnter.
     * @param callback A std::function<void()> callback.
     */
    void setOnEnterCallback(const std::function<void()>& callback) { _onEnterCallback = callback; }
    /**
     * Get the callback of event onEnter.
     * @return A std:function<void()> callback.
     */
    const std::function<void()>& getOnEnterCallback() const { return _onEnterCallback; }
    /**
     * Set the callback of event onExit.
     * @param callback A std::function<void()> callback.
     */
    void setOnExitCallback(const std::function<void()>& callback) { _onExitCallback = callback; }
    /**
     * Get the callback of event onExit.
     * @return A std::function<void()>.
     */
    const std::function<void()>& getOnExitCallback() const { return _onExitCallback; }
    /**
     * Set the callback of event EnterTransitionDidFinish.
     * @param callback A std::function<void()> callback.
     */
    void setonEnterTransitionDidFinishCallback(const std::function<void()>& callback) { _onEnterTransitionDidFinishCallback = callback; }
    /**
     * Get the callback of event EnterTransitionDidFinish.
     * @return std::function<void()>
     */
    const std::function<void()>& getonEnterTransitionDidFinishCallback() const { return _onEnterTransitionDidFinishCallback; }
    /**
     * Set the callback of event ExitTransitionDidStart.
     * @param callback A std::function<void()> callback.
     */
    void setonExitTransitionDidStartCallback(const std::function<void()>& callback) { _onExitTransitionDidStartCallback = callback; }
    /**
     * Get the callback of event ExitTransitionDidStart.
     * @return std::function<void()>
     */
    const std::function<void()>& getonExitTransitionDidStartCallback() const { return _onExitTransitionDidStartCallback; }
    
    /**
     * get & set camera mask, the node is visible by the camera whose camera flag & node's camera mask is true
     */
    unsigned short getCameraMask() const { return _cameraMask; }
    /**
     * Modify the camera mask for current node.
     * If applyChildren is true, then it will modify the camera mask of its children recursively.
     * @param mask A unsigned short bit for mask.
     * @param applyChildren A boolean value to determine whether the mask bit should apply to its children or not.
     */
    virtual void setCameraMask(unsigned short mask, bool applyChildren = true);

protected:
    // Nodes should be created using create();
    Node();
    virtual ~Node();

    virtual bool init();

protected:
    /// helper that reorder a child
    void insertChild(node_ptr<Node> child, int z);

    /// Convert cocos2d coordinates to UI windows coordinate.
    Vec2 convertToWindowSpace(const Vec2& nodePoint) const;

    Mat4 transform(const Mat4 &parentTransform);
    uint32_t processParentFlags(const Mat4& parentTransform, uint32_t parentFlags);

    virtual void updateCascadeOpacity();
    virtual void disableCascadeOpacity();
    virtual void updateCascadeColor();
    virtual void disableCascadeColor();
    virtual void updateColor() {}
    
    bool doEnumerate(std::string name, std::function<bool (Node *)> callback) const;
    bool doEnumerateRecursive(const Node* node, const std::string &name, std::function<bool (Node *)> callback) const;
    
    //check whether this camera mask is visible by the current visiting camera
    bool isVisitableByVisitingCamera() const;
    
    // update quaternion from Rotation3D
    void updateRotationQuat();
    // update Rotation3D from quaternion
    void updateRotation3D();
    
private:
    void addChildHelper(node_ptr<Node> child, int localZOrder, int tag, const std::string &name, bool setTag);
    
    /// Removes a child, call child->onExit(), do cleanup, remove it from children array.
    void detachChild(children_iterator, bool doCleanup);

protected:

    NodeId _id;

    float _rotationX;               ///< rotation on the X-axis
    float _rotationY;               ///< rotation on the Y-axis

    // rotation Z is decomposed in 2 to simulate Skew for Flash animations
    float _rotationZ_X;             ///< rotation angle on Z-axis, component X
    float _rotationZ_Y;             ///< rotation angle on Z-axis, component Y
    
    Quaternion _rotationQuat;       ///rotation using quaternion, if _rotationZ_X == _rotationZ_Y, _rotationQuat = RotationZ_X * RotationY * RotationX, else _rotationQuat = RotationY * RotationX

    float _scaleX;                  ///< scaling factor on x-axis
    float _scaleY;                  ///< scaling factor on y-axis
    float _scaleZ;                  ///< scaling factor on z-axis

    Vec2 _position;                 ///< position of the node
    float _positionZ;               ///< OpenGL real Z position
    Vec2 _normalizedPosition;
    bool _usingNormalizedPosition;
    bool _normalizedPositionDirty;

    float _skewX;                   ///< skew angle on x-axis
    float _skewY;                   ///< skew angle on y-axis

    Vec2 _anchorPointInPoints;      ///< anchor point in points
    Vec2 _anchorPoint;              ///< anchor point normalized (NOT in points)

    Size _contentSize;              ///< untransformed size of the node
    bool _contentSizeDirty;         ///< whether or not the contentSize is dirty

    Mat4 _modelViewTransform;       ///< ModelView transform of the Node.

    // "cache" variables are allowed to be mutable
    mutable Mat4 _transform;        ///< transform
    mutable bool _transformDirty;   ///< transform dirty flag
    mutable Mat4 _inverse;          ///< inverse transform
    mutable bool _inverseDirty;     ///< inverse transform dirty flag
    mutable Mat4* _additionalTransform; ///< two transforms needed by additional transforms
    mutable bool _additionalTransformDirty; ///< transform dirty ?
    bool _transformUpdated;         ///< Whether or not the Transform object was updated since the last frame

    int _localZOrder; /// < Local order (relative to its siblings) used to sort the node

    float _globalZOrder;            ///< Global order used to sort the node

private:
    children_container _children;    ///< array of children nodes

protected:
    Node *_parent;                  ///< weak reference to parent node
    Director* _director;            //cached director pointer to improve rendering performance
    int _tag;                       ///< a tag. Can be any number you assigned just to identify this node
    
    std::string _name;              ///<a string label, an user defined string to identify this node
    size_t _hashOfName;             ///<hash value of _name, used for speed in getChildByName

    void *_userData;                ///< A user assigned void pointer, Can be point to any cpp object
    Ref *_userObject;               ///< A user assigned Object

    GLProgramState *_glProgramState; ///< OpenGL Program State

    EventDispatcher* _eventDispatcher;  ///< event dispatcher used to dispatch all kinds of events

    bool _running;                  ///< is running

    bool _visible;                  ///< is this node visible

    bool _ignoreAnchorPointForPosition; ///< true if the Anchor Vec2 will be (0,0) when you position the Node, false otherwise.
                                          ///< Used by Layer and Scene.

    bool _reorderChildDirty;          ///< children order dirty flag
    bool _isTransitionFinished;       ///< flag to indicate whether the transition was finished

    ComponentContainer *_componentContainer;        ///< Dictionary of components
    
    // opacity controls
    GLubyte     _displayedOpacity;
    GLubyte     _realOpacity;
    Color3B     _displayedColor;
    Color3B     _realColor;
    bool        _cascadeColorEnabled;
    bool        _cascadeOpacityEnabled;

    // camera mask, it is visible only when _cameraMask & current camera' camera flag is true
    unsigned short _cameraMask;
    
    std::function<void()> _onEnterCallback;
    std::function<void()> _onExitCallback;
    std::function<void()> _onEnterTransitionDidFinishCallback;
    std::function<void()> _onExitTransitionDidStartCallback;

//Physics:remaining backwardly compatible  
#if CC_USE_PHYSICS
    PhysicsBody* _physicsBody;
public:
    void setPhysicsBody(PhysicsBody* physicsBody)
    {
        if (_physicsBody != nullptr)
        {
            removeComponent(_physicsBody);
        }

        addComponent(physicsBody);
    }
    PhysicsBody* getPhysicsBody() const { return _physicsBody; }

    friend class PhysicsBody;
#endif

private:
    Node(const Node &) = delete;
    const Node & operator=(const Node &) = delete;
};

inline
void node_ptr_deleter::operator()(Node * p) const
{
    p->release();
}

/**
 * This is a helper function, checks a GL screen point is in content rectangle space.
 *
 * The content rectangle defined by origin(0,0) and content size.
 * This function convert GL screen point to near and far planes as points Pn and Pf,
 * then calculate the intersect point P which the line PnPf intersect with content rectangle.
 * If P in content rectangle means this node be hit.
 *
 * @param pt        The point in GL screen space.
 * @param camera    Which camera used to unproject pt to near/far planes.
 * @param w2l       World to local transform matrix, used to convert Pn and Pf to rectangle space.
 * @param rect      The test rectangle in local space.
 * @parma p         Point to a Vec3 for store the intersect point, if don't need them set to nullptr.
 * @return true if the point is in content rectangle, false otherwise.
 */
bool CC_DLL isScreenPointInRect(const Vec2 &pt, const Camera* camera, const Mat4& w2l, const Rect& rect, Vec3 *p);

} // namespace cocos2d

#endif // __CCNODE_H__
