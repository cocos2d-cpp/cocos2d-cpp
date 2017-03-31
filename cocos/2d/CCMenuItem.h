/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
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

#ifndef __CCMENU_ITEM_H__
#define __CCMENU_ITEM_H__

// C++ includes
#include <functional>

// cocos2d includes
#include "2d/CCNode.h"
#include "base/CCProtocols.h"

namespace cocos2d {

typedef std::function<void(Ref*)> ccMenuCallback;

class Label;
class LabelAtlas;
class Sprite;
class SpriteFrame;
#define kItemSize 32
    
/**
 * @addtogroup _2d
 * @{
 */

/** @brief MenuItem base class.
 *
 *  Subclass MenuItem (or any subclass) to create your custom MenuItem objects.
 */
class CC_DLL MenuItem : public Node
{
public:
    /** Creates a MenuItem with no target/selector. */
    static MenuItem* create();
    /** Creates a MenuItem with a target/selector. */
    static MenuItem* create(const ccMenuCallback& callback);

    /** Returns the outside box. */
    Rect rect() const;
    /** Activate the item. */
    virtual void activate();
    /** The item was selected (not activated), similar to "mouse-over". */
    virtual void selected();
    /** The item was unselected. */
    virtual void unselected();
    /** Returns whether or not the item is enabled. */
    virtual bool isEnabled() const;
    /** Enables or disables the item. */
    virtual void setEnabled(bool value);
    /** Returns whether or not the item is selected. */
    virtual bool isSelected() const;

    /** Set the callback to the menu item.
    * @code
    * In js,can contain two params,the second param is jsptr.
    * @endcode
    * @lua NA
    */
    void setCallback(const ccMenuCallback& callback);
    
    /**
     * @js NA
     */
    virtual std::string getDescription() const override;
    
protected:
    /**
     * @js ctor
     */
    MenuItem()
    : _selected(false)
    , _enabled(false)
	, _callback(nullptr)
    {}
    /**
     * @js NA
     * @lua NA
     */
    virtual ~MenuItem();
    
    /** Initializes a MenuItem with a target/selector.
     * @lua NA
     */
    bool initWithCallback(const ccMenuCallback& callback);

protected:
    bool            _selected;
    bool            _enabled;
    // callback
    ccMenuCallback _callback;

private:
    MenuItem(const MenuItem &) = delete;
    const MenuItem & operator=(const MenuItem &) = delete;
};

/** @brief An abstract class for "label" MenuItemLabel items.
 Any Node that supports the LabelProtocol protocol can be added.
 Supported nodes:
 - BitmapFontAtlas
 - LabelAtlas
 - Label
 */
class CC_DLL MenuItemLabel : public MenuItem
{
public:
    /** Creates a MenuItemLabel with a Label and a callback. */
    CC_DEPRECATED_ATTRIBUTE static MenuItemLabel * create(Node* label, const ccMenuCallback& callback)  { return MenuItemLabel::create(to_node_ptr(label), callback); }
    static MenuItemLabel * create(node_ptr<Node> label, const ccMenuCallback& callback);

    /** Creates a MenuItemLabel with a Label. Target and selector will be nil. */
    CC_DEPRECATED_ATTRIBUTE static MenuItemLabel* create(Node *label) { return MenuItemLabel::create(to_node_ptr(label)); }
    static MenuItemLabel* create(node_ptr<Node> label);

    /** Sets a new string to the inner label. */
    void setString(const std::string& label);

    /** Get the inner string of the inner label. */
    std::string getString() const;

    /** Gets the color that will be used when the item is disabled. */
    const Color3B& getDisabledColor() const { return _disabledColor; }

    /** Sets the color that will be used when the item is disabled. */
    void setDisabledColor(const Color3B& color) { _disabledColor = color; }
    
    /** Gets the label that is rendered. */
    Node* getLabel() const { return _label.get(); }
    
    /** Sets the label that is rendered. */
    void setLabel(node_ptr<Node> node);
    
    // Overrides
    virtual void activate() override;
    virtual void selected() override;
    virtual void unselected() override;
    virtual void setEnabled(bool enabled) override;
    
protected:
    /**
     * @js ctor
     */
    MenuItemLabel()
    : _originalScale(0.0)
    , _label()
    {}
    /**
     * @js NA
     * @lua NA
     */
    virtual ~MenuItemLabel();
    
    /** Initializes a MenuItemLabel with a Label, target and selector. */
    bool initWithLabel(node_ptr<Node> label, const ccMenuCallback& callback);
    
protected:
    Color3B    _colorBackup;
    float      _originalScale;

    /** The color that will be used to disable the item. */
    Color3B _disabledColor;
    /** Label that is rendered. It can be any Node that implements the LabelProtocol. */
    node_ptr<Node> _label;

private:
    MenuItemLabel(const MenuItemLabel &) = delete;
    const MenuItemLabel & operator=(const MenuItemLabel &) = delete;
};


/** @brief A MenuItemAtlasFont.
 Helper class that creates a MenuItemLabel class with a LabelAtlas.
 */
class CC_DLL MenuItemAtlasFont : public MenuItemLabel
{
public:
    /** Creates a menu item from a string and atlas with a target/selector. */
    static MenuItemAtlasFont* create(const std::string& value, const std::string& charMapFile, int itemWidth, int itemHeight, char startCharMap);
    /** Creates a menu item from a string and atlas. Use it with MenuItemToggle. */
    static MenuItemAtlasFont* create(const std::string& value, const std::string& charMapFile, int itemWidth, int itemHeight, char startCharMap, const ccMenuCallback& callback);
    
protected:
    /**
     * @js ctor
     */
    MenuItemAtlasFont(){}
    /**
     * @js NA
     * @lua NA
     */
    virtual ~MenuItemAtlasFont(){}
    
    /** Initializes a menu item from a string and atlas with a target/selector. */
    bool initWithString(const std::string& value, const std::string& charMapFile, int itemWidth, int itemHeight, char startCharMap, const ccMenuCallback& callback);

private:
    MenuItemAtlasFont(const MenuItemAtlasFont &) = delete;
    const MenuItemAtlasFont & operator=(const MenuItemAtlasFont &) = delete;
};


/** @brief A MenuItemFont.
 Helper class that creates a MenuItemLabel class with a Label.
 */
class CC_DLL MenuItemFont : public MenuItemLabel
{
public:
    /** Creates a menu item from a string without target/selector. To be used with MenuItemToggle. */
    static MenuItemFont * create(const std::string& value = "");
    /** Creates a menu item from a string with a target/selector. */
    static MenuItemFont * create(const std::string& value, const ccMenuCallback& callback);

    /** Set default font size. */
    static void setFontSize(int size);
    /** Get default font size. */
    static int getFontSize();
    /** Set the default font name. */
    static void setFontName(const std::string& name);
    /** Get the default font name. */
    static const std::string& getFontName();

    /** Set font size.
     * c++ can not overload static and non-static member functions with the same parameter types.
     * so change the name to setFontSizeObj.
     * @js setFontSize
     * @js NA
     */
    void setFontSizeObj(int size);
    
    /** get font size .
     * @js getFontSize
     * @js NA
     */
    int getFontSizeObj() const;
    
    /**
     * Set the font name .
     * c++ can not overload static and non-static member functions with the same parameter types.
     * so change the name to setFontNameObj.
     * @js setFontName
     * @js NA
     */
    void setFontNameObj(const std::string& name);

    /** Returns the name of the Font.
     * @js getFontNameObj
     * @js NA
     */
    const std::string& getFontNameObj() const;

protected:
    /**
     * @js ctor
     */
    MenuItemFont();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~MenuItemFont();
    
    /** Initializes a menu item from a string with a target/selector. */
    bool initWithString(const std::string& value, const ccMenuCallback& callback);
    
protected:
    int _fontSize;
    std::string _fontName;

private:
    MenuItemFont(const MenuItemFont &) = delete;
    const MenuItemFont & operator=(const MenuItemFont &) = delete;
};


/** @brief MenuItemSprite accepts Node<RGBAProtocol> objects as items.
 The images has 3 different states:
 - unselected image
 - selected image
 - disabled image
 
 @since v0.8.0
 */
class CC_DLL MenuItemSprite : public MenuItem
{
public:
    /** Creates a menu item with a normal, selected and disabled image.*/
    static MenuItemSprite * create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite = nullptr);
    /** Creates a menu item with a normal and selected image with a callable object. */
    static MenuItemSprite * create(Node* normalSprite, Node* selectedSprite, const ccMenuCallback& callback);
    /** Creates a menu item with a normal,selected  and disabled image with target/selector. */
    static MenuItemSprite * create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite, const ccMenuCallback& callback);

    /** Gets the image used when the item is not selected. */
    Node* getNormalImage() const { return _normalImage; }
    
    /** Sets the image used when the item is not selected. */
    void setNormalImage(Node* image);
    
    /** Gets the image used when the item is selected. */
    Node* getSelectedImage() const { return _selectedImage; }
    
    /** Sets the image used when the item is selected. */
    void setSelectedImage(Node* image);
    
    /** Gets the image used when the item is disabled. */
    Node* getDisabledImage() const { return _disabledImage; }
    
    /** Sets the image used when the item is disabled. */
    void setDisabledImage(Node* image);
    
    /**
     * The item was selected (not activated), similar to "mouse-over".
     @since v0.99.5
     */
    virtual void selected();

    /** The item was unselected. */
    virtual void unselected();

    /** Enables or disables the item. */
    virtual void setEnabled(bool bEnabled);
    
protected:
    MenuItemSprite()
    :_normalImage(nullptr)
    ,_selectedImage(nullptr)
    ,_disabledImage(nullptr)
    {}
    
    /** Initializes a menu item with a normal, selected and disabled image with a callable object. */
    bool initWithNormalSprite(Node* normalSprite, Node* selectedSprite, Node* disabledSprite, const ccMenuCallback& callback);
    
protected:
    virtual void updateImagesVisibility();

    /** The image used when the item is not selected. */
    Node* _normalImage;
    /** The image used when the item is selected. */
    Node* _selectedImage;
    /** The image used when the item is disabled. */
    Node* _disabledImage;

private:
    MenuItemSprite(const MenuItemSprite &) = delete;
    const MenuItemSprite & operator=(const MenuItemSprite &) = delete;
};


/** @brief MenuItemImage accepts images as items.
 The images has 3 different states:
 - unselected image
 - selected image
 - disabled image
 
 For best results try that all images are of the same size.
 */
class CC_DLL MenuItemImage : public MenuItemSprite
{
public:
    /** Creates an MenuItemImage. */
    static MenuItemImage* create();
    /** Creates a menu item with a normal and selected image.*/
    static MenuItemImage* create(const std::string& normalImage, const std::string& selectedImage);
    /** Creates a menu item with a normal,selected  and disabled image.*/
    static MenuItemImage* create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage);
    /** Creates a menu item with a normal and selected image with a callable object. */
    static MenuItemImage* create(const std::string&normalImage, const std::string&selectedImage, const ccMenuCallback& callback);

    /** Creates a menu item with a normal,selected and disabled image with a callable object. */
    static MenuItemImage* create(const std::string&normalImage, const std::string&selectedImage, const std::string&disabledImage, const ccMenuCallback& callback);

    /** Sets the sprite frame for the normal image. */
    void setNormalSpriteFrame(SpriteFrame* frame);
    /** Sets the sprite frame for the selected image. */
    void setSelectedSpriteFrame(SpriteFrame* frame);
    /** Sets the sprite frame for the disabled image. */
    void setDisabledSpriteFrame(SpriteFrame* frame);
    
protected:
    /**
     * @js ctor
     */
    MenuItemImage(){}
    /**
     * @js NA
     * @lua NA
     */
    virtual ~MenuItemImage(){}
    
    bool init();
    
    /** Initializes a menu item with a normal, selected and disabled image with a callable object. */
    bool initWithNormalImage(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback);

private:
    MenuItemImage(const MenuItemImage &) = delete;
    const MenuItemImage & operator=(const MenuItemImage &) = delete;
};


/** @brief A MenuItemToggle.
 A simple container class that "toggles" it's inner items.
 The inner items can be any MenuItem.
 */
class CC_DLL MenuItemToggle : public MenuItem
{
public:
    using items_container = std::vector<node_ptr<MenuItem>>;
    /**
     *@brief Creates a menu item from a Array with a callable object.
     */
    static MenuItemToggle * createWithCallback(const ccMenuCallback & callback,
                                               items_container && menuItems);

    template<typename ...Items>
    static MenuItemToggle * createWithCallback(const ccMenuCallback & callback,
                                               items_container && menuItems,
                                               items_container::value_type && item,
                                               Items && ...items)
    {
        menuItems.push_back(std::move(item));
        return createWithCallback(callback, std::move(menuItems), std::forward<Items>(items)...);
    }
    template<typename ...Items>
    static MenuItemToggle * createWithCallback(const ccMenuCallback & callback,
                                               items_container::value_type && item,
                                               Items && ...items)
    {
        items_container menuItems;
        menuItems.push_back(std::move(item));
        return createWithCallback(callback, std::move(menuItems), std::forward<Items>(items)...);
    }

    /** Creates a menu item with no target/selector and no items. */
    static MenuItemToggle* create();
    
    /** Creates a menu item with a item. */
    static MenuItemToggle* create(MenuItem *item);
    
    /** Add more menu item. */
    void addSubItem(MenuItem *item);
    
    /** Return the selected item. */
    MenuItem* getSelectedItem();
    
    /** Gets the index of the selected item. */
    unsigned int getSelectedIndex() const { return _selectedIndex; }
    
    /** Sets the index of the selected item. */
    void setSelectedIndex(unsigned int index);
    
    // Overrides
    virtual void activate() override;
    virtual void selected() override;
    virtual void unselected() override;
    virtual void setEnabled(bool var) override;
    virtual void cleanup() override;
    
protected:
    /**
     * @js ctor
     */
    MenuItemToggle()
    : _selectedIndex(0)
    , _selectedItem(nullptr)
    {}
    
    /** Initializes a menu item with a item. */
    bool initWithItem(MenuItem *item);

protected:
    /** Returns the selected item. */
    unsigned int _selectedIndex;
    MenuItem* _selectedItem;

private:
    /** Array that contains the subitems. You can add/remove items in runtime, and you can replace the array with a new one.
     @since v0.7.2
     */
    items_container _subItems;

    MenuItemToggle(const MenuItemToggle &) = delete;
    const MenuItemToggle & operator=(const MenuItemToggle &) = delete;
};


// end of 2d group
/// @}

} // namespace cocos2d

#endif //__CCMENU_ITEM_H__
