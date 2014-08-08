
//
//  File Name   :   List.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __List_H__
#define __List_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes
#include <vector>

// Prototypes

template<class T>
class CList
{

	// Member Types
public:

	struct TItems
	{
		typename std::vector<T>::iterator iterCurrent;
		typename std::vector<T>::iterator iterEnd;

		TItems (std::vector<T>& _aVector)
		{
			iterCurrent = _aVector.begin();
			iterEnd		= _aVector.end();
		}

		T& Value()			{ return ((*iterCurrent)); }

		void operator ++ () { ++ iterCurrent; }

		bool End()			{ return (iterCurrent == iterEnd); }
	};

	struct TItemsConst
	{
		typename std::vector<T>::const_iterator iterCurrent;
		typename std::vector<T>::const_iterator iterEnd;

		TItemsConst(std::vector<T>& _aVector)
		{
			iterCurrent = _aVector.cbegin();
			iterEnd = _aVector.cend();
		}

		T& Value()			{ return ((*iterCurrent)); }

		void operator ++ ()		{ ++Current; }

		bool End()				{ return (iterCurrent == iterEnd); }
	};

	// Member Functions
public:

	 CList() {};
	 CList(const CList& _krList) { m_aVector = _krList.m_aVector;  }
	~CList() {};

	TItems		Items()					{ return (TItems(m_aVector)); }
	TItemsConst	ItemsConst()			{ return (TItemsConst(m_aVector)); }

	void	Add(T& _rValue)				{ m_aVector.push_back(_rValue); }
	void	Remove(uint _uiIndex)		{ m_aVector.erase(m_aVector.begin() += _uiIndex); }
	void	Clear()						{ m_aVector.clear(); }

	uint	Count()						{ return (m_aVector.size()); }

	T& operator [] (uint _uiIndex)				{ return (m_aVector[_uiIndex]); }
	CList<T>& operator = (const CList& _krList) { m_aVector = _krList.m_aVector; return (*this); }

	std::vector<T>& GetVector()			{ return (m_aVector); }

protected:

private:

	
	

	// Member Variables
protected:

private:

	std::vector<T> m_aVector;

};

#endif //__List_H__