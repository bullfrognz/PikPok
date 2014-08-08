
//
//  File Name   :   D10UiText.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __D10UiText_H__
#define __D10UiText_H__

// Dependent Includes
#include "Library/UiText.h"

// Local Includes
#include "Library/Colour.h"
#include "Library/Rect.h"
#include "Defines/Types.h"

// Library Includes
#include <DirectX/D3DX10.h>
#include <string>

// Prototypes
class CD10Renderer;

class CD10UiText : public IUiText
{

	// Member Types
public:

	// Member Functions
public:

			 CD10UiText(CD10Renderer& _rD10Renderer);
	virtual ~CD10UiText();

	virtual void Render();

	virtual void ClearText();

	virtual void Text(c_char* _kcpText);
	virtual	void TextHAlign(ETextHAlign _eAlign);
	virtual	void TextVAlign(ETextVAlign _eAlign);
	virtual void FontSize(ushort _usSize);
	virtual void FontColour(const TColour& _kvrColour);

	virtual c_char*			Text()			const;
	virtual	ETextHAlign		TextHAlign()	const;
	virtual	ETextVAlign		TextVAlign()	const;
	virtual ushort			FontSize()		const;
	virtual const TColour&	FontColour()	const;

protected:

	D3DX10_FONT_DESC CompileFontDesc() const;

private:

	bool RecreateD10Font();

	CD10UiText(const CD10UiText& _krD10UiText);
	CD10UiText& operator = (const CD10UiText& _krD10UiText);

	// Member Variables
protected:

private:

	CD10Renderer&	m_rD10Renderer;

	TColour			m_vColour;
	TRect			m_tContainer;

	ID3DX10Font*	m_pD10Font;
	std::string		m_Text;

	ETextHAlign		m_eHAlign;
	ETextVAlign		m_eVAlign;

	uint			m_uiFontSize;
	uint			m_uiDrawFlags;

	bool			m_bRecreate;

};

#endif //__D10UiText_H__