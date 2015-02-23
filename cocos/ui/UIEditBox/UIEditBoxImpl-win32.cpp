/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2013 Jozef Pridavok
 
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
#include "UIEditBoxImpl-win32.h"

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include "UIEditBox.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "2d/CCLabel.h"
#include "base/CCDirector.h"
#include "base/ccUTF8.h"

#pragma warning (disable: 4312)

class CWin32InputBox;

// Structure used to orient the inputbox behavior
struct WIN32INPUTBOX_PARAM
{
	friend class CWin32InputBox;

	//
	IN OPTIONAL bool bMultiline;

	// Pass this as none zero so to use this memory dlg template
	IN OPTIONAL LPVOID DlgTemplateData;

	// Pass this as none ZERO so to load DLGTEMPLATE from resources
	IN OPTIONAL LPCSTR DlgTemplateName;

	// passing both "DlgTemplateName" and "DlgTemplateData" ZERO will cause
	// the dialog to use his default embedded resource

	// Center on monitor or owner window?
	IN OPTIONAL bool bCenter;

	// Want to add more styles to the dialog?
	IN OPTIONAL DWORD dwStylesPlus, dwStylesMinus;
	IN OPTIONAL DWORD dwExStylesPlus, dwExStylesMinus;

	IN LPCSTR szTitle, szPrompt;

	// Return buffer
	std::string* pstrResult;

	IN DWORD nMaxLength;

	// Owner window
	HWND hwndOwner;
	HINSTANCE hInstance;

	short xPos, yPos;

	WIN32INPUTBOX_PARAM();
private:
	HWND hDlg;
};

class CWin32InputBox
{
private:
	WIN32INPUTBOX_PARAM *_param;
	static LRESULT CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
	HWND _hwndEditCtrl;

	void InitDialog();
	void SetParam(WIN32INPUTBOX_PARAM *);
	WIN32INPUTBOX_PARAM * GetParam();

public:

	CWin32InputBox(WIN32INPUTBOX_PARAM *);
	~CWin32InputBox();

	static INT_PTR InputBoxEx(WIN32INPUTBOX_PARAM *);
	static INT_PTR InputBox(
		LPCSTR szTitle,
		LPCSTR szPrompt,
		std::string* pstrResult,
		DWORD nMaxLength,
		bool bMultiLine = false,
		HWND hwndParent = 0);
};




typedef struct _MSDN_DLGTEMPLATEEX
{
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
	BYTE  _rest[1]; // rest of structure
} MSDN_DLGTEMPLATEEX;

static bool IsDlgTemplateExtended(DLGTEMPLATE *dlgTemplate)
{
	MSDN_DLGTEMPLATEEX *dgExTemplate = (MSDN_DLGTEMPLATEEX *)dlgTemplate;

	// MSDN excerpt:
	//* dlgVer
	//  Specifies the version number of the extended dialog box template. This member must be 1. 
	//* signature
	//  Indicates whether a template is an extended dialog box template. 
	// If signature is 0xFFFF, this is an extended dialog box template. 
	// In this case, the dlgVer member specifies the template version number. 
	// If signature is any value other than 0xFFFF, this is a standard dialog box template that uses the DLGTEMPLATE and DLGITEMTEMPLATE structures. 

	return (dgExTemplate->dlgVer == 1) && (dgExTemplate->signature == 0xFFFF);
}

// Use alignment if supported by the compiler
#ifdef _MSC_VER
#if _MSC_VER > 1200
__declspec(align(4))
#endif
#endif

// per the MSDN, the DLGTEMPLATE must be DWORD aligned
// this was generated by the DlgResToDlgTemplate tool
static unsigned char definputbox_dlg[] =
{
	0x01, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x00, 0xc8, 0x00, 0x06,
	0x00, 0x16, 0x00, 0x11, 0x00, 0xe7, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x57, 0x00, 0x69,
	0x00, 0x6e, 0x00, 0x33, 0x00, 0x32, 0x00, 0x49, 0x00, 0x6e, 0x00, 0x70, 0x00, 0x75, 0x00, 0x74,
	0x00, 0x42, 0x00, 0x6f, 0x00, 0x78, 0x00, 0x00, 0x00, 0x08, 0x00, 0xbc, 0x02, 0x00, 0x00, 0x4d,
	0x00, 0x53, 0x00, 0x20, 0x00, 0x53, 0x00, 0x68, 0x00, 0x65, 0x00, 0x6c, 0x00, 0x6c, 0x00, 0x20,
	0x00, 0x44, 0x00, 0x6c, 0x00, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x80, 0x00, 0x02, 0x50, 0x06, 0x00, 0x04, 0x00, 0x9d, 0x00, 0x21, 0x00, 0xe8,
	0x03, 0x00, 0x00, 0xff, 0xff, 0x82, 0x00, 0x50, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x70,
	0x00, 0x74, 0x00, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x80, 0x00, 0x81, 0x50, 0x06, 0x00, 0x25, 0x00, 0xd8, 0x00, 0x0e, 0x00, 0xe9,
	0x03, 0x00, 0x00, 0xff, 0xff, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x84, 0x10, 0xa1, 0x50, 0x06, 0x00, 0x37, 0x00, 0xd8, 0x00, 0x31, 0x00, 0xea,
	0x03, 0x00, 0x00, 0xff, 0xff, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x50, 0xab, 0x00, 0x04, 0x00, 0x33, 0x00, 0x0e, 0x00, 0x01,
	0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x4f, 0x00, 0x4b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x50, 0xab, 0x00, 0x15, 0x00, 0x33,
	0x00, 0x0e, 0x00, 0x02, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x43, 0x00, 0x41, 0x00, 0x4e,
	0x00, 0x43, 0x00, 0x45, 0x00, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x27, 0x00, 0x08, 0x00, 0x08, 0x00, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00
};

static LPCTSTR definputbox_buttonnames[] = { _T("OK"), _T("CANCEL") };
static const INT_PTR definputbox_buttonids[] = { IDOK, IDCANCEL };

static const INT
definputbox_id_prompt = 1000,
definputbox_id_edit1 = 1001,
definputbox_id_edit2 = 1002;

WIN32INPUTBOX_PARAM::WIN32INPUTBOX_PARAM()
{
	bMultiline = false;
	hwndOwner = 0;
	DlgTemplateName = 0;
	hInstance = (HINSTANCE) ::GetModuleHandle(0);
	DlgTemplateData = definputbox_dlg;

	bCenter = true;

	dwStylesPlus = 0;
	dwExStylesPlus = 0;
	dwStylesMinus = 0xFFFFFFFF;
	dwExStylesMinus = 0xFFFFFFFF;

	xPos = yPos = -1;

	pstrResult = nullptr;
    nMaxLength = (DWORD) -1;
}

CWin32InputBox::CWin32InputBox(WIN32INPUTBOX_PARAM *param)
{
	_param = param;
}

CWin32InputBox::~CWin32InputBox()
{

}

void CWin32InputBox::SetParam(WIN32INPUTBOX_PARAM *param)
{
	_param = param;
}

WIN32INPUTBOX_PARAM *CWin32InputBox::GetParam()
{
	return _param;
}

INT_PTR CWin32InputBox::InputBoxEx(WIN32INPUTBOX_PARAM *param)
{
	// Check mandatory parameters
	if (param->pstrResult == nullptr)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}

	LPDLGTEMPLATE dlgTemplate;

	if (param->DlgTemplateName != 0)
	{
		HMODULE hModule = (HMODULE)param->hInstance;
#ifdef __MINGW32__
		HRSRC rcDlg = ::FindResource(hModule, (LPWSTR)(ULONG_PTR)(size_t)(param->DlgTemplateName), RT_DIALOG);
#else
		HRSRC rcDlg = ::FindResource(hModule, MAKEINTRESOURCE(param->DlgTemplateName), RT_DIALOG);
#endif
		if (rcDlg == nullptr)
			return 0;

		HGLOBAL hglobalDlg = ::LoadResource(hModule, rcDlg);
		if (hglobalDlg == nullptr)
			return 0;

		dlgTemplate = (LPDLGTEMPLATE)hglobalDlg;
	}
	else if (param->DlgTemplateData != 0)
	{
		dlgTemplate = (LPDLGTEMPLATE)param->DlgTemplateData;
	}

	MSDN_DLGTEMPLATEEX *dlgTemplateEx =
		IsDlgTemplateExtended((LPDLGTEMPLATE)dlgTemplate) ? (MSDN_DLGTEMPLATEEX *)dlgTemplate : 0;

	if (dlgTemplateEx != 0)
	{
		dlgTemplateEx->exStyle |= param->dwExStylesPlus;
		dlgTemplateEx->style |= param->dwStylesPlus;
		dlgTemplateEx->exStyle &= param->dwExStylesMinus;
		dlgTemplateEx->style &= param->dwStylesMinus;

		if (param->bCenter)
			dlgTemplateEx->style |= DS_CENTER;

		if (param->xPos != -1)
			dlgTemplateEx->x = param->xPos;
		if (param->yPos != -1)
			dlgTemplateEx->y = param->yPos;
	}
	else
	{
		dlgTemplate->dwExtendedStyle |= param->dwExStylesPlus;
		dlgTemplate->style |= param->dwStylesPlus;
		dlgTemplate->dwExtendedStyle &= param->dwExStylesMinus;
		dlgTemplate->style &= param->dwStylesMinus;

		if (param->bCenter)
			dlgTemplate->style |= DS_CENTER;

		if (param->xPos != -1)
			dlgTemplate->x = param->xPos;

		if (param->yPos != -1)
			dlgTemplate->y = param->yPos;
	}

	CWin32InputBox inputbox(param);

	// Resize dialog and SHOW or HIDE multiline
	INT_PTR r = ::DialogBoxIndirectParam(param->hInstance, dlgTemplate, param->hwndOwner, (DLGPROC)DlgProc, (LPARAM)&inputbox);

	return r;
}

INT_PTR CWin32InputBox::InputBox(
	LPCSTR szTitle,
	LPCSTR szPrompt,
	std::string* pstrResult,
	DWORD nResultSize,
	bool bMultiLine,
	HWND hwndParent)
{
	WIN32INPUTBOX_PARAM param;

	param.szTitle = szTitle;
	param.szPrompt = szPrompt;
    param.pstrResult = pstrResult;
    param.nMaxLength = nResultSize;
	param.bMultiline = bMultiLine;
	param.hwndOwner = hwndParent;
	return InputBoxEx(&param);
}

void CWin32InputBox::InitDialog()
{
	// Set the button captions
	for (size_t i = 0; i<sizeof(definputbox_buttonids) / sizeof(definputbox_buttonids[0]); i++)
		::SetDlgItemText(_param->hDlg, (int)definputbox_buttonids[i], definputbox_buttonnames[i]);

	// Set other controls
	std::u16string utf16Title;
	cocos2d::StringUtils::UTF8ToUTF16(_param->szTitle, utf16Title);
	::SetWindowTextW(_param->hDlg, (LPCWSTR) utf16Title.c_str());

	std::u16string utf16Prompt;
	cocos2d::StringUtils::UTF8ToUTF16(_param->szTitle, utf16Prompt);
	::SetDlgItemTextW(_param->hDlg, definputbox_id_prompt, (LPCWSTR) utf16Prompt.c_str());

	HWND hwndEdit1 = ::GetDlgItem(_param->hDlg, definputbox_id_edit1);
	HWND hwndEdit2 = ::GetDlgItem(_param->hDlg, definputbox_id_edit2);

	if (_param->bMultiline)
		_hwndEditCtrl = hwndEdit2;
	else
		_hwndEditCtrl = hwndEdit1;

	std::u16string utf16Result;
	cocos2d::StringUtils::UTF8ToUTF16(_param->pstrResult->c_str(), utf16Result);
	::SetWindowTextW(_hwndEditCtrl, (LPCWSTR) utf16Result.c_str());

	RECT rectDlg, rectEdit1, rectEdit2;

	::GetWindowRect(_param->hDlg, &rectDlg);
	::GetWindowRect(hwndEdit1, &rectEdit1);
	::GetWindowRect(hwndEdit2, &rectEdit2);

	if (_param->bMultiline)
	{
		::ShowWindow(hwndEdit1, SW_HIDE);
		::SetWindowPos(
			hwndEdit2,
			HWND_NOTOPMOST,
			rectEdit1.left - rectDlg.left,
			(rectEdit1.top - rectDlg.top) - (rectEdit1.bottom - rectEdit1.top),
			0,
			0,
			SWP_NOSIZE | SWP_NOZORDER);

		::SetWindowPos(
			_param->hDlg,
			HWND_NOTOPMOST,
			0,
			0,
			rectDlg.right - rectDlg.left,
			rectDlg.bottom - rectDlg.top - (rectEdit1.bottom - rectEdit1.top),
			SWP_NOMOVE);

	}
	else
	{
		::SetWindowPos(
			_param->hDlg,
			HWND_NOTOPMOST,
			0,
			0,
			rectDlg.right - rectDlg.left,
			rectEdit1.bottom - rectDlg.top + 5,
			SWP_NOMOVE);

		::ShowWindow(hwndEdit2, SW_HIDE);
	}
}

// Message handler for about box.
LRESULT CALLBACK CWin32InputBox::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	CWin32InputBox *_this = (CWin32InputBox *) ::GetWindowLongPtr(hDlg, GWLP_USERDATA);
	WIN32INPUTBOX_PARAM *param = _this ? _this->GetParam() : 0;

	switch (message)
	{
	case WM_INITDIALOG:
	{
						  SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)lParam);

						  _this = (CWin32InputBox *)lParam;
						  _this->_param->hDlg = hDlg;
						  _this->InitDialog();
						  return TRUE;
	}

	case WM_COMMAND:
	{
#ifdef _MY_DEBUG
					   CHAR buf[1024];
					   static int i = 0;
					   sprintf(buf, "WM_COMMAND: %09d wParam=%08X lParam=%08X\n", i++, wParam, lParam);
					   OutputDebugString(buf);
#endif
					   INT_PTR buttonId = LOWORD(wParam);
					   for (size_t i = 0;
						   i<sizeof(definputbox_buttonids) / sizeof(definputbox_buttonids[0]);
						   i++)
					   {
						   if (buttonId == definputbox_buttonids[i])
						   {
							   std::u16string wstrResult;
							   std::string utf8Result;

							   int inputLength = ::GetWindowTextLengthW(_this->_hwndEditCtrl);
							   wstrResult.resize(inputLength);

							   ::GetWindowTextW(
								   _this->_hwndEditCtrl,
								   (LPWSTR) const_cast<char16_t*>(wstrResult.c_str()),
								   inputLength+1);

							   bool conversionResult = cocos2d::StringUtils::UTF16ToUTF8(wstrResult, utf8Result);
							   _this->_param->pstrResult->clear();
							   if (conversionResult)
							   {
								   if((_this->_param->nMaxLength > 0) && (_this->_param->nMaxLength < (DWORD) cocos2d::StringUtils::getCharacterCountInUTF8String(utf8Result)))
								   {
									   // LengthFilter
									   for(size_t pos=0; pos < _this->_param->nMaxLength; pos++)
									   {
										   std::string utf8Bytes;
										   std::u16string utf16Character(1, wstrResult[pos]);

										   if(cocos2d::StringUtils::UTF16ToUTF8(utf16Character, utf8Bytes))
										   {
											   _this->_param->pstrResult->append(utf8Bytes);
										   }
									   }
								   }
								   else
								   {
									   *(_this->_param->pstrResult) = utf8Result;
								   }
							   }

							   ::EndDialog(hDlg, buttonId);
							   return TRUE;
						   }
					   }
	}
		break;
	}
	return FALSE;
}


NS_CC_BEGIN

namespace ui {

EditBoxImpl* __createSystemEditBox(EditBox* pEditBox)
{
    return new EditBoxImplWin(pEditBox);
}

EditBoxImplWin::EditBoxImplWin(EditBox* pEditText)
: EditBoxImpl(pEditText)
, _label(nullptr)
, _labelPlaceHolder(nullptr)
, _editBoxInputMode(EditBox::InputMode::SINGLE_LINE)
, _editBoxInputFlag(EditBox::InputFlag::INTIAL_CAPS_ALL_CHARACTERS)
, _keyboardReturnType(EditBox::KeyboardReturnType::DEFAULT)
, _colText(Color3B::WHITE)
, _colPlaceHolder(Color3B::GRAY)
, _maxLength(-1)
{
    
}

EditBoxImplWin::~EditBoxImplWin()
{
}

void EditBoxImplWin::doAnimationWhenKeyboardMove(float duration, float distance)
{
}

static const int CC_EDIT_BOX_PADDING = 5;

bool EditBoxImplWin::initWithSize(const Size& size)
{
    //! int fontSize = getFontSizeAccordingHeightJni(size.height-12);
    _label = Label::create();
    _label->setSystemFontSize(size.height-12);
	// align the text vertically center
    _label->setAnchorPoint(Vec2(0, 0.5f));
    _label->setPosition(Vec2(CC_EDIT_BOX_PADDING, size.height / 2.0f));
    _label->setColor(_colText);
    _editBox->addChild(_label);

    _labelPlaceHolder = Label::create();
    _labelPlaceHolder->setSystemFontSize(size.height-12);
	// align the text vertically center
    _labelPlaceHolder->setAnchorPoint(Vec2(0, 0.5f));
    _labelPlaceHolder->setPosition(CC_EDIT_BOX_PADDING, size.height / 2.0f);
    _labelPlaceHolder->setVisible(false);
    _labelPlaceHolder->setColor(_colPlaceHolder);
    _editBox->addChild(_labelPlaceHolder);
    
    _editSize = size;
    return true;
}

void EditBoxImplWin::setFont(const char* pFontName, int fontSize)
{
	if(_label != nullptr) {
		_label->setSystemFontName(pFontName);
		_label->setSystemFontSize(fontSize);
	}
	
	if(_labelPlaceHolder != nullptr) {
		_labelPlaceHolder->setSystemFontName(pFontName);
		_labelPlaceHolder->setSystemFontSize(fontSize);
	}
}

void EditBoxImplWin::setFontColor(const Color3B& color)
{
    _colText = color;
    _label->setColor(color);
}

void EditBoxImplWin::setPlaceholderFont(const char* pFontName, int fontSize)
{
	if(_labelPlaceHolder != nullptr) {
		_labelPlaceHolder->setSystemFontName(pFontName);
		_labelPlaceHolder->setSystemFontSize(fontSize);
	}
}

void EditBoxImplWin::setPlaceholderFontColor(const Color3B& color)
{
    _colPlaceHolder = color;
    _labelPlaceHolder->setColor(color);
}

void EditBoxImplWin::setInputMode(EditBox::InputMode inputMode)
{
    _editBoxInputMode = inputMode;
}

void EditBoxImplWin::setMaxLength(int maxLength)
{
    _maxLength = maxLength;
}

int EditBoxImplWin::getMaxLength()
{
    return _maxLength;
}

void EditBoxImplWin::setInputFlag(EditBox::InputFlag inputFlag)
{
    _editBoxInputFlag = inputFlag;
}

void EditBoxImplWin::setReturnType(EditBox::KeyboardReturnType returnType)
{
    _keyboardReturnType = returnType;
}

bool EditBoxImplWin::isEditing()
{
    return false;
}

void EditBoxImplWin::setText(const char* pText)
{
    if (pText != nullptr)
    {
        _text = pText;

        if (_text.length() > 0)
        {
            _labelPlaceHolder->setVisible(false);

            std::string strToShow;

			if (EditBox::InputFlag::PASSWORD == _editBoxInputFlag)
            {
				long length = StringUtils::getCharacterCountInUTF8String(_text);
                for (long i = 0; i < length; i++)
                {
                    strToShow.append("*");
                }
            }
            else
            {
                strToShow = _text;
            }

            //! std::string strWithEllipsis = getStringWithEllipsisJni(strToShow.c_str(), _editSize.width, _editSize.height-12);
            //! _label->setString(strWithEllipsis.c_str());
            _label->setString(strToShow.c_str());
            
            float maxWidth = _editSize.width - 2 * CC_EDIT_BOX_PADDING;
            auto labelSize = _label->getContentSize();
            if (labelSize.width > maxWidth)
            {
                _label->setDimensions(maxWidth, labelSize.height);
            }
        }
        else
        {
            _labelPlaceHolder->setVisible(true);
            _label->setString("");
        }

    }
}

const char*  EditBoxImplWin::getText(void)
{
    return _text.c_str();
}

void EditBoxImplWin::setPlaceHolder(const char* pText)
{
    if (pText != nullptr)
    {
        _placeHolder = pText;
        if (_placeHolder.length() > 0 && _text.length() == 0)
        {
            _labelPlaceHolder->setVisible(true);
        }

        _labelPlaceHolder->setString(_placeHolder.c_str());
    }
}

void EditBoxImplWin::setPosition(const Vec2& pos)
{
	//_label->setPosition(pos);
	//_labelPlaceHolder->setPosition(pos);
}

void EditBoxImplWin::setVisible(bool visible)
{ // don't need to be implemented on win32 platform.
}

void EditBoxImplWin::setContentSize(const Size& size)
{
}

void EditBoxImplWin::setAnchorPoint(const Vec2& anchorPoint)
{ // don't need to be implemented on win32 platform.
	
}

void EditBoxImplWin::visit(void)
{   
}

void EditBoxImplWin::openKeyboard()
{
    if (_delegate != nullptr)
    {
        _delegate->editBoxEditingDidBegin(_editBox);
    }

#if CC_ENABLE_SCRIPT_BINDING
    auto editBox = this->getEditBox();
    if (editBox && editBox->getScriptEditBoxHandler())
    {
        CommonScriptData data(editBox->getScriptEditBoxHandler(), "began",editBox);
        ScriptEvent event(kCommonEvent,(void*)&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
#endif
    
	std::string placeHolder = _labelPlaceHolder->getString();
	if (placeHolder.length() == 0)
		placeHolder = "Enter value";

	std::string text = getText();
	auto glView = Director::getInstance()->getOpenGLView();
	HWND hwnd = glView->getWin32Window();
	bool didChange = CWin32InputBox::InputBox("Input", placeHolder.c_str(), &text, _maxLength, false, hwnd) == IDOK;
	
	if (didChange) 	
		setText(text.c_str());

	if (_delegate != nullptr) {
		if (didChange)
			_delegate->editBoxTextChanged(_editBox, getText());
		_delegate->editBoxEditingDidEnd(_editBox);
		_delegate->editBoxReturn(_editBox);
	}
    
#if CC_ENABLE_SCRIPT_BINDING
    if (nullptr != _editBox && 0 != _editBox->getScriptEditBoxHandler())
    {
        CommonScriptData data(_editBox->getScriptEditBoxHandler(), "changed",_editBox);
        ScriptEvent event(kCommonEvent,(void*)&data);
        if (didChange)
        {
            ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
        }
        memset(data.eventName,0,sizeof(data.eventName));
        strncpy(data.eventName,"ended",sizeof(data.eventName));
        event.data = (void*)&data;
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
        memset(data.eventName,0,sizeof(data.eventName));
        strncpy(data.eventName,"return",sizeof(data.eventName));
        event.data = (void*)&data;
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
#endif // #if CC_ENABLE_SCRIPT_BINDING
}

void EditBoxImplWin::closeKeyboard()
{

}

void EditBoxImplWin::onEnter(void)
{

}

}

NS_CC_END

#endif /* (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) */
