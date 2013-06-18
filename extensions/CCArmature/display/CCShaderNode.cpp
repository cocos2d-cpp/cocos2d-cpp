/****************************************************************************
Copyright (c) 2013 cocos2d-x.org

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

#include "CCShaderNode.h"

NS_CC_EXT_BEGIN

enum
{
    SIZE_X = 128,
    SIZE_Y = 128,
};

CCShaderNode::CCShaderNode()
    : _center(vertex2(0.0f, 0.0f))
    , _resolution(vertex2(0.0f, 0.0f))
    , _time(0.0f)
    , _uniformCenter(0)
    , _uniformResolution(0)
    , _uniformTime(0)
{
}

CCShaderNode *CCShaderNode::shaderNodeWithVertex(const char *vert, const char *frag)
{
    CCShaderNode *node = new CCShaderNode();
    node->initWithVertex(vert, frag);
    node->autorelease();

    return node;
}

bool CCShaderNode::initWithVertex(const char *vert, const char *frag)
{

    loadShaderVertex(vert, frag);

    _time = 0;
    _resolution = vertex2(SIZE_X, SIZE_Y);

    scheduleUpdate();

    setContentSize(CCSizeMake(SIZE_X, SIZE_Y));
    setAnchorPoint(ccp(0.5f, 0.5f));

    return true;
}

void CCShaderNode::loadShaderVertex(const char *vert, const char *frag)
{
    CCGLProgram *shader = new CCGLProgram();
    shader->initWithVertexShaderFilename(vert, frag);

    shader->addAttribute("aVertex", kCCVertexAttrib_Position);
    shader->link();

    shader->updateUniforms();

    _uniformCenter = glGetUniformLocation(shader->getProgram(), "center");
    _uniformResolution = glGetUniformLocation(shader->getProgram(), "resolution");
    _uniformTime = glGetUniformLocation(shader->getProgram(), "time");

    this->setShaderProgram(shader);

    shader->release();
}

void CCShaderNode::update(float dt)
{
    _time += dt;
}

void CCShaderNode::translateFormOtherNode(CCAffineTransform &transform)
{
    CCNode::setAdditionalTransform(transform);

    _center = vertex2(_additionalTransform.tx * CC_CONTENT_SCALE_FACTOR(), _additionalTransform.ty * CC_CONTENT_SCALE_FACTOR());
    _resolution = vertex2( SIZE_X * _additionalTransform.a, SIZE_Y * _additionalTransform.d);
}

void CCShaderNode::setPosition(const CCPoint &newPosition)
{
    CCNode::setPosition(newPosition);
    CCPoint position = getPosition();
    _center = vertex2(position.x * CC_CONTENT_SCALE_FACTOR(), position.y * CC_CONTENT_SCALE_FACTOR());
}

void CCShaderNode::draw()
{
    CC_NODE_DRAW_SETUP();

    float w = SIZE_X, h = SIZE_Y;
    GLfloat vertices[12] = {0, 0, w, 0, w, h, 0, 0, 0, h, w, h};

    //
    // Uniforms
    //
    getShaderProgram()->setUniformLocationWith2f(_uniformCenter, _center.x, _center.y);
    getShaderProgram()->setUniformLocationWith2f(_uniformResolution, _resolution.x, _resolution.y);


    // time changes all the time, so it is Ok to call OpenGL directly, and not the "cached" version
    glUniform1f(_uniformTime, _time);

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    CC_INCREMENT_GL_DRAWS(1);
}


NS_CC_EXT_END
