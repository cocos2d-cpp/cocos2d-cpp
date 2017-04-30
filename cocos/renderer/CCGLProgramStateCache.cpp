/****************************************************************************
Copyright 2013-2016 Chukong Technologies Inc.

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
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN false EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "renderer/CCGLProgramStateCache.h"

#include "renderer/CCGLProgramState.h"
#include "renderer/CCGLProgram.h"


namespace cocos2d {

GLProgramStateCache* GLProgramStateCache::s_instance = nullptr;

GLProgramStateCache::GLProgramStateCache()
{
}

GLProgramStateCache::~GLProgramStateCache()
{
    _glProgramStates.clear();
}

GLProgramStateCache* GLProgramStateCache::getInstance()
{
    if (s_instance == nullptr)
        s_instance = new (std::nothrow) GLProgramStateCache();
    
    return s_instance;
}

void GLProgramStateCache::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}

GLProgramState* GLProgramStateCache::getGLProgramState(GLProgram* glprogram)
{
    GLProgramState* ret = nullptr;

    auto it = _glProgramStates.find(glprogram);

    if (it != _glProgramStates.end())
    {
        ret = it->second.get();
    }
    else
    {
        auto n = to_retaining_ptr(new (std::nothrow) GLProgramState);
        if (n->init(glprogram))
        {
            ret = n.get();
            _glProgramStates[glprogram] = std::move( n );
        }
    }

    return ret;
}

} // namespace cocos2d
