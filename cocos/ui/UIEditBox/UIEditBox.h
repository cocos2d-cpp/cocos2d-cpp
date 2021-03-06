/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2012 James Chen
 
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

#ifndef __UIEDITTEXT_H__
#define __UIEDITTEXT_H__

#include "base/CCIMEDelegate.h"
#include "ui/GUIDefine.h"
#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

namespace cocos2d {

/**
 * @addtogroup ui
 * @{
 */
namespace ui {
        
    class EditBox;
    class EditBoxImpl;
        
    /**
     *@brief Editbox delegate class.
     * It's useful when you want to do some customization during Editbox input event
     *
     * @js NA
     * @lua NA
     */
    class CC_GUI_DLL EditBoxDelegate
    {
    public:

        /**
         * Reason for ending edit (for platforms where it is known)
         */
        enum class EditBoxEndAction {
            UNKNOWN,
            TAB_TO_NEXT,
            TAB_TO_PREVIOUS,
            RETURN
        };

        virtual ~EditBoxDelegate() {}
            
        /**
         * This method is called when an edit box gains focus after keyboard is shown.
         * @param editBox The edit box object that generated the event.
         */
        virtual void editBoxEditingDidBegin(EditBox*) {}
            
            
        /**
         * This method is called when the edit box text was changed.
         * @param editBox The edit box object that generated the event.
         * @param text The new text.
         */
        virtual void editBoxTextChanged(EditBox*, const std::string &) {}
            
        /**
         * This method is called when the return button was pressed or the outside area of keyboard was touched.
         * @param editBox The edit box object that generated the event.
         */
        virtual void editBoxReturn(EditBox* editBox) = 0;

        /**
         * This method is called when an edit box loses focus after keyboard is hidden.
         * @param editBox The edit box object that generated the event.
         * @param type The reason why editing ended.
         */
        virtual void editBoxEditingDidEndWithAction(EditBox*, EditBoxEndAction) {}
    };
        
    /**
     * @brief Class for edit box.
     *
     * You can use this widget to gather small amounts of text from the user.
     *
     */
        
    class CC_GUI_DLL EditBox
        : public Widget
        , public IMEDelegate
    {
    public:

        /**
         * The popup keyboard return type.
         */
        enum class KeyboardReturnType
        {
            DEFAULT,
            DONE,
            SEND,
            SEARCH,
            GO,
            NEXT
        };
            
        /**
         * @brief The EditBox::InputMode defines the type of text that the user is allowed
         * to enter.
         */
        enum class InputMode
        {
            /**
             * The user is allowed to enter any text, including line breaks.
             */
            ANY,
                
            /**
             * The user is allowed to enter an e-mail address.
             */
            EMAIL_ADDRESS,
                
            /**
             * The user is allowed to enter an integer value.
             */
            NUMERIC,
                
            /**
             * The user is allowed to enter a phone number.
             */
            PHONE_NUMBER,
                
            /**
             * The user is allowed to enter a URL.
             */
            URL,
                
            /**
             * The user is allowed to enter a real number value.
             * This extends kEditBoxInputModeNumeric by allowing a decimal point.
             */
            DECIMAL,
                
            /**
             * The user is allowed to enter any text, except for line breaks.
             */
            SINGLE_LINE,
        };
            
        /**
         * @brief The EditBox::InputFlag defines how the input text is displayed/formatted.
         */
        enum class InputFlag
        {
            /**
             * Indicates that the text entered is confidential data that should be
             * obscured whenever possible. This implies EDIT_BOX_INPUT_FLAG_SENSITIVE.
             */
            PASSWORD,
                
            /**
             * Indicates that the text entered is sensitive data that the
             * implementation must never store into a dictionary or table for use
             * in predictive, auto-completing, or other accelerated input schemes.
             * A credit card number is an example of sensitive data.
             */
            SENSITIVE,
                
            /**
             * This flag is a hint to the implementation that during text editing,
             * the initial letter of each word should be capitalized.
             */
            INITIAL_CAPS_WORD,
                
            /**
             * This flag is a hint to the implementation that during text editing,
             * the initial letter of each sentence should be capitalized.
             */
            INITIAL_CAPS_SENTENCE,
                
            /**
             * Capitalize all characters automatically.
             */
            INITIAL_CAPS_ALL_CHARACTERS,
            
            /**
             * Lowercase all characters automatically.
             */
            LOWERCASE_ALL_CHARACTERS
        };
            
        /**
         * create a edit box with size.
         * @return An autorelease pointer of EditBox, you don't need to release it only if you retain it again.
         */
        static EditBox* create(const Size& size,
                               Scale9Sprite* normalSprite,
                               Scale9Sprite* pressedSprite = nullptr,
                               Scale9Sprite* disabledSprite = nullptr);

            
        /**
         * create a edit box with size.
         * @return An autorelease pointer of EditBox, you don't need to release it only if you retain it again.
         */
        static EditBox* create(const Size& size,
                               const std::string& normal9SpriteBg,
                               TextureResType texType = TextureResType::LOCAL);
            
        EditBox();
            
        virtual ~EditBox();
            
        /**
         * Init edit box with specified size. This method should be invoked right after constructor.
         * @param size The size of edit box.
         * @param normal9SpriteBg  background image of edit box.
         * @param texType the resource type, the default value is TextureResType::LOCAL
         * @return Whether initialization is successfully or not.
         */
        bool initWithSizeAndBackgroundSprite(const Size& size,
                                             const std::string& normal9SpriteBg,
                                             TextureResType texType = TextureResType::LOCAL);
            
        
        /**
         * Init edit box with specified size. This method should be invoked right after constructor.
         * @param size The size of edit box.
         * @param normal9SpriteBg  background image of edit box.
         * @return Whether initialization is successfully or not.
         */
        bool initWithSizeAndBackgroundSprite(const Size& size, Scale9Sprite* normal9SpriteBg);

        /**
         * Gets/Sets the delegate for edit box.
         */
        void setDelegate(EditBoxDelegate* delegate);

        EditBoxDelegate* getDelegate();
            
        /**
         * Set the text entered in the edit box.
         * @param pText The given text.
         */
        void setText(const char* pText);
            
        /**
         * Get the text entered in the edit box.
         * @return The text entered in the edit box.
         */
        const char* getText(void);
            
        /**
         * Set the font. Only system font is allowed.
         * @param pFontName The font name.
         * @param fontSize The font size.
         */
        void setFont(const char* pFontName, int fontSize);
            
        /**
         * Set the font name. Only system font is allowed.
         * @param pFontName The font name.
         */
        void setFontName(const char* pFontName);
            
        /**
         * Set the font size.
         * @param fontSize The font size.
         */
        void setFontSize(int fontSize);
            
        /**
         * Set the font color of the widget's text.
         */
        void setFontColor(const Color3B& color);
        void setFontColor(const Color4B& color);
            
        /**
         * Set the placeholder's font. Only system font is allowed.
         * @param pFontName The font name.
         * @param fontSize The font size.
         */
        void setPlaceholderFont(const char* pFontName, int fontSize);
            
        /**
         * Set the placeholder's font name. only system font is allowed.
         * @param pFontName The font name.
         */
        void setPlaceholderFontName(const char* pFontName);
            
        /**
         * Set the placeholder's font size.
         * @param fontSize The font size.
         */
        void setPlaceholderFontSize(int fontSize);
            
        /**
         * Set the font color of the placeholder text when the edit box is empty.
         */
        void setPlaceholderFontColor(const Color3B& color);

        /**
         * Set the font color of the placeholder text when the edit box is empty.
         */
        void setPlaceholderFontColor(const Color4B& color);
            
        /**
         * Set a text in the edit box that acts as a placeholder when an
         * edit box is empty.
         * @param pText The given text.
         */
        void setPlaceHolder(const char* pText);
            
        /**
         * Get a text in the edit box that acts as a placeholder when an
         * edit box is empty.
         */
        const char* getPlaceHolder(void);
            
        /**
         * Set the input mode of the edit box.
         * @param inputMode One of the EditBox::InputMode constants.
         */
        void setInputMode(InputMode inputMode);
            
        /**
         * Sets the maximum input length of the edit box.
         * Setting this value enables multiline input mode by default.
         * Available on Android, iOS and Windows Phone.
         * @param maxLength The maximum length.
         */
        void setMaxLength(int maxLength);
            
        /**
         * Gets the maximum input length of the edit box.
         * @return Maximum input length.
         */
        int getMaxLength();
            
        /**
         * Set the input flags that are to be applied to the edit box.
         * @param inputFlag One of the EditBox::InputFlag constants.
         */
        void setInputFlag(InputFlag inputFlag);
            
        /**
         * Set the return type that are to be applied to the edit box.
         * @param returnType One of the EditBox::KeyboardReturnType constants.
         */
        void setReturnType(EditBox::KeyboardReturnType returnType);

        void setTextHorizontalAlignment(cocos2d::TextHAlignment alignment);

        /* override functions */
        virtual void setPosition(const Vec2& pos) override;
        virtual void setVisible(bool visible) override;
        virtual void setContentSize(const Size& size) override;
        virtual void setAnchorPoint(const Vec2& anchorPoint) override;

        /**
         * Returns the "class name" of widget.
         */
        virtual std::string getDescription() const override;

        virtual void draw(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
        virtual void onEnter(void) override;
        virtual void onExit(void) override;
        virtual void keyboardWillShow(IMEKeyboardNotificationInfo& info) override;
        virtual void keyboardWillHide(IMEKeyboardNotificationInfo& info) override;
            
        /* callback functions
         * @js NA
         * @lua NA
         */
        void touchDownAction(Ref *sender, TouchEventType controlEvent);
            
    protected:
        virtual void adaptRenderers() override;

        void updatePosition(float dt);
        EditBoxImpl*      _editBoxImpl;
        EditBoxDelegate*  _delegate;
            
        InputMode    _editBoxInputMode;
        InputFlag    _editBoxInputFlag;
        EditBox::KeyboardReturnType  _keyboardReturnType;
        cocos2d::TextHAlignment _alignment;

        Scale9Sprite *_backgroundSprite;
        std::string _text;
        std::string _placeHolder;
            
        std::string _fontName;
        std::string _placeholderFontName;
            
        int _fontSize;
        int _placeholderFontSize;
            
        Color4B _colText;
        Color4B _colPlaceHolder;
            
        int   _maxLength;
        float _adjustHeight;
    };
}

// end of ui group
/// @}
} // namespace cocos2d

#endif /* __UIEDITTEXT_H__ */

