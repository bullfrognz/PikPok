
//
//  File Name   :   Clock.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Clock_H__
#define __Clock_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes

class IClock
{

#define CLOCK	APPLICATION.Clock()

	// Member Types
public:

	// Member Functions
public:

			 IClock() {};
	virtual ~IClock() {};


	virtual bool Initialise()			= 0;
	virtual bool ProcessFrame()	= 0;


	virtual float Deltatick()		const = 0;
	virtual uint  FramesPerSecond()	const = 0;

protected:

private:

	IClock(const IClock& _krClock);
	IClock& operator = (const IClock& _krClock);

	// Member Variables
protected:

private:

};

#endif //__Clock_H__