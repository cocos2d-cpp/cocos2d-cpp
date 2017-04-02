/*
 * Copyright (C) 2009 Matt Oswald
 * Copyright (c) 2009-2010 Ricardo Quesada
 * Copyright (c) 2010-2012 cocos2d-x.org
 * Copyright (c) 2011      Zynga Inc.
 * Copyright (c) 2011      Marco Tillemans
 * Copyright (c) 2013-2016 Chukong Technologies Inc.
 * Copyright (c) 2017      Iakov Sergeev <yahont@github>
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

#include "2d/CCParticleBatchNode.h"
#include "2d/CCGrid.h"
#include "2d/CCParticleSystem.h"
#include "renderer/CCTextureCache.h"
#include "renderer/CCQuadCommand.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCTextureAtlas.h"
#include "base/CCProfiling.h"
#include "base/ccUTF8.h"

namespace cocos2d {

ParticleBatchNode::ParticleBatchNode()
: _textureAtlas(nullptr)
{

}

ParticleBatchNode::~ParticleBatchNode()
{
    CC_SAFE_RELEASE(_textureAtlas);
}
/*
 * creation with Texture2D
 */

ParticleBatchNode* ParticleBatchNode::createWithTexture(const Texture2D *tex, int capacity/* = kParticleDefaultCapacity*/)
{
    ParticleBatchNode * p = new (std::nothrow) ParticleBatchNode();
    if( p && p->initWithTexture(tex, capacity))
    {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

/*
 * creation with File Image
 */

ParticleBatchNode* ParticleBatchNode::create(const std::string& imageFile, int capacity/* = kParticleDefaultCapacity*/)
{
    ParticleBatchNode * p = new (std::nothrow) ParticleBatchNode();
    if( p && p->initWithFile(imageFile, capacity))
    {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

/*
 * init with Texture2D
 */
bool ParticleBatchNode::initWithTexture(const Texture2D *tex, int capacity)
{
    _textureAtlas = new (std::nothrow) TextureAtlas();
    _textureAtlas->initWithTexture(tex, capacity);

    getChildren().reserve(capacity);
    
    _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;

    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR, tex));

    return true;
}

/*
 * init with FileImage
 */
bool ParticleBatchNode::initWithFile(const std::string& fileImage, int capacity)
{
    const Texture2D *tex = Director::getInstance()->getTextureCache()->addImage(fileImage);
    return initWithTexture(tex, capacity);
}

// ParticleBatchNode - composition

// override visit.
// Don't call visit on it's children
void ParticleBatchNode::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    // CAREFUL:
    // This visit is almost identical to Node#visit
    // with the exception that it doesn't call visit on it's children
    //
    // The alternative is to have a void Sprite#visit, but
    // although this is less maintainable, is faster
    //
    if (!_visible)
    {
        return;
    }

    uint32_t flags = processParentFlags(parentTransform, parentFlags);

    if (isVisitableByVisitingCamera())
    {
        // IMPORTANT:
        // To ease the migration to v3.0, we still support the Mat4 stack,
        // but it is deprecated and your code should not rely on it
        Director* director = Director::getInstance();
        director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
        director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
        
        draw(renderer, _modelViewTransform, flags);
        
        director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    }
}

// override addChild:
void ParticleBatchNode::addChild(node_ptr<Node> child, int zOrder, int tag)
{
    CCASSERT(child, "Argument must be non-nullptr");
    auto particleSystem = node_ptr<ParticleSystem>(dynamic_cast<ParticleSystem*>(child.release()));
    CCASSERT(particleSystem, "CCParticleBatchNode only supports QuadParticleSystems as children");
    CCASSERT(particleSystem->getTexture()->getName() == _textureAtlas->getTexture()->getName(), "CCParticleSystem is not using the same texture id");
    
    addChildByTagOrName(std::move(particleSystem), zOrder, tag, "", true);
}

void ParticleBatchNode::addChild(node_ptr<Node> child, int zOrder, const std::string &name)
{
    CCASSERT(child, "Argument must be non-nullptr");
    auto particleSystem = node_ptr<ParticleSystem>(dynamic_cast<ParticleSystem*>(child.release()));
    CCASSERT(particleSystem, "CCParticleBatchNode only supports QuadParticleSystems as children");
    CCASSERT(particleSystem->getTexture()->getName() == _textureAtlas->getTexture()->getName(), "CCParticleSystem is not using the same texture id");
   
    addChildByTagOrName(std::move(particleSystem), zOrder, 0, name, false);
}

void ParticleBatchNode::addChildByTagOrName(node_ptr<ParticleSystem> child, int zOrder, int tag, const std::string &name, bool setTag)
{
    // If this is the 1st children, then copy blending function
    if (getChildren().empty())
    {
        setBlendFunc(child->getBlendFunc());
    }
    
    CCASSERT( _blendFunc.src  == child->getBlendFunc().src && _blendFunc.dst  == child->getBlendFunc().dst, "Can't add a ParticleSystem that uses a different blending function");
    
    //no lazy sorting, so don't call super addChild, call helper instead
    int pos = 0;
    auto child_ptr = child.get();

    if (setTag)
        pos = addChildHelper(std::move(child), zOrder, tag, "", true);
    else
        pos = addChildHelper(std::move(child), zOrder, 0, name, false);
    
    //get new atlasIndex
    int atlasIndex = 0;
    
    if (pos != 0)
    {
        ParticleSystem* p = static_cast<ParticleSystem*>(getChildren().at(pos-1).get());
        atlasIndex = p->getAtlasIndex() + p->getTotalParticles();
    }
    else
    {
        atlasIndex = 0;
    }
    
    insertChild(child_ptr, atlasIndex);
    
    // update quad info
    child_ptr->setBatchNode(this);
}

// don't use lazy sorting, reordering the particle systems quads afterwards would be too complex
// FIXME: research whether lazy sorting + freeing current quads and calloc a new block with size of capacity would be faster
// FIXME: or possibly using vertexZ for reordering, that would be fastest
// this helper is almost equivalent to Node's addChild, but doesn't make use of the lazy sorting
int ParticleBatchNode::addChildHelper(node_ptr<ParticleSystem> child, int z, int aTag, const std::string &name, bool setTag)
{
    CCASSERT( child, "Argument must be non-nil");
    CCASSERT( child->getParent() == nullptr, "child already added. It can't be added again");

    //don't use a lazy insert
    auto pos = searchNewPositionInChildrenForZ(z);

    auto child_ptr = child.get();

    getChildren().insert(std::begin(getChildren()) + pos, std::move(child));

    if (setTag)
        child_ptr->setTag(aTag);
    else
        child_ptr->setName(name);
    
    child_ptr->setLocalZOrder(z);

    child_ptr->setParent(this);

    if( _running )
    {
        child_ptr->onEnter();
        child_ptr->onEnterTransitionDidFinish();
    }

    return pos;
}

// Reorder will be done in this function, no "lazy" reorder to particles
void ParticleBatchNode::reorderChild(Node * aChild, int zOrder)
{
    CCASSERT( aChild != nullptr, "Child must be non-nullptr");
    CCASSERT( dynamic_cast<ParticleSystem*>(aChild) != nullptr, "CCParticleBatchNode only supports QuadParticleSystems as children");
    CCASSERT(getChildren().end()
             != std::find_if(getChildren().begin(), getChildren().end(),
                             [aChild](const Node::children_container::value_type & c) {
                                 return c.get() == aChild;
                             }),
             "Child doesn't belong to batch" );

    ParticleSystem* child = static_cast<ParticleSystem*>(aChild);

    if( zOrder == child->getLocalZOrder() )
    {
        return;
    }

    // no reordering if only 1 child
    if (!getChildren().empty())
    {
        int newIndex = 0, oldIndex = 0;

        getCurrentIndex(&oldIndex, &newIndex, child, zOrder);

        if( oldIndex != newIndex )
        {

            // reorder getChildren()->array
            auto tmp = std::move(getChildren().at(oldIndex));
            getChildren().erase(getChildren().cbegin() + oldIndex);
            getChildren().insert(getChildren().cbegin() + newIndex, std::move(tmp));

            // save old altasIndex
            int oldAtlasIndex = child->getAtlasIndex();

            // update atlas index
            updateAllAtlasIndexes();

            // Find new AtlasIndex
            int newAtlasIndex = 0;
            for (int i=0, size = getChildren().size(); i < size; ++i)
            {
                ParticleSystem* node = static_cast<ParticleSystem*>(getChildren().at(i).get());
                if( node == child )
                {
                    newAtlasIndex = child->getAtlasIndex();
                    break;
                }
            }

            // reorder textureAtlas quads
            _textureAtlas->moveQuadsFromIndex(oldAtlasIndex, child->getTotalParticles(), newAtlasIndex);

            child->updateWithNoTime();
        }
    }

    child->setLocalZOrder(zOrder);
}

void ParticleBatchNode::getCurrentIndex(int* oldIndex, int* newIndex, Node* child, int z)
{
    bool foundCurrentIdx = false;
    bool foundNewIdx = false;

    int  minusOne = 0;
    auto count = getChildren().size();

    for (size_t i=0; i < count; i++)
    {
        Node* pNode = getChildren().at(i).get();

        // new index
        if( pNode->getLocalZOrder() > z &&  ! foundNewIdx )
        {
            *newIndex = i;
            foundNewIdx = true;

            if( foundCurrentIdx && foundNewIdx )
            {
                break;
            }
        }

        // current index
        if( child == pNode )
        {
            *oldIndex = i;
            foundCurrentIdx = true;

            if( ! foundNewIdx )
            {
                minusOne = -1;
            }

            if( foundCurrentIdx && foundNewIdx )
            {
                break;
            }
        }

    }

    if( ! foundNewIdx )
    {
        *newIndex = static_cast<int>(count);
    }

    *newIndex += minusOne;
}

int ParticleBatchNode::searchNewPositionInChildrenForZ(int z)
{
    auto count = getChildren().size();

    for (size_t i=0; i < count; i++)
    {
        Node *child = getChildren().at(i).get();
        if (child->getLocalZOrder() > z)
        {
            return i;
        }
    }
    return static_cast<int>(count);
}

// override removeChild:
void  ParticleBatchNode::removeChild(NodeId id, bool cleanup)
{
    auto child = Director::getInstance()->getNodeRegister().get<ParticleSystem>(id);

    CCASSERT(child != nullptr, "CCParticleBatchNode only supports QuadParticleSystems as children");
    CCASSERT(getChildren().end()
             != std::find_if(getChildren().begin(), getChildren().end(),
                             [child](const Node::children_container::value_type & c) {
                                 return c.get() == child;
                             }),
             "CCParticleBatchNode doesn't contain the sprite. Can't remove it");

    // remove child helper
    _textureAtlas->removeQuadsAtIndex(child->getAtlasIndex(), child->getTotalParticles());

    // after memmove of data, empty the quads at the end of array
    _textureAtlas->fillWithEmptyQuadsFromIndex(_textureAtlas->getTotalQuads(), child->getTotalParticles());

    // particle could be reused for self rendering
    child->setBatchNode(nullptr);
    Node::removeChild(id, cleanup);

    updateAllAtlasIndexes();
}

void ParticleBatchNode::removeChildAtIndex(int index, bool doCleanup)
{
    removeChild(getChildren().at(index)->getNodeId(), doCleanup);
}

void ParticleBatchNode::removeAllChildrenWithCleanup(bool doCleanup)
{
    for(const auto &child : getChildren())
        static_cast<ParticleSystem*>(child.get())->setBatchNode(nullptr);

    Node::removeAllChildrenWithCleanup(doCleanup);

    _textureAtlas->removeAllQuads();
}

void ParticleBatchNode::draw(Renderer* renderer, const Mat4 & /*transform*/, uint32_t flags)
{
    CC_PROFILER_START("CCParticleBatchNode - draw");

    if( _textureAtlas->getTotalQuads() == 0 )
    {
        return;
    }
    _batchCommand.init(_globalZOrder, getGLProgram(), _blendFunc, _textureAtlas, _modelViewTransform, flags);
    renderer->addCommand(&_batchCommand);
    CC_PROFILER_STOP("CCParticleBatchNode - draw");
}



void ParticleBatchNode::increaseAtlasCapacityTo(ssize_t quantity)
{
    CCLOG("cocos2d: ParticleBatchNode: resizing TextureAtlas capacity from [%lu] to [%lu].",
          (long)_textureAtlas->getCapacity(),
          (long)quantity);

    if( ! _textureAtlas->resizeCapacity(quantity) ) {
        // serious problems
        CCLOGWARN("cocos2d: WARNING: Not enough memory to resize the atlas");
        CCASSERT(false,"XXX: ParticleBatchNode #increaseAtlasCapacity SHALL handle this assert");
    }
}

//sets a 0'd quad into the quads array
void ParticleBatchNode::disableParticle(int particleIndex)
{
    V3F_C4B_T2F_Quad* quad = &((_textureAtlas->getQuads())[particleIndex]);
    quad->br.vertices.x = quad->br.vertices.y = quad->tr.vertices.x = quad->tr.vertices.y = quad->tl.vertices.x = quad->tl.vertices.y = quad->bl.vertices.x = quad->bl.vertices.y = 0.0f;
}

// ParticleBatchNode - add / remove / reorder helper methods

// add child helper
void ParticleBatchNode::insertChild(ParticleSystem* system, int index)
{
    system->setAtlasIndex(index);

    if(_textureAtlas->getTotalQuads() + system->getTotalParticles() > _textureAtlas->getCapacity())
    {
        increaseAtlasCapacityTo(_textureAtlas->getTotalQuads() + system->getTotalParticles());

        // after a realloc empty quads of textureAtlas can be filled with gibberish (realloc doesn't perform calloc), insert empty quads to prevent it
        _textureAtlas->fillWithEmptyQuadsFromIndex(_textureAtlas->getCapacity() - system->getTotalParticles(), system->getTotalParticles());
    }

    // make room for quads, not necessary for last child
    if (system->getAtlasIndex() + system->getTotalParticles() != _textureAtlas->getTotalQuads())
    {
        _textureAtlas->moveQuadsFromIndex(index, index + system->getTotalParticles());
    }

    // increase totalParticles here for new particles, update method of particle-system will fill the quads
    _textureAtlas->increaseTotalQuadsWith(system->getTotalParticles());

    updateAllAtlasIndexes();
}

//rebuild atlas indexes
void ParticleBatchNode::updateAllAtlasIndexes()
{
    int index = 0;
    
    for(const auto & child : getChildren())
    {
        ParticleSystem* partiSys = static_cast<ParticleSystem*>(child.get());
        partiSys->setAtlasIndex(index);
        index += partiSys->getTotalParticles();
    }
}

// ParticleBatchNode - CocosNodeTexture protocol

void ParticleBatchNode::updateBlendFunc()
{
    if( ! _textureAtlas->getTexture()->hasPremultipliedAlpha())
        _blendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
}

void ParticleBatchNode::setTexture(const Texture2D* texture)
{
    _textureAtlas->setTexture(texture);

    // If the new texture has No premultiplied alpha, AND the blendFunc hasn't been changed, then update it
    if( texture
        && ! texture->hasPremultipliedAlpha()
        && ( _blendFunc.src == GL_ONE
             && _blendFunc.dst == GL_ONE_MINUS_SRC_ALPHA ) )
    {
        _blendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
    }
}

const Texture2D* ParticleBatchNode::getTexture() const
{
    return _textureAtlas->getTexture();
}

void ParticleBatchNode::setBlendFunc(const BlendFunc &blendFunc)
{
    _blendFunc = blendFunc;
}
// returns the blending function used for the texture
const BlendFunc& ParticleBatchNode::getBlendFunc() const
{
    return _blendFunc;
}

} // namespace cocos2d
