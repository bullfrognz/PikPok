
//
//  File Name   :   MathUtilities.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __MathUtilities_H__
#define __MathUtilities_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
struct TViewMatrix;
struct TProjectionMatrix;
struct TVector3;

namespace MathUtilities
{

	const float kfPi = 3.14159265359f;

	template <class T>
	T Max(T _Value1, T _Value2)
	{
		return (_Value1 > _Value2 ? _Value1 : _Value2);
	}

	template <class T>
	T Min(T _Value1, T _Value2)
	{
		return (_Value1 < _Value2 ? _Value1 : _Value2);
	}

	template <class T>
	void Offset2(T _OriginX, T _OriginY,
				 T _PositionX, T _PositionY,
				 T& _rOffsetX, T& _rOffsetY)
	{
		_rOffsetX = _PositionX - _OriginX;
		_rOffsetY = _PositionY - _OriginY;
	}

	template<class T>
	T Clamp(T _Value, T _Min, T _Max)
	{
		if (_Value > _Max) 
			_Value = _Max; 
		
		else if (_Value < _Min) 
			 _Value = _Min;

		return (_Value);
	}

	template<class T>
	T AddClamp(T _Value, T _Addition, T _ClampTo)
	{
		_Value += _Addition;

		if (_Addition > 0)
		{
			if (_Value > _ClampTo)
			{
				_Value = _ClampTo;
			}
		}

		else if (_Addition < 0)
		{
			if (_Value < _ClampTo)
			{
				_Value = _ClampTo;
			}
		}

		return (_Value);
	}

	float DegreesToRadians(float _fDegrees);
	float RadiansToDegrees(float _fRadians);

	TVector3 WindowToWorldSpace(const TViewMatrix& _kxrView, const TProjectionMatrix& _kxrProj, const TVector3& _kvrPoint);

}

#endif //__MathUtilities_H__