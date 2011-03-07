/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

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
#ifndef __CCLABEL_ATLAS_H__
#define __CCLABEL_ATLAS_H__
#include "CCAtlasNode.h"
namespace cocos2d{

	/** @brief CCLabelAtlas is a subclass of CCAtlasNode.

	It can be as a replacement of CCLabel since it is MUCH faster.

	CCLabelAtlas versus CCLabel:
	- CCLabelAtlas is MUCH faster than CCLabel
	- CCLabelAtlas "characters" have a fixed height and width
	- CCLabelAtlas "characters" can be anything you want since they are taken from an image file

	A more flexible class is CCBitmapFontAtlas. It supports variable width characters and it also has a nice editor.
	*/
	class CC_DLL CCLabelAtlas : public CCAtlasNode, public CCLabelProtocol
	{
	public:
		CCLabelAtlas()
			:m_sString("")
		{}
		virtual ~CCLabelAtlas()
		{ 
			m_sString.clear(); 
		}
		/** creates the CCLabelAtlas with a string, a char map file(the atlas), the width and height of each element and the starting char of the atlas */
		static CCLabelAtlas * labelWithString(const char *label, const char *charMapFile, int itemWidth, int itemHeight, char startCharMap);


        /** creates the CCLabelAtlas with a string, a char map file(the atlas), the width and height of each element in points and the starting char of the atlas.
        @deprecated Will be removed in 1.0.1. Use "labelWithString:" instead
        */
        static CCLabelAtlas * labelAtlasWithString(const char *label, const char *charMapFile, int itemWidth, int itemHeight, char startCharMap);

		/** initializes the CCLabelAtlas with a string, a char map file(the atlas), the width and height of each element and the starting char of the atlas */
		bool initWithString(const char *label, const char *charMapFile, int itemWidth, int itemHeight, char startCharMap);
		// super methods
		virtual void updateAtlasValues();
		virtual void setString(const char *label);
		virtual const char* getString(void);
		virtual void draw();		

		virtual CCLabelProtocol* convertToLabelProtocol() { return (CCLabelProtocol*)this; }
	protected:
		// string to render
		std::string m_sString;
		// the first char in the charmap
		char m_cMapStartChar;
	};
}// namespace cocos2d

#endif //__CCLABEL_ATLAS_H__
