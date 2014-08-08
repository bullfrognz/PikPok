
//
//  File Name   :   Debug.h
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#pragma once

#ifndef __Debug_H__
#define __Debug_H__

// Dependent Includes
#include "Library/Observer.h"

// Local Includes
#include "Framework/Application.h"
#include "Defines/Types.h"

// Library Includes
#include <vector>
#include <list>
#include <stdarg.h>

// Prototypes
class IFile;
class IUiText;
class CApplication;
struct TColour;

class CDebug : public IObserver<CApplication>
{

#define DEBUG										APPLICATION.Debug()

#define DEBUG_WRITE(MESSAGE, ...)					DEBUG.WriteMessage(MESSAGE, ##__VA_ARGS__);

#define DEBUG_ERROR(MESSAGE_FORMAT, ...)			DEBUG.WriteError(__FUNCTION__, __LINE__, MESSAGE_FORMAT, ##__VA_ARGS__); FW_BREAKPOINT;
#define DEBUG_ERROR_ON(CONDITION, MESSAGE, ...)		if (CONDITION) { DEBUG_ERROR(MESSAGE, ##__VA_ARGS__); };

#define DEBUG_WARNING(MESSAGE_FORMAT, ...)			DEBUG.WriteWarning(__FUNCTION__, __LINE__, MESSAGE_FORMAT,  ##__VA_ARGS__);
#define DEBUG_WARNING_ON(CONDITION, MESSAGE, ...)	if (CONDITION) { DEBUG_WARNING(MESSAGE, ##__VA_ARGS__); };

#define VALIDATE(CONDITION)							if (!CONDITION) { DEBUG.WriteError(__FUNCTION__, __LINE__, # CONDITION); return (false); }
#define VALIDATE_V(CONDITION)						if (!CONDITION) { DEBUG.WriteError(__FUNCTION__, __LINE__, # CONDITION); return ; }

	// Member Types
public:

	enum ESettings
	{
		WRITE_BUFFER_LENGTH = 4096,
		CONSOLE_MESSAGE_MAX = 10,
	};

	// Member Functions
public:

			 CDebug(IFile& _rLogFile);
	virtual ~CDebug();

	virtual void Process();

	virtual void Notify(CApplication& _rSender, short _sSubject, CEventArgs& _crArgs);

	bool Initialise();

	void WriteError(c_char* _kcpFunction, uint _uiLine, c_char* _kcpFormat, ...);
	void WriteWarning(c_char* _kcpFunction, uint _uiLine, c_char* _kcpFormat, ...);
	void WriteMessage(c_char* _kcpFormat, ...);

	void Clear();

	void TextColour(const TColour& _krColour);

	void SetErrorReportingEnabled(bool _bEnabled);
	void SetWarningReportingEnabled(bool _bEnabled);

	IFile& GetDebugFile();

protected:

	void UpdateConsole();

	void WriteVarList(uint _uiTargets, c_char* _kcpFormat, va_list& _rVarList);
	void WriteWindow(c_char* _kcpMessage);
	void WriteOutput(c_char* _kcpMessage);
	void WriteFile(c_char* _kcpMessage);

private:

	bool InitialiseWindowWriteText();

	void CleanWindowMessages();

	CDebug(const CDebug& _krDebug);
	CDebug& operator = (const CDebug& _krDebug);

	// Member Variables
protected:

private:

	IFile&				m_rLogFile;

	IUiText*			m_pConsoleText;

	uint				m_uiMaxWindowEntries;

	bool				m_bUpdateConsole;
	bool				m_bErrorsEnabled;
	bool				m_bWarningsEnabled;

	std::vector<char*>	m_MessageLog;

};

#endif //__Debug_H__