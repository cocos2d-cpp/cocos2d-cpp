/****************************************************************************
 Copyright (c) 2013 Chukong Technologies Inc.
 
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

#include "physics/CCPhysicsWorld.h"
#if CC_USE_PHYSICS
#include <algorithm>
#include <climits>

#include "chipmunk/chipmunk_private.h"
#include "physics/CCPhysicsBody.h"
#include "physics/CCPhysicsShape.h"
#include "physics/CCPhysicsContact.h"
#include "physics/CCPhysicsJoint.h"
#include "physics/CCPhysicsHelper.h"

#include "2d/CCDrawNode.h"
#include "2d/CCScene.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventCustom.h"

namespace cocos2d {

const float PHYSICS_INFINITY = FLT_MAX;
extern const char* PHYSICSCONTACT_EVENT_NAME;

namespace
{
    typedef struct RayCastCallbackInfo
    {
        PhysicsWorld* world;
        PhysicsRayCastCallbackFunc func;
        Vec2 p1;
        Vec2 p2;
        void* data;
    }RayCastCallbackInfo;
    
    typedef struct RectQueryCallbackInfo
    {
        PhysicsWorld* world;
        PhysicsQueryRectCallbackFunc func;
        void* data;
    }RectQueryCallbackInfo;
    
    typedef struct PointQueryCallbackInfo
    {
        PhysicsWorld* world;
        PhysicsQueryPointCallbackFunc func;
        void* data;
    }PointQueryCallbackInfo;
}

class PhysicsWorld::Callbacks
{
public:
    static cpBool collisionBeginCallbackFunc(cpArbiter *arb, struct cpSpace *space, PhysicsWorld *world);
    static cpBool collisionPreSolveCallbackFunc(cpArbiter *arb, cpSpace *space, PhysicsWorld *world);
    static void collisionPostSolveCallbackFunc(cpArbiter *arb, cpSpace *space, PhysicsWorld *world);
    static void collisionSeparateCallbackFunc(cpArbiter *arb, cpSpace *space, PhysicsWorld *world);
    static void rayCastCallbackFunc(cpShape *shape, cpVect point, cpVect normal, cpFloat alpha, RayCastCallbackInfo *info);
    static void queryRectCallbackFunc(cpShape *shape, RectQueryCallbackInfo *info);
    static void queryPointFunc(cpShape *shape, cpVect point, cpFloat distance, cpVect gradient, PointQueryCallbackInfo *info);
    static void getShapesAtPointFunc(cpShape *shape, cpVect point, cpFloat distance, cpVect gradient, std::vector<retaining_ptr<PhysicsShape>>* arr);
    
public:
    static bool continues;
};

bool PhysicsWorld::Callbacks::continues = true;

cpBool PhysicsWorld::Callbacks::collisionBeginCallbackFunc(cpArbiter *arb, struct cpSpace* /*space*/, PhysicsWorld *world)
{
    CP_ARBITER_GET_SHAPES(arb, a, b);
    
    PhysicsShape *shapeA = static_cast<PhysicsShape*>(cpShapeGetUserData(a));
    PhysicsShape *shapeB = static_cast<PhysicsShape*>(cpShapeGetUserData(b));
    CC_ASSERT(shapeA != nullptr && shapeB != nullptr);
    
    auto contact = PhysicsContact::construct(shapeA, shapeB);
    cpArbiterSetUserData(arb, contact);
    contact->_contactInfo = arb;
    
    return world->collisionBeginCallback(*contact);
}

cpBool PhysicsWorld::Callbacks::collisionPreSolveCallbackFunc(cpArbiter *arb, cpSpace* /*space*/, PhysicsWorld *world)
{
    return world->collisionPreSolveCallback(*static_cast<PhysicsContact*>(cpArbiterGetUserData(arb)));
}

void PhysicsWorld::Callbacks::collisionPostSolveCallbackFunc(cpArbiter *arb, cpSpace* /*space*/, PhysicsWorld *world)
{
    world->collisionPostSolveCallback(*static_cast<PhysicsContact*>(cpArbiterGetUserData(arb)));
}

void PhysicsWorld::Callbacks::collisionSeparateCallbackFunc(cpArbiter *arb, cpSpace* /*space*/, PhysicsWorld *world)
{
    PhysicsContact* contact = static_cast<PhysicsContact*>(cpArbiterGetUserData(arb));
    
    world->collisionSeparateCallback(*contact);
    
    delete contact;
}

void PhysicsWorld::Callbacks::rayCastCallbackFunc(cpShape *shape, cpVect point, cpVect normal, cpFloat alpha, RayCastCallbackInfo *info)
{
    if (!continues)
    {
        return;
    }
    
    PhysicsShape *physicsShape = static_cast<PhysicsShape*>(cpShapeGetUserData(shape));
    CC_ASSERT(physicsShape != nullptr);
    
    PhysicsRayCastInfo callbackInfo =
    {
        physicsShape,
        info->p1,
        info->p2,
        PhysicsHelper::cpv2point(point),
        PhysicsHelper::cpv2point(normal),
        static_cast<float>(alpha),
        nullptr
    };
    
    continues = info->func(*info->world, callbackInfo, info->data);
}

void PhysicsWorld::Callbacks::queryRectCallbackFunc(cpShape *shape, RectQueryCallbackInfo *info)
{
    PhysicsShape *physicsShape = static_cast<PhysicsShape*>(cpShapeGetUserData(shape));
    CC_ASSERT(physicsShape != nullptr);
    
    if (!continues)
    {
        return;
    }
    
    continues = info->func(*info->world, *physicsShape, info->data);
}

void PhysicsWorld::Callbacks::getShapesAtPointFunc(cpShape *shape, cpVect /*point*/, cpFloat /*distance*/, cpVect /*gradient*/, std::vector<retaining_ptr<PhysicsShape>>* arr)
{
    PhysicsShape *physicsShape = static_cast<PhysicsShape*>(cpShapeGetUserData(shape));
    CC_ASSERT(physicsShape != nullptr);
    arr->push_back( to_retaining_ptr( physicsShape));
}

void PhysicsWorld::Callbacks::queryPointFunc(cpShape *shape, cpVect /*point*/, cpFloat /*distance*/, cpVect /*gradient*/, PointQueryCallbackInfo *info)
{
    PhysicsShape *physicsShape = static_cast<PhysicsShape*>(cpShapeGetUserData(shape));
    CC_ASSERT(physicsShape != nullptr);
    continues = info->func(*info->world, *physicsShape, info->data);
}

static inline cpSpaceDebugColor RGBAColor(float r, float g, float b, float a){
    cpSpaceDebugColor color = {r, g, b, a};
    return color;
}

static inline cpSpaceDebugColor LAColor(float l, float a){
    cpSpaceDebugColor color = {l, l, l, a};
    return color;
}

static void DrawCircle(cpVect p, cpFloat /*a*/, cpFloat r, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data)
{
    const Color4F fillColor(fill.r, fill.g, fill.b, fill.a);
    const Color4F outlineColor(outline.r, outline.g, outline.b, outline.a);
    DrawNode* drawNode = static_cast<DrawNode*>(data);
    float radius = PhysicsHelper::cpfloat2float(r);
    Vec2 centre = PhysicsHelper::cpv2point(p);
    
    static const int CIRCLE_SEG_NUM = 12;
    Vec2 seg[CIRCLE_SEG_NUM] = {};
    
    for (int i = 0; i < CIRCLE_SEG_NUM; ++i)
    {
        float angle = (float)i * M_PI / (float)CIRCLE_SEG_NUM * 2.0f;
        Vec2 d(radius * cosf(angle), radius * sinf(angle));
        seg[i] = centre + d;
    }
    drawNode->drawPolygon(seg, CIRCLE_SEG_NUM, fillColor, 1, outlineColor);
}

static void DrawFatSegment(cpVect a, cpVect b, cpFloat r, cpSpaceDebugColor outline, cpSpaceDebugColor /*fill*/, cpDataPointer data)
{
    const Color4F outlineColor(outline.r, outline.g, outline.b, outline.a);
    DrawNode* drawNode = static_cast<DrawNode*>(data);
    drawNode->drawSegment(PhysicsHelper::cpv2point(a),
                          PhysicsHelper::cpv2point(b),
                          PhysicsHelper::cpfloat2float(r==0 ? 1 : r), outlineColor);
}

static void DrawSegment(cpVect a, cpVect b, cpSpaceDebugColor color, cpDataPointer data)
{
    DrawFatSegment(a, b, 0.0, color, color, data);
}

static void DrawPolygon(int count, const cpVect *verts, cpFloat /*r*/, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data)
{
    const Color4F fillColor(fill.r, fill.g, fill.b, fill.a);
    const Color4F outlineColor(outline.r, outline.g, outline.b, outline.a);
    DrawNode* drawNode = static_cast<DrawNode*>(data);
    int num = count;
    Vec2* seg = new (std::nothrow) Vec2[num];
    for(int i=0;i<num;++i)
        seg[i] = PhysicsHelper::cpv2point(verts[i]);
    
    drawNode->drawPolygon(seg, num, fillColor, 1.0f, outlineColor);
    
    delete[] seg;
}

static void DrawDot(cpFloat /*size*/, cpVect pos, cpSpaceDebugColor color, cpDataPointer data)
{
    const Color4F dotColor(color.r, color.g, color.b, color.a);
    DrawNode* drawNode = static_cast<DrawNode*>(data);
    drawNode->drawDot(PhysicsHelper::cpv2point(pos), 2, dotColor);
}

static cpSpaceDebugColor ColorForShape(cpShape *shape, cpDataPointer /*data*/)
{
    if(cpShapeGetSensor(shape)){
        return LAColor(1.0f, 0.3f);
    } else {
        cpBody *body = cpShapeGetBody(shape);
        
        if(cpBodyIsSleeping(body)){
            return LAColor(0.2f, 0.3f);
        } else if(body->sleeping.idleTime > shape->space->sleepTimeThreshold) {
            return LAColor(0.66f, 0.3f);
        } else {
            
            GLfloat intensity = (cpBodyGetType(body) == CP_BODY_TYPE_STATIC ? 0.15f : 0.75f);
            return RGBAColor(intensity, 0.0f, 0.0f, 0.3f);
        }
    }
}


void PhysicsWorld::debugDraw()
{
    if (_debugDraw == nullptr)
    {
        _debugDraw = DrawNode::create();
        _debugDraw->retain();
        Director::getInstance()->getRunningScene()->addChild(_debugDraw);
    }
    
    cpSpaceDebugDrawOptions drawOptions = {
        DrawCircle,
        DrawSegment,
        DrawFatSegment,
        DrawPolygon,
        DrawDot,
        
        (cpSpaceDebugDrawFlags)(_debugDrawMask),
        
        {1.0f, 0.0f, 0.0f, 1.0f},
        ColorForShape,
        {0.0f, 0.75f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        _debugDraw,
    };
    if (_debugDraw)
    {
        _debugDraw->clear();
        cpSpaceDebugDraw(_cpSpace, &drawOptions);
    }
}

bool PhysicsWorld::collisionBeginCallback(PhysicsContact& contact)
{
    bool ret = true;
    
    PhysicsShape* shapeA = contact.getShapeA();
    PhysicsShape* shapeB = contact.getShapeB();
    PhysicsBody* bodyA = shapeA->getBody();
    PhysicsBody* bodyB = shapeB->getBody();
    std::vector<PhysicsJoint*> jointsA = bodyA->getJoints();
    
    // check the joint is collision enable or not
    for (PhysicsJoint* joint : jointsA)
    {
        if (std::find(_joints.begin(), _joints.end(), joint) == _joints.end())
        {
            continue;
        }
        
        if (!joint->isCollisionEnabled())
        {
            PhysicsBody* body = joint->getBodyA() == bodyA ? joint->getBodyB() : joint->getBodyA();
            
            if (body == bodyB)
            {
                contact.setNotificationEnable(false);
                return false;
            }
        }
    }
    
    // bitmask check
    if ((shapeA->getCategoryBitmask() & shapeB->getContactTestBitmask()) == 0
        || (shapeA->getContactTestBitmask() & shapeB->getCategoryBitmask()) == 0)
    {
        contact.setNotificationEnable(false);
    }
    
    if (shapeA->getGroup() != 0 && shapeA->getGroup() == shapeB->getGroup())
    {
        ret = shapeA->getGroup() > 0;
    }
    else
    {
        if ((shapeA->getCategoryBitmask() & shapeB->getCollisionBitmask()) == 0
            || (shapeB->getCategoryBitmask() & shapeA->getCollisionBitmask()) == 0)
        {
            ret = false;
        }
    }
    
    if (contact.isNotificationEnabled())
    {
        contact.setEventCode(PhysicsContact::EventCode::BEGIN);
        contact.setWorld(this);
        _eventDispatcher->dispatchEvent(&contact);
    }
    
    return ret ? contact.resetResult() : false;
}

bool PhysicsWorld::collisionPreSolveCallback(PhysicsContact& contact)
{
    if (!contact.isNotificationEnabled())
    {
        return true;
    }
    
    contact.setEventCode(PhysicsContact::EventCode::PRESOLVE);
    contact.setWorld(this);
    _eventDispatcher->dispatchEvent(&contact);
    
    return contact.resetResult();
}

void PhysicsWorld::collisionPostSolveCallback(PhysicsContact& contact)
{
    if (!contact.isNotificationEnabled())
    {
        return;
    }
    
    contact.setEventCode(PhysicsContact::EventCode::POSTSOLVE);
    contact.setWorld(this);
    _eventDispatcher->dispatchEvent(&contact);
}

void PhysicsWorld::collisionSeparateCallback(PhysicsContact& contact)
{
    if (!contact.isNotificationEnabled())
    {
        return;
    }
    
    contact.setEventCode(PhysicsContact::EventCode::SEPARATE);
    contact.setWorld(this);
    _eventDispatcher->dispatchEvent(&contact);
}

void PhysicsWorld::rayCast(PhysicsRayCastCallbackFunc func, const Vec2& point1, const Vec2& point2, void* data)
{
    CCASSERT(func != nullptr, "func shouldn't be nullptr");
    
    if (func != nullptr)
    {
        if (!_delayAddBodies.empty() || !_delayRemoveBodies.empty())
        {
            updateBodies();
        }
        RayCastCallbackInfo info = { this, func, point1, point2, data };
        
        PhysicsWorld::Callbacks::continues = true;
        cpSpaceSegmentQuery(_cpSpace,
                            PhysicsHelper::point2cpv(point1),
                            PhysicsHelper::point2cpv(point2),
                            0.0f,
                            CP_SHAPE_FILTER_ALL,
                            (cpSpaceSegmentQueryFunc)PhysicsWorld::Callbacks::rayCastCallbackFunc,
                            &info);
    }
}

void PhysicsWorld::queryRect(PhysicsQueryRectCallbackFunc func, const Rect& rect, void* data)
{
    CCASSERT(func != nullptr, "func shouldn't be nullptr");
    
    if (func != nullptr)
    {
        if (!_delayAddBodies.empty() || !_delayRemoveBodies.empty())
        {
            updateBodies();
        }
        RectQueryCallbackInfo info = {this, func, data};
        
        PhysicsWorld::Callbacks::continues = true;
        cpSpaceBBQuery(_cpSpace,
                       PhysicsHelper::rect2cpbb(rect),
                       CP_SHAPE_FILTER_ALL,
                       (cpSpaceBBQueryFunc)PhysicsWorld::Callbacks::queryRectCallbackFunc,
                       &info);
    }
}

void PhysicsWorld::queryPoint(PhysicsQueryPointCallbackFunc func, const Vec2& point, void* data)
{
    CCASSERT(func != nullptr, "func shouldn't be nullptr");
    
    if (func != nullptr)
    {
        if (!_delayAddBodies.empty() || !_delayRemoveBodies.empty())
        {
            updateBodies();
        }
        PointQueryCallbackInfo info = {this, func, data};
        
        PhysicsWorld::Callbacks::continues = true;
        cpSpacePointQuery(_cpSpace,
                                 PhysicsHelper::point2cpv(point),
                                 0,
                                 CP_SHAPE_FILTER_ALL,
                                 (cpSpacePointQueryFunc)PhysicsWorld::Callbacks::queryPointFunc,
                                 &info);
    }
}

std::vector<retaining_ptr<PhysicsShape>> PhysicsWorld::getShapes(const Vec2& point) const
{
    std::vector<retaining_ptr<PhysicsShape>> arr;
    cpSpacePointQuery(_cpSpace,
                      PhysicsHelper::point2cpv(point),
                      0,
                      CP_SHAPE_FILTER_ALL,
                      (cpSpacePointQueryFunc)PhysicsWorld::Callbacks::getShapesAtPointFunc,
                      &arr);

    return arr;
}

PhysicsShape* PhysicsWorld::getShape(const Vec2& point) const
{
    cpShape* shape = cpSpacePointQueryNearest(_cpSpace,
                                    PhysicsHelper::point2cpv(point),
                                    0,
                                    CP_SHAPE_FILTER_ALL,
                                    nullptr);
    return shape == nullptr ? nullptr : static_cast<PhysicsShape*>(cpShapeGetUserData(shape));
}

bool PhysicsWorld::init()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    _cpSpace = cpSpaceNew();
#else
    _cpSpace = cpHastySpaceNew();
    cpHastySpaceSetThreads(_cpSpace, 0);
#endif

    if (!_cpSpace)
    {
        return false;
    }

    cpSpaceSetGravity(_cpSpace, PhysicsHelper::point2cpv(_gravity));

    cpCollisionHandler *handler = cpSpaceAddDefaultCollisionHandler(_cpSpace);
    handler->userData = this;
    handler->beginFunc = (cpCollisionBeginFunc)PhysicsWorld::Callbacks::collisionBeginCallbackFunc;
    handler->preSolveFunc = (cpCollisionPreSolveFunc)PhysicsWorld::Callbacks::collisionPreSolveCallbackFunc;
    handler->postSolveFunc = (cpCollisionPostSolveFunc)PhysicsWorld::Callbacks::collisionPostSolveCallbackFunc;
    handler->separateFunc = (cpCollisionSeparateFunc)PhysicsWorld::Callbacks::collisionSeparateCallbackFunc;

    return true;
}

// ADD

void PhysicsWorld::addBody(PhysicsBody* body)
{
    CCASSERT(body != nullptr, "the body can not be nullptr");
    
    if (body->getWorld() == this)
    {
        return;
    }
    
    if (body->getWorld() != nullptr)
    {
        body->removeFromWorld();
    }
    
    addBodyOrDelay(body);
    _bodies.push_back( to_retaining_ptr( body));
    body->_world = this;
}

void PhysicsWorld::doAddBody(PhysicsBody* body)
{
    if (body->isEnabled())
    {
        // add body to space
        if (!cpSpaceContainsBody(_cpSpace, body->_cpBody))
        {
            cpSpaceAddBody(_cpSpace, body->_cpBody);
        }
        
        // add shapes to space
        for (auto& shape : body->getShapes())
        {
            addShape(dynamic_cast<PhysicsShape*>(shape.get()));
        }
    }
}

void PhysicsWorld::addBodyOrDelay(PhysicsBody* body)
{
    auto removeBodyIter = std::find_if(
        _delayRemoveBodies.begin(), _delayRemoveBodies.end(),
        [body](const retaining_ptr<PhysicsBody> & p) {
            return p.get() == body;
        }
    );

    if (removeBodyIter != _delayRemoveBodies.end())
    {
        _delayRemoveBodies.erase(removeBodyIter);
        return;
    }

    auto addBodyIter = std::find_if(
        _delayAddBodies.begin(), _delayAddBodies.end(),
        [body](const retaining_ptr<PhysicsBody> & p) {
            return p.get() == body;
        }
    );
    
    if (addBodyIter == _delayAddBodies.end())
    {
        _delayAddBodies.push_back( to_retaining_ptr( body));
    }
}

void PhysicsWorld::updateBodies()
{
    if (cpSpaceIsLocked(_cpSpace))
    {
        return;
    }
    
    // issue #4944, contact callback will be invoked when add/remove body, _delayAddBodies maybe changed, so we need make a copy.
    auto addCopy = std::move( _delayAddBodies );
    for (auto& body : addCopy)
    {
        doAddBody(body.get());
    }
    
    auto removeCopy = std::move( _delayRemoveBodies );
    for (auto& body : removeCopy)
    {
        doRemoveBody(body.get());
    }
}

void PhysicsWorld::removeBody(int tag)
{
    for (auto& body : _bodies)
    {
        if (body->getTag() == tag)
        {
            removeBody(body.get());
            return;
        }
    }
}

void PhysicsWorld::removeBody(PhysicsBody* body)
{
    if (body->getWorld() != this)
    {
        CCLOG("Physics Warning: this body doesn't belong to this world");
        return;
    }
    
    // destroy the body's joints
    auto removeCopy = body->_joints;
    for (auto joint : removeCopy)
    {
        removeJoint(joint, true);
    }
    body->_joints.clear();
    
    removeBodyOrDelay(body);
    
    auto it = std::find_if(
        _bodies.begin(), _bodies.end(),
        [body](const retaining_ptr<PhysicsBody> & p) {
            return p.get() == body;
        }
    );
    if (_bodies.end() != it)
        _bodies.erase(it);

    body->_world = nullptr;
}

void PhysicsWorld::removeBodyOrDelay(PhysicsBody* body)
{
    auto addBodyIter = std::find_if(
        _delayAddBodies.begin(), _delayAddBodies.end(),
        [body](const retaining_ptr<PhysicsBody> & p) {
            return p.get() == body;
        }
    );

    if (_delayAddBodies.end() != addBodyIter)
    {
        _delayAddBodies.erase(addBodyIter);
        return;
    }
    
    if (cpSpaceIsLocked(_cpSpace))
    {
        auto removeBodyIter = std::find_if(
            _delayRemoveBodies.begin(), _delayRemoveBodies.end(),
            [body](const retaining_ptr<PhysicsBody> & p) {
                return p.get() == body;
            }
        );

        if (_delayRemoveBodies.end() == removeBodyIter)
        {
            _delayRemoveBodies.push_back( to_retaining_ptr( body));
        }
    }
    else
    {
        doRemoveBody(body);
    }
}

void PhysicsWorld::removeJoint(PhysicsJoint* joint, bool destroy)
{
    if (joint)
    {
        if (joint->getWorld() != this && destroy)
        {
            CCLOG("physics warning: the joint is not in this world, it won't be destroyed until the body it connects is destroyed");
            return;
        }

        joint->_destroyMark = destroy;

        bool removedFromDelayAdd = false;
        auto it = std::find(_delayAddJoints.begin(), _delayAddJoints.end(), joint);
        if (it != _delayAddJoints.end())
        {
            _delayAddJoints.erase(it);
            removedFromDelayAdd = true;
        }

        if (cpSpaceIsLocked(_cpSpace))
        {
            if (removedFromDelayAdd)
                return;
            if (std::find(_delayRemoveJoints.rbegin(), _delayRemoveJoints.rend(), joint) == _delayRemoveJoints.rend())
            {
                _delayRemoveJoints.push_back(joint);
            }
        }
        else
        {
            doRemoveJoint(joint);
        }
    }
}

void PhysicsWorld::updateJoints()
{
    if (cpSpaceIsLocked(_cpSpace))
    {
        return;
    }
    
    for (auto joint : _delayAddJoints)
    {
        joint->_world = this;
        if (joint->initJoint(_cpSpace))
        {
            _joints.push_back(joint);
        }
        else
        {
            delete joint;
        }
    }
    _delayAddJoints.clear();

    for (auto joint : _delayRemoveJoints)
    {
        doRemoveJoint(joint);
    }
    _delayRemoveJoints.clear();
}

void PhysicsWorld::removeShape(PhysicsShape* shape)
{
    if (shape)
    {
        for (auto cps : shape->_cpShapes)
        {
            if (cpSpaceContainsShape(_cpSpace, cps))
            {
                cpSpaceRemoveShape(_cpSpace, cps);
            }
        }
    }
}

void PhysicsWorld::addJoint(PhysicsJoint* joint)
{
    if (joint)
    {
        CCASSERT(joint->getWorld() == nullptr, "Can not add joint already add to other world!");

        joint->_world = this;
        auto it = std::find(_delayRemoveJoints.begin(), _delayRemoveJoints.end(), joint);
        if (it != _delayRemoveJoints.end())
        {
            _delayRemoveJoints.erase(it);
            return;
        }

        if (std::find(_delayAddJoints.begin(), _delayAddJoints.end(), joint) == _delayAddJoints.end())
        {
            _delayAddJoints.push_back(joint);
        }
    }
}

void PhysicsWorld::removeAllJoints(bool destroy)
{
    auto removeCopy = _joints;
    for (auto joint : removeCopy)
    {
        removeJoint(joint, destroy);
    }
}

void PhysicsWorld::addShape(PhysicsShape* physicsShape)
{
    if (physicsShape)
    {
        for (auto shape : physicsShape->_cpShapes)
        {
            cpSpaceAddShape(_cpSpace, shape);
        }
    }
}

void PhysicsWorld::doRemoveBody(PhysicsBody* body)
{
    CCASSERT(body != nullptr, "the body can not be nullptr");
    
    // remove shapes
    for (auto& shape : body->getShapes())
    {
        removeShape(shape.get());
    }
    
    // remove body
    if (cpSpaceContainsBody(_cpSpace, body->_cpBody))
    {
        cpSpaceRemoveBody(_cpSpace, body->_cpBody);
    }
}

void PhysicsWorld::doRemoveJoint(PhysicsJoint* joint)
{
    for (auto constraint : joint->_cpConstraints)
    {
        cpSpaceRemoveConstraint(_cpSpace, constraint);
    }
    _joints.remove(joint);
    joint->_world = nullptr;

    if (joint->getBodyA())
    {
        joint->getBodyA()->removeJoint(joint);
    }

    if (joint->getBodyB())
    {
        joint->getBodyB()->removeJoint(joint);
    }

    if (joint->_destroyMark)
    {
        delete joint;
    }
}

void PhysicsWorld::removeAllBodies()
{
    for (auto& child : _bodies)
    {
        removeBodyOrDelay(child.get());
        child->_world = nullptr;
    }
    
    _bodies.clear();
}

void PhysicsWorld::setDebugDrawMask(DebugDraw mask)
{
    if (mask == DebugDraw::NONE)
    {
        _debugDraw->removeFromParent();
        CC_SAFE_RELEASE_NULL(_debugDraw);
    }
    
    _debugDrawMask = mask;
}

PhysicsBody* PhysicsWorld::getBody(int tag) const
{
    for (auto& body : _bodies)
    {
        if (body->getTag() == tag)
        {
            return body.get();
        }
    }
    
    return nullptr;
}

void PhysicsWorld::setGravity(const Vec2& gravity)
{
    _gravity = gravity;
    cpSpaceSetGravity(_cpSpace, PhysicsHelper::point2cpv(gravity));
}

void PhysicsWorld::setSubsteps(int steps)
{
    if(steps > 0)
    {
        _substeps = steps;
        if (steps > 1)
        {
          _updateRate = 1;
        }
    }
}

void PhysicsWorld::step(float delta)
{
    if (_autoStep)
    {
        CCLOG("Physics Warning: You need to close auto step( setAutoStep(false) ) first");
    }
    else
    {
        update(delta, true);
    }
}

void PhysicsWorld::update(float delta, bool userCall/* = false*/)
{
    if(!_delayAddBodies.empty())
    {
        updateBodies();
    }
    else if (!_delayRemoveBodies.empty())
    {
        updateBodies();
    }
    
    auto sceneToWorldTransform = _scene->getNodeToParentTransform();
    beforeSimulation(_scene, sceneToWorldTransform, 1.f, 1.f, 0.f);

    if (!_delayAddJoints.empty() || !_delayRemoveJoints.empty())
    {
        updateJoints();
    }
    
    if (delta < FLT_EPSILON)
    {
        return;
    }
    
    if (userCall)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		cpSpaceStep(_cpSpace, delta);
#else
		cpHastySpaceStep(_cpSpace, delta);
#endif
    }
    else
    {
        _updateTime += delta;
        if(_fixedRate)
        {
            const float step = 1.0f / _fixedRate;
            const float dt = step * _speed;
            while(_updateTime>step)
            {
                _updateTime-=step;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
				cpSpaceStep(_cpSpace, dt);
#else
				cpHastySpaceStep(_cpSpace, dt);
#endif
			}
        }
        else
        {
            if (++_updateRateCount >= _updateRate)
            {
                const float dt = _updateTime * _speed / _substeps;
                for (int i = 0; i < _substeps; ++i)
                {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
					cpSpaceStep(_cpSpace, dt);
#else
					cpHastySpaceStep(_cpSpace, dt);
#endif 
					for (auto& body : _bodies)
                    {
                        body->update(dt);
                    }
                }
                _updateRateCount = 0;
                _updateTime = 0.0f;
            }
        }
    }
    
    if (_debugDrawMask != DebugDraw::NONE)
    {
        debugDraw();
    }

    // Update physics position, should loop as the same sequence as node tree.
    // PhysicsWorld::afterSimulation() will depend on the sequence.
    afterSimulation(_scene, sceneToWorldTransform, 0.f);
}

PhysicsWorld* PhysicsWorld::construct(Scene* scene)
{
    PhysicsWorld * world = new (std::nothrow) PhysicsWorld();
    if (world && world->init())
    {
        world->_scene = scene;
        world->_eventDispatcher = scene->getEventDispatcher();
        return world;
    }

    CC_SAFE_DELETE(world);
    return nullptr;
}

PhysicsWorld::PhysicsWorld()
: _gravity(Vec2(0.0f, -98.0f))
, _speed(1.0f)
, _updateRate(1)
, _updateRateCount(0)
, _updateTime(0.0f)
, _substeps(1)
, _fixedRate(0)
, _cpSpace(nullptr)
, _updateBodyTransform(false)
, _scene(nullptr)
, _autoStep(true)
, _debugDraw(nullptr)
, _debugDrawMask(DebugDraw::NONE)
, _eventDispatcher(nullptr)
{
    
}

PhysicsWorld::~PhysicsWorld()
{
    removeAllJoints(true);
    removeAllBodies();
    if (_cpSpace)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		cpSpaceFree(_cpSpace);
#else
		cpHastySpaceFree(_cpSpace);
#endif 
    }
    CC_SAFE_RELEASE_NULL(_debugDraw);
}

void PhysicsWorld::beforeSimulation(Node *node, const Mat4& parentToWorldTransform, float nodeParentScaleX, float nodeParentScaleY, float parentRotation)
{
    auto scaleX = nodeParentScaleX * node->getScaleX();
    auto scaleY = nodeParentScaleY * node->getScaleY();
    auto rotation = parentRotation + node->getRotation();

    auto nodeToWorldTransform = parentToWorldTransform * node->getNodeToParentTransform();

    auto physicsBody = node->getPhysicsBody();
    if (physicsBody)
    {
        physicsBody->beforeSimulation(parentToWorldTransform, nodeToWorldTransform, scaleX, scaleY, rotation);
    }

    for (auto & child : node->getChildren())
        beforeSimulation(child.get(), nodeToWorldTransform, scaleX, scaleY, rotation);
}

void PhysicsWorld::afterSimulation(Node *node, const Mat4& parentToWorldTransform, float parentRotation)
{
    auto nodeToWorldTransform = parentToWorldTransform * node->getNodeToParentTransform();
    auto nodeRotation = parentRotation + node->getRotation();

    auto physicsBody = node->getPhysicsBody();
    if (physicsBody)
    {
        physicsBody->afterSimulation(parentToWorldTransform, parentRotation);
    }

    for (auto & child : node->getChildren())
        afterSimulation(child.get(), nodeToWorldTransform, nodeRotation);
}

} // namespace cocos2d

#endif // CC_USE_PHYSICS
