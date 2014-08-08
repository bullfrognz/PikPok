
//
//  File Name   :   D10UiText.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "D10UiText.h"

// Local Includes
#include "Framework/Debug.h"

// Library Includes
#include "Framework/D10/D10Renderer.h"

// Static Initialisers

// Implementation

CD10UiText::CD10UiText(CD10Renderer& _rD10Renderer)
: m_rD10Renderer(_rD10Renderer)
, m_vColour(0.0f, 0.0f, 0.0f, 1.0f)
, m_pD10Font(0)
, m_eHAlign(TEXT_H_ALIGN_LEFT)
, m_eVAlign(TEXT_V_ALIGN_TOP)
, m_uiFontSize(18)
, m_uiDrawFlags(0)
, m_bRecreate(true)
{
	ZeroMemory(&m_tContainer, sizeof(RECT));
}

CD10UiText::~CD10UiText()
{
	FW_RELEASE(m_pD10Font);
}

void CD10UiText::Render()
{
	if (m_Text.length() == 0)
		return;

	if (m_bRecreate)
	{
		VALIDATE_V(RecreateD10Font());
	}

	TRect tContainer = CUiElement::Bounds();

	HRESULT hResult = m_pD10Font->DrawText(0, m_Text.c_str(), -1, reinterpret_cast<RECT*>(&tContainer), DT_NOCLIP, reinterpret_cast<D3DXCOLOR&>(m_vColour));

	if (FAILED(hResult))
	{
		DEBUG_ERROR("Failed to render text.");
	}
}

void CD10UiText::ClearText()
{
	m_Text.clear();
}

void CD10UiText::Text(c_char* _kcpText)
{
	m_Text = _kcpText;
}

void CD10UiText::TextHAlign(ETextHAlign _eAlign)
{
	m_eHAlign = _eAlign;
}

void CD10UiText::TextVAlign(ETextVAlign _eAlign)
{
	m_eVAlign = _eAlign;
}

void CD10UiText::FontSize(ushort _usSize)
{
	m_uiFontSize = _usSize;
	m_bRecreate  = true;
}

void CD10UiText::FontColour(const TColour& _kvrColour)
{
	m_vColour = _kvrColour;
}

c_char*	CD10UiText::Text() const
{
	return (m_Text.c_str());
}

IUiText::ETextHAlign CD10UiText::TextHAlign() const
{
	return (m_eHAlign);
}

IUiText::ETextVAlign CD10UiText::TextVAlign() const
{
	return (m_eVAlign);
}

ushort CD10UiText::FontSize() const
{
	return (m_uiFontSize);
}

const TColour& CD10UiText::FontColour() const
{
	return (m_vColour);
}

D3DX10_FONT_DESC CD10UiText::CompileFontDesc() const
{
	D3DX10_FONT_DESC tD10FontDesc;
	FW_MEMZERO(&tD10FontDesc, sizeof(D3DX10_FONT_DESC));

	tD10FontDesc.Height				= (int)m_uiFontSize;
	tD10FontDesc.Width				= 0;
	tD10FontDesc.Weight				= 0;
	tD10FontDesc.MipLevels			= 1;
	tD10FontDesc.Italic				= false;
	tD10FontDesc.CharSet			= DEFAULT_CHARSET;
	tD10FontDesc.OutputPrecision	= OUT_DEFAULT_PRECIS;
	tD10FontDesc.PitchAndFamily		= DEFAULT_PITCH | FF_DONTCARE;
	tD10FontDesc.Quality			= DEFAULT_QUALITY;

	strcpy_s(tD10FontDesc.FaceName, "Times New Roman");

	return (tD10FontDesc);

}

bool CD10UiText::RecreateD10Font()
{
	HRESULT hResult = D3DX10CreateFontIndirect(m_rD10Renderer.GetDevice(), &CompileFontDesc(), &m_pD10Font);

	if (FAILED(hResult))
	{
		DEBUG_ERROR("Failed to create font indirect");
		return (false);
	}

	m_bRecreate = false;

	return (true);
}