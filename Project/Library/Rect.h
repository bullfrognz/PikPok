
//
//  File Name   :   Rect.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Rect_H__
#define __Rect_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
struct TVector2;

struct TRect
{

// Member Types

// Member Functions

	 TRect();
	 TRect(const TRect& _krRect);
	 TRect(float _fTop, float _fLeft, float _fWidth, float _fHeight);
	~TRect();

	float	LeftFloor()		const;
	float	RightFloor()	const;
	float	TopFloor()		const;
	float	BottomFloor()	const;
	float	WidthFloor()	const;
	float	HeightFloor()	const;

	int		LeftFloorI()	const;
	int		RightFloorI()	const;
	int		TopFloorI()		const;
	int		BottomFloorI()	const;
	int		WidthFloorI()	const;
	int		HeightFloorI()	const;

	void	Margins(float _fTop, float _fLeft);
	void	Dimentions(float _fWidth, float _fHeight);

	float	Right()		const;
	float	Bottom()	const;
	float	CenterX()	const;
	float	CenterY()	const;

	bool	IsIntersecting(const TRect& _kvrRect)		const;
	bool	IsIntersecting(const TVector2& _kvrVector2) const;

	TRect&	operator += (const TRect& _krRight);
	TRect&	operator -= (const TRect& _krRight);
	TRect	operator +  (const TRect& _krRight);
	TRect	operator -  (const TRect& _krRight);

// Member Variables

	float fTop;
	float fLeft;
	float fWidth;
	float fHeight;

};

#endif //__Rect_H__