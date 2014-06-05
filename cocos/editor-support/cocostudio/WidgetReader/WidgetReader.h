/****************************************************************************
 Copyright (c) 2014 cocos2d-x.org
 
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

#ifndef __TestCpp__WidgetReader__
#define __TestCpp__WidgetReader__

#include "WidgetReaderProtocol.h"
#include "../CCSGUIReader.h"
#include "ui/GUIDefine.h"
#include "ui/UIWidget.h"



namespace cocostudio
{
    class CocoLoader;
    class stExpCocoNode;
    
    class WidgetReader : public cocos2d::Ref, public WidgetReaderProtocol
    {
    public:
        DECLARE_CLASS_WIDGET_READER_INFO
        
        WidgetReader();
        virtual ~WidgetReader();
        
        static WidgetReader* getInstance();
        static void purge();
        
        virtual void setPropsFromJsonDictionary(cocos2d::ui::Widget* widget,
                                                const rapidjson::Value& options);
        
        virtual void setColorPropsFromJsonDictionary(cocos2d::ui::Widget* widget,
                                                     const rapidjson::Value& options);
        
        virtual void setBasicPropsFromBinary(cocos2d::ui::Widget* widget, CocoLoader* pCocoLoader,  stExpCocoNode*	pCocoNode);
        virtual void setColorPropsFromBinary(cocos2d::ui::Widget* widget, CocoLoader* pCocoLoader,  stExpCocoNode*	pCocoNode);
        
        virtual void setPropsFromBinary(cocos2d::ui::Widget* widget, CocoLoader* pCocoLoader,  stExpCocoNode*	pCocoNode)
        {
            this->setBasicPropsFromBinary(widget, pCocoLoader, pCocoNode);
            this->setColorPropsFromBinary(widget, pCocoLoader, pCocoNode);
        };
    protected:
        std::string getResourcePath(const rapidjson::Value& dict,
                                    const std::string& key,
                                    cocos2d::ui::Widget::TextureResType texType);
        std::string getResourcePath(CocoLoader* pCocoLoader,  stExpCocoNode*	pCocoNode, cocos2d::ui::Widget::TextureResType texType);
        void setAnchorPointForWidget(cocos2d::ui::Widget* widget, const rapidjson::Value&options);
        
        std::function<int(std::string)> valueToInt;
        std::function<bool(std::string)> valueToBool;
        std::function<float(std::string)> valueToFloat;

    };
}

#endif /* defined(__TestCpp__WidgetReader__) */
