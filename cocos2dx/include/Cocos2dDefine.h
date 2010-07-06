/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

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

#ifndef __COCOS2D_DEFINE_H__
#define __COCOS2D_DEFINE_H__

#define GET_VARIABLE(varType, varName)\
	public inline varType get##varName() { return varName; };

#define SET_VARIABLE(varType, varName)\
	public inline void set##varName(varType funVar) { varName = funVar; };

#define VAR_PROPERTY_READONLY(varType, varName)\
	protected varType m_##varName;\
	GET_VARIABLE(varType, varName)

#define VAR_PROPERTY_READWRITE(varType, varName)\
	protected varType m_##varName;\
	GET_VARIABLE(varType, varName)\
	SET_VARIABLE(varTYPE, varName)

#endif // __COCOS2D_DEFINE_H__