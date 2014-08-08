
//
//  File Name   :   Processable.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Processable_H__
#define __Processable_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include <list>

// Prototypes

class CProcessable
{

	// Member Types
public:

	// Member Functions
public:

			 CProcessable();
	virtual ~CProcessable();

	virtual void Process() = 0;

	void ProcessingEnabled(bool _bEnabled);

	uint ProcessOrderIndex() const;
	bool ProcessingEnabled() const;

	static std::list<CProcessable*>& GetProcessablesList();

protected:

private:

	CProcessable(const CProcessable& _krProcessable);
	CProcessable& operator = (const CProcessable& _krProcessable);

	// Member Variables
protected:

private:

	uint	m_uiProcessOrderIndex;

	uchar	m_ucOwnerSceneIndex;

	bool	m_bProcessingEnabled;

	static std::list<CProcessable*> s_Processables;

};

#endif //__Processable_H__