
//
//  File Name   :   WinClock.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __WinClock_H__
#define __WinClock_H__

// Dependent Includes
#include "Framework/Clock.h"

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes

class CWinClock : public IClock
{

	// Member Types
public:

	// Member Functions
public:

			 CWinClock();
	virtual ~CWinClock();

	virtual bool Initialise();
	virtual bool ProcessFrame();

	virtual float Deltatick()		const;
	virtual uint  FramesPerSecond()	const;

protected:

private:

	void UpdateDeltatick();
	void UpdateFps();

	CWinClock(const CWinClock& _krWinClock);
	CWinClock& operator = (const CWinClock& _krWinClock);

	// Member Variables
protected:

private:

	__int64 m_i64CurrentTime;
	__int64 m_i64LastTime;
	__int64 m_i64CountsPerSecond;

	double m_dDeltatick;

	float m_fPerfCounterTimeScale;
	float m_fTimeElasped;

	uint m_uiFrameCount;
	uint m_uiFramesPerSecond;

};

#endif //__WinClock_H__