
//
//  File Name   :   Observer.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Observer_H__
#define __Observer_H__

// Dependent Includes

// Local Includes
#include "Defines/Types.h"

// Library Includes

// Prototypes
class CEventArgs;

template <class T>
class IObserver
{

	// Member Types
public:

	// Member Functions
public:

			 IObserver() {};
	virtual ~IObserver() {};

	virtual void Notify(T& _rSender, short _sSubject, CEventArgs& _crArgs) = 0;

protected:

private:

	IObserver(const IObserver& _krObserver);
	IObserver& operator = (const IObserver& _krObserver);

	// Member Variables
protected:

private:

};

#endif //__Observer_H__