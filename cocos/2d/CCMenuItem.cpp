/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2016 Chukong Technologies Inc.
Copyright (c) 2017      Iakov Sergeev <yahont@github>

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

#include "2d/CCMenuItem.h"
#include "2d/CCActionInterval.h"
#include "2d/CCSprite.h"
#include "2d/CCLabelAtlas.h"
#include "2d/CCLabel.h"
#include "base/CCDirector.h"
#include "base/ccUTF8.h"
#include <stdarg.h>

namespace cocos2d {
    
static int _globalFontSize = kItemSize;
static std::string _globalFontName = "Marker Felt";
static bool _globalFontNameRelease = false;

const unsigned int    kZoomActionTag = 0xc0c05002;

//
// MenuItem
//

MenuItem* MenuItem::create()
{
    return MenuItem::create(ccMenuCallback());
}

MenuItem* MenuItem::create( const ccMenuCallback& callback)
{
    MenuItem *ret = new (std::nothrow) MenuItem();
    ret->initWithCallback(callback);
    ret->autorelease();
    return ret;
}

bool MenuItem::initWithCallback(const ccMenuCallback& callback)
{
    setAnchorPoint(Vec2(0.5f, 0.5f));
    _callback = callback;
    _enabled = true;
    _selected = false;
    return true;
}

MenuItem::~MenuItem()
{
}

void MenuItem::selected()
{
    _selected = true;
}

void MenuItem::unselected()
{
    _selected = false;
}

void MenuItem::activate()
{
    if (_enabled)
    {
        if( _callback )
        {
            _callback(this);
        }
    }
}

void MenuItem::setEnabled(bool enabled)
{
    _enabled = enabled;
}

bool MenuItem::isEnabled() const
{
    return _enabled;
}

Rect MenuItem::rect() const
{
    return Rect( _position.x - _contentSize.width * _anchorPoint.x,
                      _position.y - _contentSize.height * _anchorPoint.y,
                      _contentSize.width, _contentSize.height);
}

bool MenuItem::isSelected() const
{
    return _selected;
}

void MenuItem::setCallback(const ccMenuCallback& callback)
{
    _callback = callback;
}

std::string MenuItem::getDescription() const
{
    return StringUtils::format("<MenuItem | tag = %d>", _tag);
}

//
//CCMenuItemLabel
//

void MenuItemLabel::setLabel(node_ptr<Node> label)
{
    assert(nullptr != dynamic_cast<LabelProtocol*>(label.get()));

    removeChild(_labelId, true);

    if (label)
    {
        label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        setContentSize(label->getContentSize()); 
        _labelId = addChild( std::move(label) );
    }
}

MenuItemLabel * MenuItemLabel::create(node_ptr<Node> label, const ccMenuCallback& callback)
{
    MenuItemLabel *ret = new (std::nothrow) MenuItemLabel();
    ret->initWithLabel(std::move(label), callback);
    ret->autorelease();
    return ret;
}

MenuItemLabel* MenuItemLabel::create(node_ptr<Node> label)
{
    MenuItemLabel *ret = new (std::nothrow) MenuItemLabel();
    ret->initWithLabel(std::move(label), ccMenuCallback());
    ret->autorelease();
    return ret;
}

bool MenuItemLabel::initWithLabel(node_ptr<Node> label, const ccMenuCallback& callback)
{
    MenuItem::initWithCallback(callback);
    _originalScale = 1.0f;
    _colorBackup = Color3B::WHITE;
    setDisabledColor(Color3B(126,126,126));
    setLabel( std::move(label) );

    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);

    return true;
}


MenuItemLabel::~MenuItemLabel()
{
}

void MenuItemLabel::setString(const std::string& text)
{
    auto node = Director::getInstance()->getNodeRegister().getNode(_labelId);

    if (auto label = dynamic_cast<LabelProtocol*>(node))
    {
        label->setString(text);
        setContentSize(node->getContentSize());
    }
}

std::string MenuItemLabel::getString() const
{
    auto node = Director::getInstance()->getNodeRegister().getNode(_labelId);

    if (auto label = dynamic_cast<LabelProtocol*>(node))
    {
        return label->getString();
    }

    return "";
}

void MenuItemLabel::activate()
{
    if(_enabled)
    {
        stopAllActions();
        setScale( _originalScale );
        MenuItem::activate();
    }
}

void MenuItemLabel::selected()
{
    // subclass to change the default action
    if(_enabled)
    {
        MenuItem::selected();
        
        
        if (! stopActionByTag(kZoomActionTag))
        {
            _originalScale = this->getScale();
        }
        
        auto zoomAction = std::make_unique<ScaleTo>(0.1f, _originalScale * 1.2f);
        zoomAction->setTag(kZoomActionTag);
        this->runAction( std::move( zoomAction));
    }
}

void MenuItemLabel::unselected()
{
    // subclass to change the default action
    if(_enabled)
    {
        MenuItem::unselected();
        this->stopActionByTag(kZoomActionTag);
        auto zoomAction = std::make_unique<ScaleTo>(0.1f, _originalScale);
        zoomAction->setTag(kZoomActionTag);
        this->runAction( std::move( zoomAction));
    }
}

void MenuItemLabel::setEnabled(bool enabled)
{
    if( _enabled != enabled ) 
    {
        if(enabled == false)
        {
            _colorBackup = this->getColor();
            this->setColor(_disabledColor);
        }
        else
        {
            this->setColor(_colorBackup);
        }
    }
    MenuItem::setEnabled(enabled);
}

//
//CCMenuItemAtlasFont
//

MenuItemAtlasFont * MenuItemAtlasFont::create(const std::string& value, const std::string& charMapFile, int itemWidth, int itemHeight, char startCharMap)
{
    return MenuItemAtlasFont::create(value, charMapFile, itemWidth, itemHeight, startCharMap, ccMenuCallback());
}

MenuItemAtlasFont * MenuItemAtlasFont::create(const std::string& value, const std::string& charMapFile, int itemWidth, int itemHeight, char startCharMap, const ccMenuCallback& callback)
{
    MenuItemAtlasFont *ret = new (std::nothrow) MenuItemAtlasFont();
    ret->initWithString(value, charMapFile, itemWidth, itemHeight, startCharMap, callback);
    ret->autorelease();
    return ret;
}

bool MenuItemAtlasFont::initWithString(const std::string& value, const std::string& charMapFile, int itemWidth, int itemHeight, char startCharMap, const ccMenuCallback& callback)
{
    CCASSERT( value.size() != 0, "value length must be greater than 0");
    auto label = make_node_ptr<LabelAtlas>();
    label->initWithString(value, charMapFile, itemWidth, itemHeight, startCharMap);
    if (MenuItemLabel::initWithLabel( std::move(label), callback))
    {
        // do something ?
    }
    return true;
}

//
//CCMenuItemFont
//

void MenuItemFont::setFontSize(int s)
{
    _globalFontSize = s;
}

int MenuItemFont::getFontSize()
{
    return _globalFontSize;
}

void MenuItemFont::setFontName(const std::string& name)
{
    if (_globalFontNameRelease)
    {
        _globalFontName.clear();
    }
    _globalFontName = name;
    _globalFontNameRelease = true;
}

const std::string& MenuItemFont::getFontName()
{
    return _globalFontName;
}

MenuItemFont * MenuItemFont::create(const std::string& value, const ccMenuCallback& callback)
{
    MenuItemFont *ret = new (std::nothrow) MenuItemFont();
    ret->initWithString(value, callback);
    ret->autorelease();
    return ret;
}


MenuItemFont * MenuItemFont::create(const std::string& value)
{
    MenuItemFont *ret = new (std::nothrow) MenuItemFont();
    ret->initWithString(value, ccMenuCallback());
    ret->autorelease();
    return ret;
}

MenuItemFont::MenuItemFont()
 : _fontSize(0), _fontName("")
{}

MenuItemFont::~MenuItemFont()
{
    CCLOGINFO("In the destructor of MenuItemFont (%p).", this);
}

bool MenuItemFont::initWithString(const std::string& value, const ccMenuCallback& callback)
{
    CCASSERT( !value.empty(), "Value length must be greater than 0");

    _fontName = _globalFontName;
    _fontSize = _globalFontSize;

    auto label = to_node_ptr(Label::createWithSystemFont(value, _fontName, _fontSize));
    if (MenuItemLabel::initWithLabel(std::move(label), callback))
    {
        // do something ?
    }
    return true;
}

void MenuItemFont::setFontSizeObj(int s)
{
    _fontSize = s;

    if (auto label = Director::getInstance()->getNodeRegister().get<Label>(_labelId))
    {
        label->setSystemFontSize(_fontSize);
        setContentSize(label->getContentSize());
    }
}

int MenuItemFont::getFontSizeObj() const
{
    return _fontSize;
}

void MenuItemFont::setFontNameObj(const std::string& name)
{
    _fontName = name;

    if (auto label = Director::getInstance()->getNodeRegister().get<Label>(_labelId))
    {
        label->setSystemFontName(_fontName);
        setContentSize(label->getContentSize());
    }
}

const std::string& MenuItemFont::getFontNameObj() const
{
    return _fontName;
}

//
//CCMenuItemSprite
//

void MenuItemSprite::setNormalImage(node_ptr<Node> image)
{
    removeChild(_normalImageId, true);

    if (image)
    {
        image->setAnchorPoint(Vec2(0, 0));
        setContentSize(image->getContentSize());
        _normalImageId = addChild( std::move(image) );
    }

    updateImagesVisibility();
}

void MenuItemSprite::setSelectedImage(node_ptr<Node> image)
{
    removeChild(_selectedImageId, true);

    if (image)
    {
        image->setAnchorPoint(Vec2(0, 0));
        _selectedImageId = addChild( std::move(image) );
    }

    updateImagesVisibility();
}

void MenuItemSprite::setDisabledImage(node_ptr<Node> image)
{
    removeChild(_disabledImageId, true);

    if (image)
    {
        image->setAnchorPoint(Vec2(0, 0));
        _disabledImageId = addChild( std::move(image) );
    }

    updateImagesVisibility();
}

MenuItemSprite * MenuItemSprite::create(node_ptr<Node> normalSprite, node_ptr<Node> selectedSprite, node_ptr<Node> disabledSprite)
{
    return MenuItemSprite::create(std::move(normalSprite),
                                  std::move(selectedSprite),
                                  std::move(disabledSprite),
                                  ccMenuCallback());
}

MenuItemSprite * MenuItemSprite::create(node_ptr<Node> normalSprite, node_ptr<Node> selectedSprite)
{
    return MenuItemSprite::create(std::move(normalSprite),
                                  std::move(selectedSprite),
                                  node_ptr<Node>(),
                                  ccMenuCallback());
}

MenuItemSprite * MenuItemSprite::create(node_ptr<Node> normalSprite, node_ptr<Node> selectedSprite, const ccMenuCallback& callback)
{
    return MenuItemSprite::create(std::move(normalSprite),
                                  std::move(selectedSprite),
                                  node_ptr<Node>(),
                                  callback);
}

MenuItemSprite * MenuItemSprite::create(node_ptr<Node> normalSprite, node_ptr<Node> selectedSprite, node_ptr<Node> disabledSprite, const ccMenuCallback& callback)
{
    MenuItemSprite *ret = new (std::nothrow) MenuItemSprite();
    ret->initWithNormalSprite(std::move(normalSprite),
                              std::move(selectedSprite),
                              std::move(disabledSprite),
                              callback);
    ret->autorelease();
    return ret;
}

bool MenuItemSprite::initWithNormalSprite(node_ptr<Node> normalSprite, node_ptr<Node> selectedSprite, node_ptr<Node> disabledSprite, const ccMenuCallback& callback)
{
    MenuItem::initWithCallback(callback);

    setNormalImage( std::move(normalSprite) );
    setSelectedImage( std::move(selectedSprite) );
    setDisabledImage( std::move(disabledSprite) );

    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);

    return true;
}

/**
 @since v0.99.5
 */
void MenuItemSprite::selected()
{
    MenuItem::selected();

    auto& nodes = Director::getInstance()->getNodeRegister();

    if (auto disabledImage = nodes.getNode(_disabledImageId))
    {
        disabledImage->setVisible(false);
    }

    auto selectedImage = nodes.getNode(_selectedImageId);

    if (selectedImage != nullptr)
    {
        selectedImage->setVisible(true);
    }

    if (auto normalImage = nodes.getNode(_normalImageId))
    {
        normalImage->setVisible(selectedImage == nullptr);
    }
}

void MenuItemSprite::unselected()
{
    MenuItem::unselected();
    this->updateImagesVisibility();
}

void MenuItemSprite::setEnabled(bool bEnabled)
{
    if( _enabled != bEnabled ) 
    {
        MenuItem::setEnabled(bEnabled);
        this->updateImagesVisibility();
    }
}

// Helper 
void MenuItemSprite::updateImagesVisibility()
{
    auto& nodes = Director::getInstance()->getNodeRegister();

    auto disabledImage = nodes.getNode(_disabledImageId);

    if (auto normalImage = nodes.getNode(_normalImageId))
        normalImage->setVisible(_enabled || (nullptr == disabledImage));

    if (auto selectedImage = nodes.getNode(_selectedImageId))
        selectedImage->setVisible(false);

    if (nullptr != disabledImage)
        disabledImage->setVisible(!_enabled);
}

///
/// MenuItemImage
///

MenuItemImage* MenuItemImage::create()
{
    MenuItemImage *ret = new (std::nothrow) MenuItemImage();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

MenuItemImage * MenuItemImage::create(const std::string& normalImage, const std::string& selectedImage)
{
    return MenuItemImage::create(normalImage, selectedImage, "", ccMenuCallback());
}

MenuItemImage * MenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, const ccMenuCallback& callback)
{
    return MenuItemImage::create(normalImage, selectedImage, "", callback);
}

MenuItemImage * MenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
    MenuItemImage *ret = new (std::nothrow) MenuItemImage();
    if (ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, callback))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

MenuItemImage * MenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage)
{
    MenuItemImage *ret = new (std::nothrow) MenuItemImage();
    if (ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, ccMenuCallback()))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MenuItemImage::initWithNormalImage(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
    node_ptr<Sprite> normalSprite;
    node_ptr<Sprite> selectedSprite;
    node_ptr<Sprite> disabledSprite;

    if (! normalImage.empty())
    {
        normalSprite = make_node_ptr<Sprite>(normalImage);
    }

    if (! selectedImage.empty())
    {
        selectedSprite = make_node_ptr<Sprite>(selectedImage);
    }

    if (! disabledImage.size())
    {
        disabledSprite = make_node_ptr<Sprite>(disabledImage);
    }

    return initWithNormalSprite(std::move(normalSprite), std::move(selectedSprite), std::move(disabledSprite), callback);
}

//
// Setter of sprite frames
//
void MenuItemImage::setNormalSpriteFrame(SpriteFrame * frame)
{
    setNormalImage( make_node_ptr<Sprite>(frame) );
}

void MenuItemImage::setSelectedSpriteFrame(SpriteFrame * frame)
{
    setSelectedImage( make_node_ptr<Sprite>(frame));
}

void MenuItemImage::setDisabledSpriteFrame(SpriteFrame * frame)
{
    setDisabledImage( make_node_ptr<Sprite>(frame));
}

//
// MenuItemToggle
//

MenuItemToggle * MenuItemToggle::createWithCallback(const ccMenuCallback & callback,
                                                    items_container && menuItems)
{
    MenuItemToggle *ret = new (std::nothrow) MenuItemToggle();
    ret->MenuItem::initWithCallback(callback);
    ret->autorelease();
    ret->_subItems = std::move(menuItems);
    ret->_selectedIndex = UINT_MAX;
    ret->setSelectedIndex(0);
    return ret;
}

MenuItemToggle * MenuItemToggle::create()
{
    MenuItemToggle *ret = new (std::nothrow) MenuItemToggle();
    ret->initWithItem(nullptr);
    ret->autorelease();
    return ret;
}

MenuItemToggle* MenuItemToggle::create(MenuItem *item)
{
    MenuItemToggle *ret = new (std::nothrow) MenuItemToggle();
    ret->initWithItem(item);
    ret->autorelease();
    return ret;
}

bool MenuItemToggle::initWithItem(MenuItem *item)
{
    MenuItem::initWithCallback(ccMenuCallback());

    if (item)
    {
        addSubItem(item);
    }
    _selectedIndex = UINT_MAX;
    this->setSelectedIndex(0);
    
    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);
    
    return true;
}

void MenuItemToggle::addSubItem(MenuItem *item)
{
    _subItems.push_back(to_node_ptr(item));
}

void MenuItemToggle::cleanup()
{
    for(const auto & item : _subItems) {
#if defined(CC_NATIVE_CONTROL_SCRIPT) && !CC_NATIVE_CONTROL_SCRIPT
        ScriptEngineManager::getInstance()->getScriptEngine()->releaseScriptObject(this, item);
#endif
        item->cleanup();
    }
    MenuItem::cleanup();
}

void MenuItemToggle::setSelectedIndex(unsigned int index)
{
    if( index != _selectedIndex && _subItems.size() > 0 )
    {
        _selectedIndex = index;
        if (_selectedItem)
        {
            _selectedItem->removeFromParentAndCleanup(false);
        }

        _selectedItem = _subItems.at(_selectedIndex).get();
        this->addChild( to_node_ptr(_selectedItem) );
        Size s = _selectedItem->getContentSize();
        this->setContentSize(s);
        _selectedItem->setPosition(s.width / 2, s.height / 2);
    }
}

void MenuItemToggle::selected()
{
    MenuItem::selected();
    _subItems.at(_selectedIndex)->selected();
}

void MenuItemToggle::unselected()
{
    MenuItem::unselected();
    _subItems.at(_selectedIndex)->unselected();
}

void MenuItemToggle::activate()
{
    // update index
    if( _enabled ) 
    {
        unsigned int newIndex = (_selectedIndex + 1) % _subItems.size();
        this->setSelectedIndex(newIndex);
    }
    MenuItem::activate();
}
void MenuItemToggle::setEnabled(bool enabled)
{
    if (_enabled != enabled)
    {
        MenuItem::setEnabled(enabled);

        for(const auto & item : _subItems) {
            item->setEnabled(enabled);
        }
    }
}

MenuItem* MenuItemToggle::getSelectedItem()
{
    return _subItems.at(_selectedIndex).get();
}

} // namespace cocos2d
