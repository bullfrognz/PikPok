
//
//  File Name   :   Rect.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Rect.h"

// Local Includes
#include "Framework/Debug.h"
#include "Library/Vector2.h"

// Library Includes

// Static Initialisers

// Implementation

TRect::TRect()
: fTop(0)
, fLeft(0)
, fWidth(0)
, fHeight(0)
{
	// Empty
}

TRect::TRect(const TRect& _krRect)
: fTop(_krRect.fTop)
, fLeft(_krRect.fLeft)
, fWidth(_krRect.fWidth)
, fHeight(_krRect.fHeight)
{
	// Empty
}

TRect::TRect(float _fTop, float _fLeft, float _fWidth, float _fHeight)
: fTop(_fTop)
, fLeft(_fLeft)
, fWidth(_fWidth)
, fHeight(_fHeight)
{
	// Empty
}

TRect::~TRect()
{
	// Empty
}

float TRect::LeftFloor() const
{
	return (floorf(fLeft + 0.5f));
}

float TRect::RightFloor() const
{
	return (floorf(Right() + 0.5f));
}

float TRect::TopFloor() const
{
	return (floorf(fTop + 0.5f));
}

float TRect::BottomFloor() const
{
	return (floorf(Bottom() + 0.5f));
}

float TRect::WidthFloor() const
{
	return (floorf(fWidth + 0.5f));
}

float TRect::HeightFloor() const
{
	return (floorf(fHeight + 0.5f));
}

int TRect::LeftFloorI() const
{
	return (static_cast<int>(fLeft + 0.5f));
}

int TRect::RightFloorI() const
{
	return (static_cast<int>(Right() + 0.5f));
}

int TRect::TopFloorI() const
{
	return (static_cast<int>(fTop + 0.5f));
}

int TRect::BottomFloorI() const
{
	return (static_cast<int>(Bottom() + 0.5f));
}

int TRect::WidthFloorI() const
{
	return (static_cast<int>(fWidth + 0.5f));
}

int TRect::HeightFloorI() const
{
	return (static_cast<int>(fHeight + 0.5f));
}

bool TRect::IsIntersecting(const TRect& _kvrRect) const
{
	// TODO: This code
	return (false);
}

bool TRect::IsIntersecting(const TVector2& _kvrVector2) const
{
	if (_kvrVector2.x > fLeft &&
		_kvrVector2.x < Right() &&
		_kvrVector2.y > fTop		&&
		_kvrVector2.y < Bottom())
	{
		return (true);
	}

	return (false);
}

void TRect::Margins(float _fTop, float _fLeft)
{
	fTop = _fTop;
	fLeft = _fLeft;
}

void TRect::Dimentions(float _fWidth, float _fHeight)
{
	fWidth = _fWidth;
	fHeight = _fHeight;
}

float TRect::Right() const
{
	return (fLeft + fWidth);
}

float TRect::Bottom() const
{
	return (fTop + fHeight);
}

float TRect::CenterX() const
{
	return (fLeft + fWidth / 2.0f);
}

float TRect::CenterY() const
{
	return (fTop + fHeight / 2.0f);
}

TRect& TRect::operator += (const TRect& _krRight)
{
	fTop += _krRight.fTop;
	fLeft += _krRight.fLeft;
	fWidth += _krRight.fWidth;
	fHeight += _krRight.fHeight;

	return (*this);
}

TRect& TRect::operator -= (const TRect& _krRight)
{
	fTop		-= _krRight.fTop;
	fLeft		-= _krRight.fLeft;
	fWidth	-= _krRight.fWidth;
	fHeight	-= _krRight.fHeight;

	return (*this);
}

TRect TRect::operator +  (const TRect& _krRight)
{
	return (TRect(fTop + _krRight.fTop,
				  fLeft + _krRight.fLeft,
				  fWidth + _krRight.fWidth,
				  fHeight + _krRight.fHeight));
}

TRect TRect::operator -  (const TRect& _krRight)
{
	return (TRect(fTop - _krRight.fTop,
				  fLeft - _krRight.fLeft,
				  fWidth - _krRight.fWidth,
				  fHeight - _krRight.fHeight));
}