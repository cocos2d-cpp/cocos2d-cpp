/****************************************************************************
 Copyright (c) 2015 Chukong Technologies Inc.

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

 Ideas taken from:
 - GamePlay3D: http://gameplay3d.org/
 - OGRE3D: http://www.ogre3d.org/
 - Qt3D: http://qt-project.org/
 ****************************************************************************/

#include "renderer/CCTechnique.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCMaterial.h"
#include "renderer/CCPass.h"

namespace cocos2d {

Technique* Technique::createWithGLProgramState(Material* parent, GLProgramState* state)
{
    auto technique = new (std::nothrow) Technique();
    if (technique && technique->init(parent))
    {
        auto pass = Pass::createWithGLProgramState(technique, state);
        technique->addPass(pass);

        technique->autorelease();
        return technique;
    }
    return  nullptr;
}

Technique* Technique::create(Material* material)
{
    auto technique = new (std::nothrow) Technique();
    if (technique && technique->init(material))
    {
        technique->autorelease();
        return technique;
    }
    return nullptr;
}

Technique::Technique()
: _name("")
{
}

Technique::~Technique()
{
}

bool Technique::init(Material* parent)
{
    _parent = parent;
    return true;
}

Technique* Technique::clone() const
{
    auto technique = new (std::nothrow) Technique();

    if (technique)
    {
        technique->_name = _name;
        RenderState::cloneInto(technique);

        for (auto & pass: _passes)
        {
            auto p = to_retaining_ptr( pass->clone() );
            p->_parent = technique;
            technique->_passes.push_back(std::move( p ));
        }

        technique->autorelease();
    }
    return technique;
}

void Technique::addPass(Pass *pass)
{
    _passes.push_back( to_retaining_ptr( pass ));
}

std::string Technique::getName() const
{
    return _name;
}

void Technique::setName(const std::string &name)
{
    _name = name;
}

Pass* Technique::getPassByIndex(size_t index) const
{
    CC_ASSERT(index < _passes.size() && "Invalid index");
    return _passes.at(index).get();
}

ssize_t Technique::getPassCount() const
{
    return _passes.size();
}

const std::vector<retaining_ptr<Pass>> & Technique::getPasses() const
{
    return _passes;
}

} // namespace cocos2d
