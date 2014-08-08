
//
//  File Name   :   Colour.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Colour.h"

// Local Includes
#include "Framework/Debug.h"

// Library Includes

// Static Initialisers

// Implementation

TColour::TColour()
: fR(1.0f)
, fG(1.0f)
, fB(1.0f)
, fO(1.0f)
{
	// Empty
}

TColour::TColour(float _fRed, float _fGreen, float _fBlue)
: fR(_fRed)
, fG(_fGreen)
, fB(_fBlue)
, fO(1.0f)
{

}

TColour::TColour(float _fRed, float _fGreen, float _fBlue, float _fOpacity)
: fR(_fRed)
, fG(_fGreen)
, fB(_fBlue)
, fO(_fOpacity)
{
}

TColour::~TColour()
{
	// Empty
}

TColour& TColour::Inverse(TColour& _vrColour)
{
	_vrColour.Set(1.0f - fR,
				  1.0f - fG,
				  1.0f - fB,
				  fO);

	return (_vrColour);
}

TColour TColour::Inverse()
{
	return (TColour(1.0f - fR,
					1.0f - fG,
					1.0f - fB,
					fO));
}

float& TColour::operator [] (uchar _ucIndex)
{
	switch (_ucIndex)
	{
	case 0: return (fR);
	case 1: return (fG);
	case 2: return (fB);
	case 3: return (fO);

	default:
		DEBUG_ERROR("Invalid index for TVector3 container. Index(%d)", _ucIndex);
		break;
	}

	static float fInvalid = -1.0f;
	return (fInvalid);
}

void TColour::Set(float _fRed, float _fGreen, float _fBlue)
{
	fR	= _fRed;
	fG	= _fGreen;
	fB	= _fBlue;
}

void TColour::Set(float _fRed, float _fGreen, float _fBlue, float _fOpacity)
{
	fR	= _fRed;
	fG	= _fGreen;
	fB	= _fBlue;
	fO	= _fOpacity;
}