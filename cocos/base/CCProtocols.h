/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2016 Chukong Technologies Inc.

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

#ifndef __BASE_CCPROTOCOLS_H__
#define __BASE_CCPROTOCOLS_H__
/// @cond DO_NOT_SHOW

#include <string>

#include "base/ccTypes.h"
#include "renderer/CCTexture2D.h"

namespace cocos2d {

/**
 * Common interface for Labels
 */
class CC_DLL LabelProtocol
{
public:
    virtual ~LabelProtocol() {}

    /**
     * Sets a new label using a string
     *
     * @param label A null terminated string
     * @js NA
     * @lua NA
     */
    virtual void setString(const std::string &label) = 0;

    /** 
     * Returns the string that is currently being used in this label 
     *
     * @return The string that is currently being used in this label
     * @js NA
     * @lua NA
     */
    virtual const std::string& getString() const = 0;
};

} // namespace cocos2d

/// @endcond
#endif // __BASE_CCPROTOCOLS_H__
