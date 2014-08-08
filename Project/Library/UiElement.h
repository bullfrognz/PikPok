
//
//  File Name   :   UiElement.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __UiElement_H__
#define __UiElement_H__

// Dependent Includes
#include "Library/Subject.h"
#include "Library/Observer.h"

// Local Includes
#include "Library/Rect.h"
#include "Library/Vector2.h"
#include "Library/Vector4.h"
#include "Defines/Types.h"

// Library Includes

// Prototypes

class CUiElement : public CSubject<CUiElement>, public IObserver<CUiElement>
{

	// Member Types
public:

	enum ESubject
	{
		SUBJECT_DELETE,

		SUBJECT_MARGIN_X_CHANGE,
		SUBJECT_MARGIN_Y_CHANGE,
		SUBJECT_WIDTH_CHANGE,
		SUBJECT_HEIGHT_CHANGE,
		SUBJECT_PADDING_X_CHANGE,
		SUBJECT_PADDING_Y_CHANGE,

		MAX_SUBJECT
	};

	enum EAlign
	{
		INVALID_ALIGN = -1,

		VERTICAL_ALIGN_TOP,
		HORIZONTAL_ALIGN_LEFT = VERTICAL_ALIGN_TOP,
		HORIZONTAL_ALIGN_CENTER,
		VERTICAL_ALIGN_CENTER,
		VERTICAL_ALIGN_BOTTOM,
		HORIZONTAL_ALIGN_RIGHT = VERTICAL_ALIGN_BOTTOM,
	};

	enum ESetting
	{
		SETTING_MARGIN_X_SCALE_UP = 1 << 0,
		SETTING_MARGIN_X_SCALE_DOWN = 1 << 1,

		SETTING_MARGIN_Y_SCALE_UP = 1 << 2,
		SETTING_MARGIN_Y_SCALE_DOWN = 1 << 3,

		SETTING_WIDTH_RATIO = 1 << 4,
		SETTING_HEIGHT_RATIO = 1 << 7,

		SETTING_DIMENTION_RATIO = SETTING_WIDTH_RATIO | SETTING_HEIGHT_RATIO
	};

	enum ESide
	{
		SIDE_LEFT,
		SIDE_RIGHT,
		SIDE_TOP,
		SIDE_BOTTOM
	};

	// Member Functions
public:

			 CUiElement();
			 CUiElement(CUiElement* _pParent);
	virtual ~CUiElement();

	virtual void Parent(CUiElement* _pParent);
	virtual void Setting(ESetting _eSetting, bool _bEnabled);

	virtual void Notify(CUiElement& _rSender, short _sSubject, CEventArgs& _crArgs);

	void MarginX(float _fX);
	void MarginY(float _fY);
	void Margin(float _fX, float _fY);
	void Width(float _fWidth);
	void Height(float _fHeight);
	void Dimentions(float _fWidth, float _fHeight);
	void PaddingLeft(float _fPadding);
	void PaddingTop(float _fPadding);
	void PaddingRight(float _fPadding);
	void PaddingBottom(float _fPadding);
	void Padding(float _fPadding);
	void Padding(float _fLeft, float _fTop, float _fRight, float _fBottom);
	void AlignHorizontal(EAlign _eAlign);
	void AlignVertical(EAlign _eAlign);
	void Align(EAlign _eHorizontalAlign, EAlign _eVerticalAlign);

	const TRect&	Bounds()			const;
	const TVector2&	Margin()			const;
	const TVector2&	MarginActual()		const;
	float			Width()				const;
	float			Height()			const;
	float			WidthActual()		const;
	float			HeightActual()		const;
	float			PaddingLeft()		const;
	float			PaddingTop()		const;
	float			PaddingRight()		const;
	float			PaddingBottom()		const;
	CUiElement*		Parent()			const;
	EAlign			AlignHorizontal()	const;
	EAlign			AlignVertical()		const;

	bool IsSettingEnabled(ESetting _eSetting) const;

protected:

	virtual void HandleSettingChange(ESetting _eSetting);
	virtual void UpdateWidth();
	virtual void UpdateHeight();
	virtual void UpdateMarginX();
	virtual void UpdateMarginY();
	virtual void UpdateBoundsX();
	virtual void UpdateBoundsY();

private:

	void Construct();

	CUiElement(const CUiElement& _krUiElement);
	CUiElement& operator = (const CUiElement& _krUiElement);

	// Member Variables
protected:

private:

	TVector4 m_vPadding;
	TVector2 m_vMargin;
	TVector2 m_vMarginActual;
	TVector2 m_vDimentions;
	TVector2 m_vDimentionsActual;
	TRect m_vBounds;

	CUiElement* m_pParent;

	EAlign m_eAlignHorizontal;
	EAlign m_eAlignVertical;

	uint m_uiSettings;

};

#endif //__UiElement_H__