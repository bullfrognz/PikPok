
//
//  File Name   :   Subject.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Subject_H__
#define __Subject_H__

// Dependent Includes

// Local Includes
#include "EventArgs.h"
#include "Observer.h"

// Library Includes
#include <stdarg.h>
#include <vector>
#include <algorithm>

// Prototypes

template<class T>
class CSubject
{

	// Member Types
public:

	typedef IObserver<T> ObserverT;
	#define ObserverIter typename std::vector< ObserverT* >::iterator

	// Member Functions
public:

			 CSubject();
	virtual ~CSubject();

	virtual void Subscribe(ObserverT* _pObserver, short _sSubject);
	virtual void Unsubscribe(ObserverT* _pObserver, short _sSubject);
	virtual void UnsubscribeAll(ObserverT* _pObserver);


	virtual void SetIdentifier(uint _uiIdentifier)	{ m_uiIdentifier = _uiIdentifier; }
	virtual uint GetIdentifier()					{ return (m_uiIdentifier);		  }

protected:

	void NotifySubscribers(short _sSubject, ...);

private:

	CSubject(const CSubject& _krSubject);
	CSubject& operator = (const CSubject& _krSubject);

	// Member Variables
protected:

private:

	uint	m_uiIdentifier;
	int		m_iMaxSubjectId;

	std::vector< std::vector<ObserverT*> >	m_aObservers;

	static CEventArgs s_cEmptyArgs;

};

template <class T>
CEventArgs CSubject<T>::s_cEmptyArgs(1);

template <class T>
CSubject<T>::CSubject()
	: m_iMaxSubjectId(-1)
{
	// Empty
}

template <class T>
CSubject<T>::~CSubject()
{
	// Empty
}

template <class T>
void CSubject<T>::Subscribe(ObserverT* _pObserver, short _sSubject)
{
	if (_sSubject > m_iMaxSubjectId)
	{
		m_iMaxSubjectId = _sSubject;
		m_aObservers.resize(m_iMaxSubjectId + 1);
	}

	m_aObservers[_sSubject].push_back(_pObserver);
}

template <class T>
void CSubject<T>::Unsubscribe(ObserverT* _pObserver, short _sSubject)
{
	ObserverIter Observer;
	ObserverIter Begin = m_aObservers[_sSubject].begin();
	ObserverIter End = m_aObservers[_sSubject].end();

	Observer = std::find(Begin, End, _pObserver);

	if (Observer != End)
	{
		(*Observer) = null;
	}
}

template <class T>
void CSubject<T>::UnsubscribeAll(ObserverT* _pObserver)
{
	for (unsigned int uiSubject = 0; uiSubject < m_aObservers.size(); ++ uiSubject)
	{
		Unsubscribe(_pObserver, uiSubject);
	}
}

template <class T>
void CSubject<T>::NotifySubscribers(short _sSubject, ...)
{
	if (_sSubject <= m_iMaxSubjectId)
	{
		ObserverIter Current = m_aObservers[_sSubject].begin();
		ObserverIter End = m_aObservers[_sSubject].end();

		va_list vl;
		va_start(vl, _sSubject);

		for (; Current != End; ++ Current)
		{
			if ((*Current) != null)
			{
				(*Current)->Notify(reinterpret_cast<T&>(*this), _sSubject, CEventArgs(vl));
			}
		}

		va_end(vl);
	}
	else
	{
		// Don't send
	}
}

#endif //__Subject_H__