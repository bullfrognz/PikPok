
//
//  File Name   :   CWinClock.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "WinClock.h"

// Local Includes
#include "Framework/Debug.h"
#include "Library/Processable.h"

// Library Includes
#include <Windows.h>

// Static Initialisers

// Implementation

CWinClock::CWinClock()
: m_i64CurrentTime(0)
, m_i64LastTime(0)
, m_i64CountsPerSecond(0)
, m_dDeltatick(0)
, m_fPerfCounterTimeScale(0)
, m_fTimeElasped(0)
, m_uiFrameCount(0)
, m_uiFramesPerSecond(0)
{
	// Empty
}

CWinClock::~CWinClock()
{
	// Empty
}

bool CWinClock::Initialise()
{
	bool bQueryReturn = (QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_i64CountsPerSecond)) != 0);

	if (!bQueryReturn)
	{
		DEBUG_ERROR("Could not query the performance frequency from the CPU. Feature may not be supported.");
		return (false);
	}

	m_fPerfCounterTimeScale = 1.0f / static_cast<float>(m_i64CountsPerSecond);

	// Prevent first frame deltatick spike
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_i64LastTime));

	return (true);
}

bool CWinClock::ProcessFrame()
{
	UpdateDeltatick();
	UpdateFps();

	std::list<CProcessable*>& rProcessables = CProcessable::GetProcessablesList();
	std::list<CProcessable*>::iterator iterCurrentObject = rProcessables.begin();

	while (iterCurrentObject != rProcessables.end())
	{
		// Check rendering enabled
		if ((*iterCurrentObject)->ProcessingEnabled())
		{
			// Render object
			(*iterCurrentObject)->Process();
		}

		++iterCurrentObject;
	}

	return (true);
}

void CWinClock::UpdateDeltatick()
{
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_i64CurrentTime));

	m_dDeltatick  = static_cast<double>(m_i64CurrentTime - m_i64LastTime);
	m_dDeltatick *= m_fPerfCounterTimeScale;

	m_i64LastTime = m_i64CurrentTime;
}

void CWinClock::UpdateFps()
{
	m_fTimeElasped += Deltatick();

	if (m_fTimeElasped > 0.5f)
	{
		m_fTimeElasped -= 0.5f;
		m_uiFramesPerSecond = m_uiFrameCount * 2;
		m_uiFrameCount = 0;
	}

	++m_uiFrameCount;
}

float CWinClock::Deltatick() const
{
	return (static_cast<float>(m_dDeltatick));
}


uint CWinClock::FramesPerSecond() const
{
	return (m_uiFramesPerSecond);
}