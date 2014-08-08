
//
//  File Name   :   UiText.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __UiText_H__
#define __UiText_H__

// Dependent Includes
#include "Library/UiElement.h"
#include "Library/Renderable.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
struct TColour;

class IUiText : public CUiElement, public CRenderable
{

	// Member Types
public:

	enum EFontFamily
	{
		INVALID_FONT_FAMILY = -1,

		FONT_FAMILY_ARIAL,

		MAX_FONT_FAMILY
	};

	enum ETextHAlign
	{
		INVALID_TEXT_HORIZONTAL_ALIGN = -1,

		TEXT_H_ALIGN_LEFT,
		TEXT_H_ALIGN_CENTER,
		TEXT_H_ALIGN_RIGHT,

		MAX_TEXT_HORIZONTAL_ALIGN
	};

	enum ETextVAlign
	{
		INVALID_TEXT_VERTICAL_ALIGN = -1,

		TEXT_V_ALIGN_TOP,
		TEXT_V_ALIGN_CENTER,
		TEXT_V_ALIGN_BOTTOM,

		MAX_TEXT_VERTICAL_ALIGN
	};

	// Member Functions
public:

			 IUiText() {}
			 IUiText(CUiElement* _pParent) : CUiElement(_pParent) {}
	virtual ~IUiText() {}

	virtual void Render() = 0;

	virtual void ClearText() = 0;

	virtual void Text(c_char* _kcpText)					= 0;
	virtual	void TextHAlign(ETextHAlign _eAlign)		= 0;
	virtual	void TextVAlign(ETextVAlign _eAlign)		= 0;
	virtual void FontSize(ushort _usSize)				= 0;
	virtual void FontColour(const TColour& _kvrColour)	= 0;

	virtual c_char*			Text()			const = 0;
	virtual	ETextHAlign		TextHAlign()	const = 0;
	virtual	ETextVAlign		TextVAlign()	const = 0;
	virtual ushort			FontSize()		const = 0;
	virtual const TColour&	FontColour()	const = 0;

protected:

private:

	// Member Variables
protected:

private:

};

#endif //__UiText_H__