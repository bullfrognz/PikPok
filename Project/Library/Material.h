
//
//  File Name   :   Material.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Material_H__
#define __Material_H__

// Dependent Includes

// Local Includes
#include "Library/Colour.h"
#include "Library/List.h"
#include "Defines/Types.h"

// Library Includes

// Prototypes
class ITexture;
class IShader;

struct TMaterial
{

// Member Types

// Member Functions

	 TMaterial() {};
	~TMaterial() {};

// Member Variables

	TColour		tDiffuse;
	TColour		tAmbient;
	TColour		tSpecular;

	IShader*	pShader;

	float		fSpecularPower;

	CList<ITexture*> aTextures;

};

#endif //__Material_H__