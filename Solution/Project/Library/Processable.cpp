
//
//  File Name   :   Processable.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Processable.h"

// Local Includes
#include "Framework/Debug.h"

// Library Includes

// Static Initialisers
std::list<CProcessable*> CProcessable::s_Processables;

// Implementation

CProcessable::CProcessable()
: m_uiProcessOrderIndex(0)
, m_bProcessingEnabled(true)
{
	s_Processables.push_back(this);
}

CProcessable::~CProcessable()
{
	s_Processables.remove(this);
}

void  CProcessable::ProcessingEnabled(bool _bEnabled)
{
	m_bProcessingEnabled = _bEnabled;
}

uint CProcessable::ProcessOrderIndex() const
{
	return (m_uiProcessOrderIndex);
}

bool CProcessable::ProcessingEnabled() const
{
	return (m_bProcessingEnabled);
}

std::list<CProcessable*>& CProcessable::GetProcessablesList()
{
	return (s_Processables);
}