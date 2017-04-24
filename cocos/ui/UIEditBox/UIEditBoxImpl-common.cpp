/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2012 James Chen
 Copyright (c) 2013-2015 zilongshanren

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
#include "ui/UIEditBox/UIEditBoxImpl-common.h"

#define kLabelZOrder  9999

#include "ui/UIEditBox/UIEditBox.h"
#include "base/CCDirector.h"
#include "2d/CCLabel.h"
#include "ui/UIHelper.h"

static const int CC_EDIT_BOX_PADDING = 5;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
#define PASSWORD_CHAR "*"
#else
#define PASSWORD_CHAR "\u25CF"
#endif

namespace cocos2d {

namespace ui {

EditBoxImplCommon::EditBoxImplCommon(EditBox* pEditText)
: EditBoxImpl(pEditText)
, _labelId()
, _labelPlaceHolderId()
, _editBoxInputMode(EditBox::InputMode::SINGLE_LINE)
, _editBoxInputFlag(EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS)
, _keyboardReturnType(EditBox::KeyboardReturnType::DEFAULT)
, _alignment(TextHAlignment::LEFT)
, _colText(Color3B::WHITE)
, _colPlaceHolder(Color3B::GRAY)
, _maxLength(-1)
{
}

EditBoxImplCommon::~EditBoxImplCommon()
{
}


Label* EditBoxImplCommon::getLabel() const
{
    auto label = Director::getInstance()->getNodeRegister().get<Label>(_labelId);

    if (label == nullptr)
    {
        throw std::runtime_error("EditBoxImplCommon: label cannot be found");
    }

    return label;
}

Label* EditBoxImplCommon::getLabelPlaceHolder() const
{
    auto labelPlaceHolder = Director::getInstance()->getNodeRegister().get<Label>(_labelPlaceHolderId);

    if (labelPlaceHolder == nullptr)
    {
        throw std::runtime_error("EditBoxImplCommon: label cannot be found");
    }

    return labelPlaceHolder;
}

bool EditBoxImplCommon::initWithSize(const Size& size)
{
    do 
    {
        
        Rect rect = Rect(0, 0, size.width, size.height);
        
        this->createNativeControl(rect);
        
        initInactiveLabels(size);
        setContentSize(size);
        
        return true;
    }while (0);
    
    return false;
}

void EditBoxImplCommon::initInactiveLabels(const Size& size)
{
    const char* pDefaultFontName = this->getNativeDefaultFontName();

    auto label = make_node_ptr<Label>();
    label->setAnchorPoint(Vec2(0, 0.5f));
    label->setColor(Color3B::WHITE);
    label->setVisible(false);

    _labelId = _editBox->addChild( std::move(label), kLabelZOrder);
    
    auto labelPlaceHolder = make_node_ptr<Label>();
    labelPlaceHolder->setAnchorPoint(Vec2(0, 0.5f));
    labelPlaceHolder->setColor(Color3B::GRAY);

    _labelPlaceHolderId = _editBox->addChild( std::move(labelPlaceHolder), kLabelZOrder);
    
    setFont(pDefaultFontName, size.height*2/3);
    setPlaceholderFont(pDefaultFontName, size.height*2/3);
}

void EditBoxImplCommon::placeInactiveLabels()
{
    getLabel()->setPosition(CC_EDIT_BOX_PADDING, _contentSize.height / 2.0f);
    getLabelPlaceHolder()->setPosition(CC_EDIT_BOX_PADDING, _contentSize.height / 2.0f);
    refreshLabelAlignment();
}

void EditBoxImplCommon::setInactiveText(const char* pText)
{
    if(EditBox::InputFlag::PASSWORD == _editBoxInputFlag)
    {
        std::string passwordString;
        for(size_t i = 0; i < strlen(pText); ++i)
            passwordString.append(PASSWORD_CHAR);
        getLabel()->setString(passwordString);
    }
    else
    {
        getLabel()->setString(pText);
    }
    // Clip the text width to fit to the text box
    float fMaxWidth = _editBox->getContentSize().width;
    float fMaxHeight = _editBox->getContentSize().height;
    Size labelSize = getLabel()->getContentSize();
    if(labelSize.width > fMaxWidth || labelSize.height > fMaxHeight)
    {
        getLabel()->setDimensions(fMaxWidth, fMaxHeight);
    }
}
    
void EditBoxImplCommon::setFont(const char* pFontName, int fontSize)
{
    this->setNativeFont(pFontName, fontSize * getLabel()->getNodeToWorldAffineTransform().a);

    if(strlen(pFontName) > 0)
    {
        getLabel()->setSystemFontName(pFontName);
    }
    if(fontSize > 0)
    {
        getLabel()->setSystemFontSize(fontSize);
    }
}

void EditBoxImplCommon::setFontColor(const Color4B& color)
{
    this->setNativeFontColor(color);
    
    getLabel()->setTextColor(color);
}

void EditBoxImplCommon::setPlaceholderFont(const char* pFontName, int fontSize)
{
    this->setNativePlaceholderFont(pFontName, fontSize * getLabelPlaceHolder()->getNodeToWorldAffineTransform().a);
    
    if( strlen(pFontName) > 0)
    {
        getLabelPlaceHolder()->setSystemFontName(pFontName);
    }
    if(fontSize > 0)
    {
        getLabelPlaceHolder()->setSystemFontSize(fontSize);
    }
}
    
void EditBoxImplCommon::setPlaceholderFontColor(const Color4B &color)
{
    this->setNativePlaceholderFontColor(color);
    
    getLabelPlaceHolder()->setTextColor(color);
}

void EditBoxImplCommon::setInputMode(EditBox::InputMode inputMode)
{
    _editBoxInputMode = inputMode;
    this->setNativeInputMode(inputMode);
}

void EditBoxImplCommon::setMaxLength(int maxLength)
{
    _maxLength = maxLength;
    this->setNativeMaxLength(maxLength);
}

int EditBoxImplCommon::getMaxLength()
{
    return _maxLength;
}

void EditBoxImplCommon::setTextHorizontalAlignment(cocos2d::TextHAlignment alignment)
{
    _alignment = alignment;
    this->setNativeTextHorizontalAlignment(alignment);
}

void EditBoxImplCommon::setInputFlag(EditBox::InputFlag inputFlag)
{
    _editBoxInputFlag = inputFlag;
    this->setNativeInputFlag(inputFlag);
}

void EditBoxImplCommon::setReturnType(EditBox::KeyboardReturnType returnType)
{
    _keyboardReturnType = returnType;
    this->setNativeReturnType(returnType);
}
    
void EditBoxImplCommon::refreshInactiveText()
{
    setInactiveText(_text.c_str());

    refreshLabelAlignment();

    if(_text.size() == 0)
    {
        getLabel()->setVisible(false);
        getLabelPlaceHolder()->setVisible(true);
    }
    else
    {
        getLabel()->setVisible(true);
        getLabelPlaceHolder()->setVisible(false);
    }
}

void EditBoxImplCommon::refreshLabelAlignment()
{
    getLabel()->setWidth(_contentSize.width);
    getLabelPlaceHolder()->setWidth(_contentSize.width);

    getLabel()->setHorizontalAlignment(_alignment);
    getLabelPlaceHolder()->setHorizontalAlignment(_alignment);
}

void EditBoxImplCommon::setText(const char* text)
{
    this->setNativeText(text);
    _text = text;
    refreshInactiveText();
}

const char*  EditBoxImplCommon::getText(void)
{
    return _text.c_str();
}

void EditBoxImplCommon::setPlaceHolder(const char* pText)
{
    if (pText != NULL)
    {
        _placeHolder = pText;
        getLabelPlaceHolder()->setString(_placeHolder);

        this->setNativePlaceHolder(pText);
    }
}


void EditBoxImplCommon::setVisible(bool visible)
{
    if(visible) {
        refreshInactiveText();
    } else {
        this->setNativeVisible(visible);
        getLabel()->setVisible(visible);
        getLabelPlaceHolder()->setVisible(visible);
    }
}

void EditBoxImplCommon::setContentSize(const Size& size)
{
    _contentSize = size;
    CCLOG("[Edit text] content size = (%f, %f)", size.width, size.height);
    placeInactiveLabels();
}

void EditBoxImplCommon::draw(Renderer* /*renderer*/, const Mat4& /*transform*/, uint32_t flags)
{
    if(flags)
    {
        auto rect = ui::Helper::convertBoundingBoxToScreen(_editBox);
        this->updateNativeFrame(rect);
    }
}

void EditBoxImplCommon::onEnter(void)
{
    const char* pText = getText();
    if (pText) {
        setInactiveText(pText);
    }
}

void EditBoxImplCommon::openKeyboard()
{
    getLabel()->setVisible(false);
    getLabelPlaceHolder()->setVisible(false);

    this->setNativeVisible(true);
    this->nativeOpenKeyboard();
}

void EditBoxImplCommon::closeKeyboard()
{
    this->nativeCloseKeyboard();
}

void EditBoxImplCommon::onEndEditing(const std::string& /*text*/)
{
    this->setNativeVisible(false);
    
    refreshInactiveText();
}
    
void EditBoxImplCommon::editBoxEditingDidBegin()
{
    // LOGD("textFieldShouldBeginEditing...");
    cocos2d::ui::EditBoxDelegate *pDelegate = _editBox->getDelegate();
    
    if (pDelegate != nullptr)
    {
        pDelegate->editBoxEditingDidBegin(_editBox);
    }
}

  void EditBoxImplCommon::editBoxEditingDidEnd(const std::string& text, EditBoxDelegate::EditBoxEndAction action)
{
    // LOGD("textFieldShouldEndEditing...");
    _text = text;
    
    cocos2d::ui::EditBoxDelegate *pDelegate = _editBox->getDelegate();
    if (pDelegate != nullptr)
    {
        pDelegate->editBoxEditingDidEndWithAction(_editBox, action);
        pDelegate->editBoxReturn(_editBox);
    }
    
    if (_editBox != nullptr)
    {
        this->onEndEditing(_text);
    }
}

void EditBoxImplCommon::editBoxEditingChanged(const std::string& text)
{
    // LOGD("editBoxTextChanged...");
    cocos2d::ui::EditBoxDelegate *pDelegate = _editBox->getDelegate();
    _text = text;
    if (pDelegate != nullptr)
    {
        pDelegate->editBoxTextChanged(_editBox, text);
    }
}


}

} // namespace cocos2d


