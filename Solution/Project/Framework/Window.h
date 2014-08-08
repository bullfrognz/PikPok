
//
//  File Name   :   Window.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Window_H__
#define __Window_H__

// Dependent Includes

// Local Includes
#include "Library/Subject.h"
#include "Defines/Types.h"

// Library Includes

// Prototypes

class IWindow : public CSubject<IWindow>
{

#define WINDOW APPLICATION.Window()

	// Member Types
public:

	enum ESubject
	{
		SUBJECT_FOCUS,
		SUBJECT_UNFOCUS,

		MAX_SUBJECT
	};

	// Member Functions
public:

			 IWindow() {};
	virtual ~IWindow() {};

	virtual void Title(c_char* _kcpTitle)					= 0;
	virtual void Dimentions(float _fWidth, float _fHeight)	= 0;
	virtual void Center()									= 0;

	virtual float Width()		= 0;
	virtual float Height()		= 0;
	virtual float MarginX()		= 0;
	virtual float MarginY()		= 0;

	virtual bool IsFocused()	= 0;

protected:

private:

	// Member Variables
protected:

private:

};

#endif //__Window_H__