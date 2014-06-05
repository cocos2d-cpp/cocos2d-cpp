

#include "LoadingBarReader.h"
#include "ui/UILoadingBar.h"
#include "cocostudio/CocoLoader.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

namespace cocostudio
{
    static LoadingBarReader* instanceLoadingBar = NULL;
    
    IMPLEMENT_CLASS_WIDGET_READER_INFO(LoadingBarReader)
    
    LoadingBarReader::LoadingBarReader()
    {
        
    }
    
    LoadingBarReader::~LoadingBarReader()
    {
        
    }
    
    LoadingBarReader* LoadingBarReader::getInstance()
    {
        if (!instanceLoadingBar)
        {
            instanceLoadingBar = new LoadingBarReader();
        }
        return instanceLoadingBar;
    }
    
    void LoadingBarReader::setPropsFromBinary(cocos2d::ui::Widget *widget, CocoLoader *pCocoLoader, stExpCocoNode *pCocoNode)
    {
        WidgetReader::setPropsFromBinary(widget, pCocoLoader, pCocoNode);
        
        LoadingBar* loadingBar = static_cast<LoadingBar*>(widget);
        
        float capsx = 0.0f, capsy = 0.0, capsWidth = 0.0, capsHeight = 0.0f;
        
        stExpCocoNode *stChildArray = pCocoNode->GetChildArray();
        
        for (int i = 0; i < pCocoNode->GetChildNum(); ++i) {
            std::string key = stChildArray[i].GetName(pCocoLoader);
            std::string value = stChildArray[i].GetValue();
            
            if (key == "scale9Enable") {
                loadingBar->setScale9Enabled(valueToBool(value));
            }
            else if (key == "textureData"){
                
                stExpCocoNode *backGroundChildren = stChildArray[i].GetChildArray();
                std::string resType = backGroundChildren[2].GetValue();;
                
                Widget::TextureResType imageFileNameType = (Widget::TextureResType)valueToInt(resType);
                
                std::string backgroundValue = this->getResourcePath(pCocoLoader, &stChildArray[i], imageFileNameType);
                
                loadingBar->loadTexture(backgroundValue, imageFileNameType);
                
            }
            else if(key == "capInsetsX"){
                capsx = valueToFloat(value);
            }else if(key == "capInsetsY"){
                capsy = valueToFloat(value);
            }else if(key == "capInsetsWidth"){
                capsWidth = valueToFloat(value);
            }else if(key == "capInsetsHeight"){
                capsHeight = valueToFloat(value);
            }else if(key == "direction"){
                loadingBar->setDirection((LoadingBar::Direction)valueToInt(value));
            }else if(key == "percent"){
                loadingBar->setPercent(valueToInt(value));
            }
            
        } //end of for loop
        
        if (loadingBar->isScale9Enabled()) {
            loadingBar->setCapInsets(Rect(capsx, capsy, capsWidth, capsHeight));
        }
    }
    
    void LoadingBarReader::setPropsFromJsonDictionary(Widget *widget, const rapidjson::Value &options)
    {
        WidgetReader::setPropsFromJsonDictionary(widget, options);
        
        
        LoadingBar* loadingBar = static_cast<LoadingBar*>(widget);
        
        const rapidjson::Value& imageFileNameDic = DICTOOL->getSubDictionary_json(options, "textureData");
        int imageFileNameType = DICTOOL->getIntValue_json(imageFileNameDic, "resourceType");
        std::string imageFileName = this->getResourcePath(imageFileNameDic, "path", (Widget::TextureResType)imageFileNameType);
        loadingBar->loadTexture(imageFileName, (Widget::TextureResType)imageFileNameType);

        
        /* gui mark add load bar scale9 parse */
        bool scale9Enable = DICTOOL->getBooleanValue_json(options, "scale9Enable");
        loadingBar->setScale9Enabled(scale9Enable);
        
        if (scale9Enable)
        {
            float cx = DICTOOL->getFloatValue_json(options, "capInsetsX");
            float cy = DICTOOL->getFloatValue_json(options, "capInsetsY");
            float cw = DICTOOL->getFloatValue_json(options, "capInsetsWidth");
            float ch = DICTOOL->getFloatValue_json(options, "capInsetsHeight");
            
            loadingBar->setCapInsets(Rect(cx, cy, cw, ch));
            
            float width = DICTOOL->getFloatValue_json(options, "width");
            float height = DICTOOL->getFloatValue_json(options, "height");
            loadingBar->setSize(Size(width, height));
        }
        /**/
        
        loadingBar->setDirection(LoadingBar::Direction(DICTOOL->getIntValue_json(options, "direction")));
        loadingBar->setPercent(DICTOOL->getIntValue_json(options, "percent"));
        
        
        WidgetReader::setColorPropsFromJsonDictionary(widget, options);
    }
}
