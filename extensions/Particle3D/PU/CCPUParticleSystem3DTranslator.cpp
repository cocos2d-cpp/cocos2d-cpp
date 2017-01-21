/****************************************************************************
 Copyright (C) 2013 Henry van Merode. All rights reserved.
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
 ****************************************************************************/

#include "CCPUParticleSystem3DTranslator.h"

namespace cocos2d {

PUParticleSystem3DTranslator::PUParticleSystem3DTranslator()
{
    
}
PUParticleSystem3DTranslator::~PUParticleSystem3DTranslator()
{
    
}

void PUParticleSystem3DTranslator::translate(PUScriptCompiler* compiler, PUAbstractNode *node)
{
    if (typeid(*node) != typeid(PUObjectAbstractNode))
    {
        return;
    }

    PUObjectAbstractNode* obj = reinterpret_cast<PUObjectAbstractNode*>(node);
    if(obj->name.empty())
    {
        return;
    }

    obj->context = _system;
    _system->setName(obj->name);
    for(PUAbstractNodeList::iterator i = obj->children.begin(); i != obj->children.end(); ++i)
    {
        if((*i)->type == ANT_PROPERTY)
        {
            PUPropertyAbstractNode* prop = reinterpret_cast<PUPropertyAbstractNode*>((*i));

            if (prop->name == token[TOKEN_PS_SCALE])
            {
                // Property: scale
                if (passValidateProperty(compiler, prop, token[TOKEN_PS_SCALE], VAL_VECTOR3))
                {
                    Vec3 val;
                    if(getVector3(prop->values.begin(), prop->values.end(), &val))
                    {
                        _system->setScaleX(val.x);
                        _system->setScaleY(val.y);
                        _system->setScaleZ(val.z);
                    }
                }
            }
            else if (prop->name == token[TOKEN_PS_SCALE_VELOCITY])
            {
                // Property: scale_velocity
                if (passValidateProperty(compiler, prop, token[TOKEN_PS_ITERATION_INTERVAL], VAL_REAL))
                {
                    float val = 0.0f;
                    if(getFloat(*prop->values.front(), &val))
                    {
                        _system->setParticleSystemScaleVelocity(val);
                    }
                }
            }
            else if (prop->name == token[TOKEN_KEEP_LOCAL])
            {
                // Property: keep_local
                if (passValidateProperty(compiler, prop, token[TOKEN_KEEP_LOCAL], VAL_BOOL))
                {
                    bool val;
                    if(getBoolean(*prop->values.front(), &val))
                    {
                        _system->setKeepLocal(val);
                    }
                }
            }
        }
        else if((*i)->type == ANT_OBJECT)
        {
            processNode(compiler, *i);
        }
        else
        {
            errorUnexpectedToken(compiler, *i);
        }
    }
}



} // namespace cocos2d
