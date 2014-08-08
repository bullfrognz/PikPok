
//
//  File Name   :   Colour.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Colour_H__
#define __Colour_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes

struct TColour
{

// Member Types

// Member Functions

	 TColour();
	 TColour(float _fRed, float _fGreen, float _fBlue);
	 TColour(float _fRed, float _fGreen, float _fBlue, float _fOpacity);
	~TColour();

	TColour& Inverse(TColour& _vrColour);
	TColour  Inverse();

	float& operator [] (uchar _ucIndex);

	void Set(float _fRed, float _fGreen, float _fBlue);
	void Set(float _fRed, float _fGreen, float _fBlue, float _fOpacity);

// Member Variables

	float fR;
	float fG;
	float fB;
	float fO;

};

#endif //__Colour_H__