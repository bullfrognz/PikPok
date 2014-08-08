
//
//  File Name   :   UiElement.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "UiElement.h"

// Local Includes
#include "Framework/Debug.h"
#include "Framework/Window.h"

// Library Includes

// Static Initialisers

// Implementation

CUiElement::CUiElement()
{
	Construct();
}

CUiElement::CUiElement(CUiElement* _pParent)
{
	Construct();
	Parent(_pParent);
}

CUiElement::~CUiElement()
{
	Parent(null);
}

void CUiElement::Parent(CUiElement* _pParent)
{
	if (_pParent != null)
	{
		_pParent->Subscribe(this, SUBJECT_DELETE);
		_pParent->Subscribe(this, SUBJECT_MARGIN_X_CHANGE);
		_pParent->Subscribe(this, SUBJECT_MARGIN_Y_CHANGE);
		_pParent->Subscribe(this, SUBJECT_WIDTH_CHANGE);
		_pParent->Subscribe(this, SUBJECT_HEIGHT_CHANGE);
		_pParent->Subscribe(this, SUBJECT_PADDING_X_CHANGE);
		_pParent->Subscribe(this, SUBJECT_PADDING_Y_CHANGE);

		UpdateMarginX();
		UpdateMarginY();
	}
	else if (m_pParent != null)
	{
		m_pParent->UnsubscribeAll(this);
	}

	m_pParent = _pParent;
}

void CUiElement::Setting(ESetting _eSetting, bool _bEnabled)
{
	// Turn off setting
	if (!_bEnabled)
		m_uiSettings &= ~(_eSetting);

	// Turn on setting
	else
		m_uiSettings |= _eSetting;

	HandleSettingChange(_eSetting);
}

void CUiElement::Notify(CUiElement& _rSender, short _sSubject, CEventArgs& _crArgs)
{
	switch (_sSubject)
	{
	case SUBJECT_DELETE:
		if (&_rSender == m_pParent)
			Parent(null);
		break;

	case SUBJECT_MARGIN_X_CHANGE:	UpdateMarginX();	break;
	case SUBJECT_MARGIN_Y_CHANGE:	UpdateMarginY();	break;
	case SUBJECT_WIDTH_CHANGE:		UpdateWidth();		break;
	case SUBJECT_HEIGHT_CHANGE:		UpdateHeight();		break;
	case SUBJECT_PADDING_X_CHANGE:	UpdateBoundsX();	break;
	case SUBJECT_PADDING_Y_CHANGE:	UpdateBoundsY();	break;
	}
}

void CUiElement::MarginX(float _fX)
{
	m_vMargin.x = _fX;

	UpdateMarginX();
}

void CUiElement::MarginY(float _fY)
{
	m_vMargin.y = _fY;

	UpdateMarginY();
}

void CUiElement::Margin(float _fX, float _fY)
{
	MarginX(_fX);
	MarginY(_fY);
}

void CUiElement::Width(float _fWidth)
{
	m_vDimentions.x = _fWidth;
	UpdateWidth();
}

void CUiElement::Height(float _fHeight)
{
	m_vDimentions.y = _fHeight;
	UpdateHeight();
}

void CUiElement::Dimentions(float _fWidth, float _fHeight)
{
	Width(_fWidth);
	Height(_fHeight);
}

void CUiElement::PaddingLeft(float _fPadding)
{
	m_vPadding.x = _fPadding;
	UpdateBoundsX();
}

void CUiElement::PaddingTop(float _fPadding)
{
	m_vPadding.y = _fPadding;
	UpdateBoundsY();
}

void CUiElement::PaddingRight(float _fPadding)
{
	m_vPadding.z = _fPadding;
	UpdateBoundsX();
}

void CUiElement::PaddingBottom(float _fPadding)
{
	m_vPadding.w = _fPadding;
	UpdateBoundsY();
}

void CUiElement::Padding(float _fPadding)
{
	m_vPadding.Set(_fPadding, _fPadding, _fPadding, _fPadding);
	UpdateBoundsX();
	UpdateBoundsY();
}

void CUiElement::Padding(float _fLeft, float _fTop, float _fRight, float _fBottom)
{
	m_vPadding.Set(_fLeft, _fTop, _fRight, _fBottom);
	UpdateBoundsX();
	UpdateBoundsY();
}

void CUiElement::AlignHorizontal(EAlign _eAlign)
{
	m_eAlignHorizontal = _eAlign;
	UpdateMarginX();
}

void CUiElement::AlignVertical(EAlign _eAlign)
{
	m_eAlignVertical = _eAlign;
	UpdateMarginY();
}

void CUiElement::Align(EAlign _eHorizontalAlign, EAlign _eVerticalAlign)
{
	AlignHorizontal(_eHorizontalAlign);
	AlignVertical(_eVerticalAlign);
}

const TRect& CUiElement::Bounds() const
{
	return (m_vBounds);
}

const TVector2& CUiElement::Margin() const
{
	return (m_vMargin);
}

const TVector2& CUiElement::MarginActual() const
{
	return (m_vMarginActual);
}

float CUiElement::Width() const
{
	return (m_vDimentions.x);
}

float CUiElement::Height() const
{
	return (m_vDimentions.y);
}

float CUiElement::WidthActual() const
{
	return (m_vBounds.fWidth);
}

float CUiElement::HeightActual() const
{
	return (m_vBounds.fHeight);
}

float CUiElement::PaddingLeft() const
{
	return (m_vPadding.x);
}

float CUiElement::PaddingTop() const
{
	return (m_vPadding.y);
}

float CUiElement::PaddingRight() const
{
	return (m_vPadding.z);
}

float CUiElement::PaddingBottom() const
{
	return (m_vPadding.w);
}

CUiElement* CUiElement::Parent() const
{
	return (m_pParent);
}

CUiElement::EAlign CUiElement::AlignHorizontal() const
{
	return (m_eAlignHorizontal);
}

CUiElement::EAlign CUiElement::AlignVertical() const
{
	return (m_eAlignVertical);
}

bool CUiElement::IsSettingEnabled(ESetting _eSetting) const
{
	return ((m_uiSettings & _eSetting) > 0);
}

void CUiElement::HandleSettingChange(ESetting _eSetting)
{
	if (SETTING_DIMENTION_RATIO)
	{
		UpdateWidth();	// Will also update margin x
		UpdateHeight(); // Will also update margin y
	}
	else
	{
		switch (_eSetting)
		{
		case SETTING_MARGIN_X_SCALE_UP:	// Fall through
		case SETTING_MARGIN_X_SCALE_DOWN:
			UpdateMarginX();
			break;

		case SETTING_MARGIN_Y_SCALE_UP:	// Fall through
		case SETTING_MARGIN_Y_SCALE_DOWN:
			UpdateMarginY();
			break;

		case SETTING_WIDTH_RATIO:		// Fall through
			UpdateWidth();
			break;

		case SETTING_HEIGHT_RATIO:		// Fall through
			UpdateHeight();
			break;
		}
	}
}

void CUiElement::UpdateWidth()
{
	float fWidth = m_vDimentions.x;

	if (IsSettingEnabled(SETTING_WIDTH_RATIO))
	{
		const float kfWidthRatio = fWidth;

		// Base ratio of parents width
		if (m_pParent != null)
			fWidth = m_pParent->Bounds().fWidth * kfWidthRatio;

		// Base ratio on back buffer width
		else
			fWidth = WINDOW.Width() * kfWidthRatio;
	}

	m_vDimentionsActual.x = fWidth;

	UpdateMarginX();

	NotifySubscribers(SUBJECT_WIDTH_CHANGE);
}

void CUiElement::UpdateHeight()
{
	float fHeight = m_vDimentions.y;

	if (IsSettingEnabled(SETTING_HEIGHT_RATIO))
	{
		const float kfHeightRatio = fHeight;

		// Base ratio of parents height
		if (m_pParent != null)
			fHeight = m_pParent->Bounds().fHeight * kfHeightRatio;

		// Base ratio on back buffer height
		else
			fHeight = WINDOW.Height() * kfHeightRatio;
	}

	m_vDimentionsActual.y = fHeight;

	UpdateMarginY();

	NotifySubscribers(SUBJECT_HEIGHT_CHANGE);
}

void CUiElement::UpdateMarginX()
{
	float fMarginX = Margin().x;

	m_vMarginActual.x = fMarginX;

	UpdateBoundsX();

	NotifySubscribers(SUBJECT_MARGIN_X_CHANGE);
}

void CUiElement::UpdateMarginY()
{
	float fMarginY = m_vMargin.y;

	m_vMarginActual.y = fMarginY;

	UpdateBoundsY();

	NotifySubscribers(SUBJECT_MARGIN_Y_CHANGE);
}

void CUiElement::UpdateBoundsX()
{
	m_vBounds.fWidth = m_vDimentionsActual.x;

	float fLeft = 0.0f;

	// Position is relative to parent's margin + self margin
	if (m_pParent != null)
	{
		switch (m_eAlignHorizontal)
		{
		case HORIZONTAL_ALIGN_LEFT:
			fLeft = m_pParent->Bounds().fLeft;
			fLeft += m_pParent->PaddingLeft();
			fLeft += m_vMarginActual.x;
			break;

		case HORIZONTAL_ALIGN_RIGHT:
			fLeft = m_pParent->Bounds().Right();
			fLeft -= m_pParent->PaddingRight();
			fLeft -= m_vMarginActual.x;
			fLeft -= m_vBounds.fWidth;
			break;

		case HORIZONTAL_ALIGN_CENTER:
			fLeft = m_pParent->Bounds().CenterX();
			fLeft += m_vMarginActual.x;
			fLeft -= m_vBounds.fWidth / 2.0f;
			break;
		}
	}
	else
	{
		switch (m_eAlignHorizontal)
		{
		case HORIZONTAL_ALIGN_LEFT:
			fLeft = m_vMarginActual.x;
			break;

		case HORIZONTAL_ALIGN_RIGHT:
			fLeft = WINDOW.Width();
			fLeft -= m_vMarginActual.x;
			fLeft -= m_vBounds.fWidth;
			break;

		case HORIZONTAL_ALIGN_CENTER:
			fLeft = WINDOW.Width() / 2.0f;
			fLeft += m_vMarginActual.x;
			fLeft -= m_vBounds.fWidth/ 2.0f;
			break;
		}
	}

	m_vBounds.fLeft = fLeft;

	NotifySubscribers(SUBJECT_PADDING_X_CHANGE);
}

void CUiElement::UpdateBoundsY()
{
	m_vBounds.fHeight = m_vDimentionsActual.y;

	float fTop = 0.0f;

	// Position is relative to parent's margin + self margin
	if (m_pParent != null)
	{
		switch (m_eAlignVertical)
		{
		case VERTICAL_ALIGN_TOP:
			fTop = m_pParent->Bounds().fTop;
			fTop += m_pParent->PaddingTop();
			fTop += m_vMarginActual.y;
			break;

		case VERTICAL_ALIGN_BOTTOM:
			fTop = m_pParent->Bounds().Bottom();
			fTop -= m_pParent->PaddingBottom();
			fTop -= m_vMarginActual.y;
			fTop -= m_vBounds.fHeight;
			break;

		case VERTICAL_ALIGN_CENTER:
			fTop = m_pParent->Bounds().CenterY();
			fTop -= m_vMarginActual.y;
			fTop -= m_vBounds.fHeight / 2.0f;
			break;
		}
	}
	else
	{
		switch (m_eAlignVertical)
		{
		case VERTICAL_ALIGN_TOP:
			fTop = m_vMarginActual.y;
			break;

		case VERTICAL_ALIGN_BOTTOM:
			fTop = WINDOW.Height();
			fTop -= m_vMarginActual.y;
			fTop -= m_vBounds.fHeight;
			break;

		case VERTICAL_ALIGN_CENTER:
			fTop = WINDOW.Height() / 2.0f;
			fTop -= m_vMarginActual.y;
			fTop -= m_vBounds.fHeight / 2.0f;
			break;
		}
	}

	m_vBounds.fTop = fTop;

	NotifySubscribers(SUBJECT_PADDING_Y_CHANGE);
}

void CUiElement::Construct()
{
	m_pParent = null;
	m_eAlignHorizontal = HORIZONTAL_ALIGN_LEFT;
	m_eAlignVertical = VERTICAL_ALIGN_TOP;
	m_uiSettings = 0;


	Dimentions(1.0f, 1.0f);
}