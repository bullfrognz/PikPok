
//
//  File Name   :   Dictionary.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Dictionary_H__
#define __Dictionary_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include <map>

// Prototypes

template<class KEY_T, class VALUE_T>
class CDictionary
{

	// Member Types
public:

	struct TEntries
	{
		typename std::map<KEY_T, VALUE_T>::iterator Current;
		typename std::map<KEY_T, VALUE_T>::iterator End;

		TEntries(std::map<KEY_T, VALUE_T>& _rMap)
		{
			Current = _rMap.begin();
			End = _rMap.end();
		}

		const KEY_T& Key()
		{
			return ((*Current).first);
		}

		VALUE_T& Value()
		{
			return ((*Current).second);
		}

		void operator ++ ()
		{
			++Current;
		}

		bool AtEnd()
		{
			return (Current == End);
		}
	};

	// Member Functions
public:

	 CDictionary() {};
	 CDictionary(std::map<KEY_T, VALUE_T>& _mrMap) : m_mMap(_mrMap) {};
	~CDictionary() {};

	TEntries GetEntries()
	{
		return (TEntries(m_mMap));
	}

	void Add(KEY_T _Key, VALUE_T& _rValue)
	{
		m_mMap.insert(std::pair<KEY_T, VALUE_T>(_Key, _rValue));
	}

	void Remove(KEY_T _Key)
	{
		typename std::map<KEY_T, VALUE_T>::iterator Entry = m_mMap.find(_Key);

		if (Entry != m_mMap.end())
		{
			m_mMap.erase(Entry);
		}
	}

	void Clear()						{ m_mMap.clear(); }
	bool HasKey(KEY_T _Key)				{ return (m_mMap.find(_Key) != m_mMap.end()); }
	uint Count()						{ return (m_mMap.size()); }

	VALUE_T& operator [] (KEY_T _Key)	{ return (m_mMap[_Key]); }

	// Inline Functions
	std::map<KEY_T, VALUE_T>& Map()		{ return (m_mMap); }

protected:

private:

	CDictionary(const CDictionary& _krDictionary);
	CDictionary& operator = (const CDictionary& _krDictionary);

	// Member Variables
protected:

private:

	std::map<KEY_T, VALUE_T> m_mMap;

};

#endif //__Dictionary_H__