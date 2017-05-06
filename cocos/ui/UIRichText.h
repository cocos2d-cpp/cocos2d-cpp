/****************************************************************************
 Copyright (c) 2017 Iakov Sergeev <yahont@github>
 Copyright (c) 2013 cocos2d-x.org
 
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

#ifndef __UIRICHTEXT_H__
#define __UIRICHTEXT_H__

#include "ui/UIWidget.h"
#include "ui/GUIExport.h"
#include "base/CCValue.h"

namespace cocos2d {

class Label;

namespace ui {

/**
 *@brief Rich text element base class.
 * It defines the basic common properties for all rich text element.
 */
class CC_GUI_DLL RichElement
{
    friend class RichText;

public:
    enum class Type
    {
        TEXT,
        IMAGE,
        CUSTOM,
        NEWLINE
    };
    
    RichElement(Type type, int tag, Color3B const& color, GLubyte opacity)
        : _type(type)
        , _tag(tag)
        , _color(color)
        , _opacity(opacity)
    {}

    virtual ~RichElement();

    bool equalType(Type type)
    {
        return type == _type;
    }

    void setColor(Color3B const& color)
    {
        _color = color;
    }

protected:
    Type    _type;
    int     _tag;
    Color3B _color;
    GLubyte _opacity;
};
    
class CC_GUI_DLL RichElementText : public RichElement
{
    friend class RichText;

public:

    enum Flags {
        ITALICS_FLAG       = 1 << 0,
        BOLD_FLAG          = 1 << 1,
        UNDERLINE_FLAG     = 1 << 2,
        STRIKETHROUGH_FLAG = 1 << 3,
        URL_FLAG           = 1 << 4,
        OUTLINE_FLAG       = 1 << 5,
        SHADOW_FLAG        = 1 << 6,
        GLOW_FLAG          = 1 << 7
    };
    
    RichElementText(int tag,
                    const Color3B& color,
                    GLubyte opacity,
                    const std::string& text,
                    const std::string& fontName,
                    float fontSize,
                    uint32_t flags,
                    const std::string& url,
                    const Color3B& outlineColor = Color3B::WHITE,
                    int outlineSize = -1,
                    const Color3B& shadowColor = Color3B::BLACK,
                    const cocos2d::Size& shadowOffset = Size(2.0, -2.0),
                    int shadowBlurRadius = 0,
                    const Color3B& glowColor = Color3B::WHITE)
        : RichElement(Type::TEXT, tag, color, opacity)
        , _text( text)
        , _fontName( fontName )
        , _fontSize( fontSize )
        , _flags( flags )
        , _url( url )
        , _outlineColor( outlineColor )
        , _outlineSize( outlineSize )
        , _shadowColor( shadowColor )
        , _shadowOffset( shadowOffset )
        , _shadowBlurRadius( shadowBlurRadius )
        , _glowColor( glowColor )
    {}

protected:
    std::string   _text;
    std::string   _fontName;
    float         _fontSize;
    uint32_t      _flags;
    std::string   _url;
    Color3B       _outlineColor;
    int           _outlineSize;
    Color3B       _shadowColor;
    cocos2d::Size _shadowOffset;
    int           _shadowBlurRadius;
    Color3B       _glowColor;
};
    
class CC_GUI_DLL RichElementImage : public RichElement
{
    friend class RichText;

public:
    
    RichElementImage(int tag, const Color3B& color, GLubyte opacity, const std::string& filePath, const std::string& url = "")
        : RichElement(Type::IMAGE, tag, color, opacity)
        , _filePath(filePath)
        , _width(-1)
        , _height(-1)
        , _url(url)
    {}

    void setWidth(int width)            { _width  = width;  }
    void setHeight(int height)          { _height = height; }
    void setUrl(const std::string& url) { _url    = url;    }

protected:
    std::string _filePath;
    int         _width;
    int         _height;
    std::string _url;   /*!< attributes of anchor tag */
};
    
class CC_GUI_DLL RichElementCustomNode : public RichElement
{
    friend class RichText;

public:

    RichElementCustomNode(int tag, const Color3B& color, GLubyte opacity, node_ptr<Node> customNode)
        : RichElement(Type::CUSTOM, tag, color, opacity)
        , _customNode( std::move(customNode) )
    {}

protected:

    node_ptr<Node> _customNode;
};
    
class CC_GUI_DLL RichElementNewLine : public RichElement
{
    friend class RichText;

public:
    
    RichElementNewLine(int tag, const Color3B& color, GLubyte opacity)
        : RichElement(Type::NEWLINE, tag, color, opacity)
    {}
};

/**
 *@brief A container for displaying various RichElements.
 * We could use it to display texts with images easily.
 */
class CC_GUI_DLL RichText : public Widget
{
public:

    enum WrapMode {
        WRAP_PER_WORD,
        WRAP_PER_CHAR
    };
    
    /**
     * @brief call to open a resource specified by a URL
     * @param url a URL
     */
    typedef std::function<void(const std::string& url)> OpenUrlHandler;
    
    /**
     * @brief called on the specified tag
     * @param tagAttrValueMap the attributes of a tag
     * @result text attributes and RichElement
     */
    typedef std::function<std::pair<ValueMap, std::unique_ptr<RichElement>>(const ValueMap& tagAttrValueMap)> VisitEnterHandler;
    
    static const std::string KEY_VERTICAL_SPACE;                    /*!< key of vertical space */
    static const std::string KEY_WRAP_MODE;                         /*!< key of per word, or per char */
    static const std::string KEY_FONT_COLOR_STRING;                 /*!< key of font color */
    static const std::string KEY_FONT_SIZE;                         /*!< key of font size */
    static const std::string KEY_FONT_SMALL;                        /*!< key of font size small */
    static const std::string KEY_FONT_BIG;                          /*!< key of font size big */
    static const std::string KEY_FONT_FACE;                         /*!< key of font name */
    static const std::string KEY_TEXT_BOLD;                         /*!< key of text bold */
    static const std::string KEY_TEXT_ITALIC;                       /*!< key of text italic */
    static const std::string KEY_TEXT_LINE;                         /*!< key of line style */
    static const std::string VALUE_TEXT_LINE_NONE;                  /*!< value of none */
    static const std::string VALUE_TEXT_LINE_DEL;                   /*!< value of strikethrough line */
    static const std::string VALUE_TEXT_LINE_UNDER;                 /*!< value of underline */
    static const std::string KEY_TEXT_STYLE;                        /*!< key of effect style */
    static const std::string VALUE_TEXT_STYLE_NONE;                 /*!< value of none */
    static const std::string VALUE_TEXT_STYLE_OUTLINE;              /*!< value of outline */
    static const std::string VALUE_TEXT_STYLE_SHADOW;               /*!< value of shadow */
    static const std::string VALUE_TEXT_STYLE_GLOW;                 /*!< value of glow */
    static const std::string KEY_TEXT_OUTLINE_COLOR;                /*!< key of outline color */
    static const std::string KEY_TEXT_OUTLINE_SIZE;                 /*!< key of outline size */
    static const std::string KEY_TEXT_SHADOW_COLOR;                 /*!< key of shadow color */
    static const std::string KEY_TEXT_SHADOW_OFFSET_WIDTH;          /*!< key of shadow offset (width) */
    static const std::string KEY_TEXT_SHADOW_OFFSET_HEIGHT;         /*!< key of shadow offset (height) */
    static const std::string KEY_TEXT_SHADOW_BLUR_RADIUS;           /*!< key of shadow blur radius */
    static const std::string KEY_TEXT_GLOW_COLOR;                   /*!< key of glow color */
    static const std::string KEY_URL;                               /*!< key of url */
    static const std::string KEY_ANCHOR_FONT_COLOR_STRING;          /*!< key of font color of anchor tag */
    static const std::string KEY_ANCHOR_TEXT_BOLD;                  /*!< key of text bold of anchor tag */
    static const std::string KEY_ANCHOR_TEXT_ITALIC;                /*!< key of text italic of anchor tag */
    static const std::string KEY_ANCHOR_TEXT_LINE;                  /*!< key of line style of anchor tag */
    static const std::string KEY_ANCHOR_TEXT_STYLE;                 /*!< key of effect style of anchor tag */
    static const std::string KEY_ANCHOR_TEXT_OUTLINE_COLOR;         /*!< key of outline color of anchor tag */
    static const std::string KEY_ANCHOR_TEXT_OUTLINE_SIZE;          /*!< key of outline size of anchor tag */
    static const std::string KEY_ANCHOR_TEXT_SHADOW_COLOR;          /*!< key of shadow color of anchor tag */
    static const std::string KEY_ANCHOR_TEXT_SHADOW_OFFSET_WIDTH;   /*!< key of shadow offset (width) of anchor tag */
    static const std::string KEY_ANCHOR_TEXT_SHADOW_OFFSET_HEIGHT;  /*!< key of shadow offset (height) of anchor tag */
    static const std::string KEY_ANCHOR_TEXT_SHADOW_BLUR_RADIUS;    /*!< key of shadow blur radius of anchor tag */
    static const std::string KEY_ANCHOR_TEXT_GLOW_COLOR;            /*!< key of glow color of anchor tag */
    
    RichText();
    
    /**
     * @brief Create a empty RichText.
     *
     * @return RichText instance.
     */
    static RichText* create();

    /**
     * @brief Create a RichText from an XML
     *
     * @return RichText instance.
     */
    static RichText* createWithXML(const std::string& xml, const ValueMap& defaults = ValueMap(), const OpenUrlHandler& handleOpenUrl = nullptr);

    void insertElement(std::unique_ptr<RichElement> element, int index);
    void pushBackElement(std::unique_ptr<RichElement> element);
    void removeElement(int index);
    
    /**
     * @brief Set vertical space between each RichElement.
     *
     * @param space Point in float.
     */
    void setVerticalSpace(float space);
    
    /**
     * @brief Rearrange all RichElement in the RichText.
     * It's usually called internally.
     */
    void formatText();

    //override functions.
    virtual void ignoreContentAdaptWithSize(bool ignore) override;
    virtual std::string getDescription() const override;

    void setWrapMode(WrapMode wrapMode);                /*!< sets the wrapping mode: WRAP_PER_CHAR or WRAP_PER_WORD */
    WrapMode getWrapMode() const;                       /*!< returns the current wrapping mode */
    void setFontColor(const std::string& color);        /*!< Set the font color. @param color the #RRGGBB hexadecimal notation. */
    std::string getFontColor();                         /*!< return the current font color */
    Color3B getFontColor3B();                           /*!< return the current font color */
    void setFontSize(float size);                       /*!< Set the font size. @param size the font size. */
    float getFontSize();                                /*!< return the current font size */
    void setFontFace(const std::string& face);          /*!< Set the font face. @param face the font face. */
    std::string getFontFace();                          /*!< return the current font face */
    void setAnchorFontColor(const std::string& color);  /*!< Set the font color of a-tag. @param face the font color. */
    std::string getAnchorFontColor();                   /*!< return the current font color of a-tag */
    cocos2d::Color3B getAnchorFontColor3B();            /*!< return the current font color of a-tag */
    void setAnchorTextBold(bool enable);                /*!< enable bold text of a-tag */
    bool isAnchorTextBoldEnabled();                     /*!< valid style is bold text of a-tag? */
    void setAnchorTextItalic(bool enable);              /*!< enable italic text of a-tag */
    bool isAnchorTextItalicEnabled();                   /*!< valid style is italic text of a-tag? */
    void setAnchorTextDel(bool enable);                 /*!< enable the strikethrough of a-tag */
    bool isAnchorTextDelEnabled();                      /*!< valid strikethrough of a-tag? */
    void setAnchorTextUnderline(bool enable);           /*!< enable the underline of a-tag */
    bool isAnchorTextUnderlineEnabled();                /*!< valid underline of a-tag? */
    /** @brief enable the outline of a-tag */
    void setAnchorTextOutline(bool enable, const Color3B& outlineColor = Color3B::WHITE, int outlineSize = -1);
    bool isAnchorTextOutlineEnabled();                  /*!< valid outline of a-tag? */
    Color3B getAnchorTextOutlineColor3B();              /*!< return the current text outline color of a-tag */
    int getAnchorTextOutlineSize();                     /*!< return the current text outline size of a-tag */
    /** @brief enable the shadow of a-tag */
    void setAnchorTextShadow(bool enable, const Color3B& shadowColor = Color3B::BLACK, const Size& offset = Size(2.0, -2.0), int blurRadius = 0);
    bool isAnchorTextShadowEnabled();                   /*!< valid shadow of a-tag? */
    Color3B getAnchorTextShadowColor3B();               /*!< return the current text shadow color of a-tag */
    Size getAnchorTextShadowOffset();                   /*!< return the current text shadow offset of a-tag */
    int getAnchorTextShadowBlurRadius();                /*!< return the current text shadow blur radius of a-tag */
    void setAnchorTextGlow(bool enable, const Color3B& glowColor = Color3B::WHITE); /*!< enable the glow of a-tag */
    bool isAnchorTextGlowEnabled();                     /*!< valid glow of a-tag? */
    Color3B getAnchorTextGlowColor3B();                 /*!< return the current text glow color of a-tag */
    void setDefaults(const ValueMap& defaults);         /*!< set the default values */
    ValueMap getDefaults() const;                       /*!< returns the default values */

    cocos2d::Color3B color3BWithString(const std::string& color);   /*!< convert a color string into a Color3B. */
    std::string stringWithColor3B(const cocos2d::Color3B& color3b); /*!< convert a Color3B into a color string. */
    std::string stringWithColor4B(const cocos2d::Color4B& color4b); /*!< convert a Color4B into a color string. */

    /**
     * @brief add a callback to own tag.
     * @param tag tag's name
     * @param isFontElement use attributes of text tag
     * @param handleVisitEnter callback
     */
    static void setTagDescription(const std::string& tag, bool isFontElement, VisitEnterHandler handleVisitEnter);

    /**
     * @brief remove a callback to own tag.
     * @param tag tag's name
     */
    static void removeTagDescription(const std::string& tag);
    
    void openUrl(const std::string& url);

    /**
     * @brief Asks the callback to open a resource specified by a URL.
     * @discussion If you set up your own URL in the anchor tag, it is used to intercept the URL open.
     * @param handleOpenUrl the callback
     */
    void setOpenUrlHandler(const OpenUrlHandler& handleOpenUrl);

protected:
    virtual bool init() override;

    bool initWithXML(const std::string& xml, const ValueMap& defaults = ValueMap(), const OpenUrlHandler& handleOpenUrl = nullptr);

protected:
    virtual void adaptRenderers() override;

    virtual void initRenderer() override;
    void pushToContainer(Node* renderer);
    void handleTextRenderer(const std::string& text, const std::string& fontName, float fontSize, const Color3B& color,
                            GLubyte opacity, uint32_t flags, const std::string& url="",
                            const Color3B& outlineColor = Color3B::WHITE, int outlineSize = -1,
                            const Color3B& shadowColor = Color3B::BLACK, const cocos2d::Size& shadowOffset = Size(2.0, -2.0), int shadowBlurRadius = 0,
                            const Color3B& glowColor = Color3B::WHITE);
    void handleImageRenderer(const std::string& fileParh, const Color3B& color, GLubyte opacity, int width, int height, const std::string& url);
    void handleCustomRenderer(Node* renderer);
    void formarRenderers();
    void addNewLine();
    int findSplitPositionForWord(cocos2d::Label* label, const std::string& text);
    int findSplitPositionForChar(cocos2d::Label* label, const std::string& text);

    bool _formatTextDirty;
    std::vector<std::unique_ptr<RichElement>> _richElements;
    std::vector<std::vector<node_ptr<Node>>> _elementRenders;
    float _leftSpaceWidth;

    ValueMap _defaults;             /*!< default values */
    OpenUrlHandler _handleOpenUrl;  /*!< the callback for open URL */
};
    
} // namespace ui
} // namespace cocos2d

#endif /* defined(__UIRichText__) */
