
//
//  File Name   :   D10HlslLoader.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __D10HlslLoader_H__
#define __D10HlslLoader_H__

// Dependent Includes
#include "Library/HlslLoader.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes

class CD10HlslLoader : public IHlslLoader
{

	// Member Types
public:

	// Member Functions
public:

			 CD10HlslLoader();
	virtual ~CD10HlslLoader();

	bool Initialise();

protected:

private:

	CD10HlslLoader(const CD10HlslLoader& _krD10HlslLoader);
	CD10HlslLoader& operator = (const CD10HlslLoader& _krD10HlslLoader);

	// Member Variables
protected:

private:

};

#endif //__D10HlslLoader_H__