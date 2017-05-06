/****************************************************************************
 Copyright (c) 2017 Iakov Sergeev <yahont@github>
 Copyright (c) 2015 Chukong Technologies Inc.
 
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

#include "navmesh/CCNavMeshObstacle.h"
#if CC_USE_NAVMESH

#include "navmesh/CCNavMesh.h"
#include "2d/CCNode.h"
#include "2d/CCScene.h"
#include "recast/DetourTileCache/DetourTileCache.h"

namespace cocos2d {

const std::string NavMeshObstacle::NAV_MESH_OBSTACLE_COMPONENT_NAME = "___NavMeshObstacleComponent___";

NavMeshObstacle::NavMeshObstacle(float radius, float height)
    : Component(NAV_MESH_OBSTACLE_COMPONENT_NAME)
    , _radius(radius)
    , _height(height)
    , _syncFlag(NODE_AND_NODE)
    , _obstacleID(-1)
    , _tileCache(nullptr)
{
}

void cocos2d::NavMeshObstacle::removeFrom(dtTileCache* /*tileCache*/)
{
    _tileCache->removeObstacle(_obstacleID);
    _tileCache = nullptr;
    _obstacleID = -1;
}

void cocos2d::NavMeshObstacle::addTo(dtTileCache *tileCache)
{
    _tileCache = tileCache;
    Mat4 mat = _owner->getNodeToWorldTransform();
    _tileCache->addObstacle(&mat.m[12], _radius, _height, &_obstacleID);
}

void cocos2d::NavMeshObstacle::onExit()
{
    if (_obstacleID == static_cast<dtObstacleRef>(-1)) return;
    Component::onExit();
    auto scene = _owner->getScene();
    if (scene && scene->getNavMesh()){
        scene->getNavMesh()->removeNavMeshObstacle(this);
    }
}

void cocos2d::NavMeshObstacle::onEnter()
{
    if (_obstacleID != static_cast<dtObstacleRef>(-1)) return;
    Component::onEnter();
    auto scene = _owner->getScene();
    if (scene && scene->getNavMesh()){
        scene->getNavMesh()->addNavMeshObstacle(this);
    }
}

void cocos2d::NavMeshObstacle::postUpdate(float /*delta*/)
{
    if ((_syncFlag & OBSTACLE_TO_NODE) != 0)
        syncToNode();
}

void cocos2d::NavMeshObstacle::preUpdate(float /*delta*/)
{
    if ((_syncFlag & NODE_TO_OBSTACLE) != 0)
        syncToObstacle();
}

void NavMeshObstacle::syncToNode()
{
    if (_tileCache){
        auto obstacle = _tileCache->getObstacleByRef(_obstacleID);
        if (obstacle){
            Vec3 localPos = Vec3(obstacle->pos[0], obstacle->pos[1], obstacle->pos[2]);
            if (_owner->getParent())
                _owner->getParent()->getWorldToNodeTransform().transformPoint(localPos, &localPos);
            _owner->setPosition3D(localPos);
            _radius = obstacle->radius;
            _height = obstacle->height;
        }
    }
}

void cocos2d::NavMeshObstacle::setRadius(float radius)
{
    _radius = radius;
}

void cocos2d::NavMeshObstacle::setHeight(float height)
{
    _height = height;
}

void NavMeshObstacle::syncToObstacle()
{
    if (_tileCache){
        auto obstacle = _tileCache->getObstacleByRef(_obstacleID);
        if (obstacle){
            Vec3 worldPos = Vec3(obstacle->pos[0], obstacle->pos[1], obstacle->pos[2]);
            Mat4 mat = _owner->getNodeToWorldTransform();
            if ((mat.m[12] != obstacle->pos[0] && mat.m[13] != obstacle->pos[1] && mat.m[14] != obstacle->pos[2])
                || obstacle->radius != _radius
                || obstacle->height != _height){
                _tileCache->removeObstacle(_obstacleID);
                _tileCache->addObstacle(&mat.m[12], _radius, _height, &_obstacleID);
            }
        }
    }
}

} // namespace cocos2d

#endif //CC_USE_NAVMESH
