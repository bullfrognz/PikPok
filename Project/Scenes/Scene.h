
//
//  File Name   :   Scene.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Scene_H__
#define __Scene_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes

class IScene
{

	// Member Types
public:

	// Member Functions
public:

			 IScene() {};
	virtual ~IScene() {};

	virtual bool Initialise(void* _kpInitData) = 0;

protected:

private:

	IScene(const IScene& _krScene);
	IScene& operator = (const IScene& _krScene);

	// Member Variables
protected:

private:

};

#endif //__Scene_H__