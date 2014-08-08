
//
//  File Name   :   MathUtilities.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "MathUtilities.h"

// Local Includes
#include "Framework/Debug.h"

// Library Includes

// Static Initialisers

// Implementation

namespace MathUtilities
{

	float DegreesToRadians(float _fDegrees)
	{
		float fRadians = _fDegrees;
		fRadians *= (kfPi / 180.0f);

		return (fRadians);
	}

	float RadiansToDegrees(float _fRadians)
	{
		float fDegrees = _fRadians;
		fDegrees *= (180.0f / kfPi);

		return (fDegrees);
	}

}