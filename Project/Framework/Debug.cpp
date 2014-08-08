
//
//  File Name   :   Debug.cpp
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

#include "Debug.h"

// Local Includes
#include "Framework/Renderer.h"
#include "Library/StrUtilities.h"
#include "Library/UiText.h"
#include "Framework/File.h"

// Library Includes
#include <Windows.h>

// Static Initialisers

// Implementation

CDebug::CDebug(IFile& _rLogFile)
: m_rLogFile(_rLogFile)
, m_pConsoleText(0)
, m_uiMaxWindowEntries(15)
, m_bUpdateConsole(false)
, m_bErrorsEnabled(true)
, m_bWarningsEnabled(true)
{
	// Empty
}

CDebug::~CDebug()
{
	CleanWindowMessages();

	FW_DELETE(m_pConsoleText);
}

bool CDebug::Initialise()
{
	APPLICATION.Subscribe(this, CApplication::SUBJECT_INIT_COMPLETE); // Cannot instance text until renderer is made

	return (true);
}

void CDebug::Process()
{
	if (m_bUpdateConsole)
	{
		UpdateConsole();
	}
}

void CDebug::WriteError(c_char* _kcpFunction, uint _uiLine, c_char* _kcpFormat, ...)
{
	// Checks error logging enabled
	if (m_bErrorsEnabled)
	{
		// Get parameters
		va_list cpVarList;
		va_start(cpVarList, _kcpFormat);

		// Write parameters
		char cWriteBuffer[WRITE_BUFFER_LENGTH];
		vsprintf_s(cWriteBuffer, _kcpFormat, cpVarList);

		// Log message
		WriteMessage("[ERROR] %s (%s:%d)", cWriteBuffer, _kcpFunction, _uiLine);

		va_end(cpVarList);

	#ifdef WIN32
		MessageBoxA(0, cWriteBuffer, "Debug - Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
	#endif
	}
}

void CDebug::WriteWarning(c_char* _kcpFunction, uint _uiLine, c_char* _kcpFormat, ...)
{
	// Check warning logging enabled
	if (m_bWarningsEnabled)
	{
		// Get parameters
		va_list cpVarList;
		va_start(cpVarList, _kcpFormat);

		// Write parameters
		char cWriteBuffer[WRITE_BUFFER_LENGTH];
		vsprintf_s(cWriteBuffer, _kcpFormat, cpVarList);

		// Log message
		WriteMessage("[WARNING] %s (%s:%d)", cWriteBuffer, _kcpFunction, _uiLine);

		va_end(cpVarList);
	}
}

void CDebug::WriteMessage(c_char* _kcpFormat, ...)
{
	// Get parameters
	va_list cpVarList;
	va_start(cpVarList, _kcpFormat);

	// Write parameters
	char cWriteBuffer[WRITE_BUFFER_LENGTH];
	vsprintf_s(cWriteBuffer, _kcpFormat, cpVarList);

	WriteWindow(cWriteBuffer);
	WriteOutput(cWriteBuffer);
	WriteFile(cWriteBuffer);

	va_end(cpVarList);
}

void CDebug::Clear()
{
	CleanWindowMessages();
}

void CDebug::TextColour(const TColour& _krColour)
{
	m_pConsoleText->FontColour(_krColour);
}

void CDebug::Notify(CApplication& _rSender, short _sSubject, CEventArgs& _crArgs)
{
	// Have to wait for renderer to be made before the text can be initialized
	if (_sSubject == CApplication::SUBJECT_INIT_COMPLETE)
	{
		VALIDATE_V(InitialiseWindowWriteText());
	}
}

void CDebug::SetErrorReportingEnabled(bool _bEnabled)
{
	m_bErrorsEnabled = _bEnabled;
}

void CDebug::SetWarningReportingEnabled(bool _bEnabled)
{
	m_bWarningsEnabled = _bEnabled;
}

IFile& CDebug::GetDebugFile()
{
	return (m_rLogFile);
}

void CDebug::UpdateConsole()
{
	if (m_MessageLog.size() == 0)
		return;
	
	uint uiTotalLength = 0;
	std::vector<char*>::iterator StartMsg = m_MessageLog.begin();
	std::vector<char*>::iterator CurrentMsg = m_MessageLog.begin();
	std::vector<char*>::iterator End = m_MessageLog.end();

	if (m_MessageLog.size() > CONSOLE_MESSAGE_MAX)
	{
		StartMsg += (m_MessageLog.size() - CONSOLE_MESSAGE_MAX);
	}

	CurrentMsg = StartMsg;

	// Track message lengths
	uint uiMessageLengths[CONSOLE_MESSAGE_MAX];
	FW_MEMZERO(uiMessageLengths, sizeof(uint) * CONSOLE_MESSAGE_MAX);

	for (uint i = 0; i < CONSOLE_MESSAGE_MAX; ++ i)
	{
		// Save message length, plus new line
		uiMessageLengths[i] = strlen(*CurrentMsg) + 1;

		// Add length of message
		uiTotalLength += uiMessageLengths[i];

		// Increment message
		++ CurrentMsg;

		// Break if there is not enough messages
		if (CurrentMsg == End)
		{
			break;
		}
	}

	// Create buffer. plus null terminator
	char* cpBuffer = new char[uiTotalLength + 1];
	char* cpBufferPosition = cpBuffer;

	// Add null terminator
	cpBuffer[uiTotalLength] = '\0';
	
	// Reset message iterations
	CurrentMsg = StartMsg;

	for (uint i = 0; i < CONSOLE_MESSAGE_MAX; ++ i)
	{
		// Copy in text
		FW_MEMCOPY(cpBufferPosition, (*CurrentMsg), sizeof(char) * (uiMessageLengths[i] - 1));

		// Increment buffer position
		cpBufferPosition += (uiMessageLengths[i] - 1);

		*(cpBufferPosition ++) = '\n';

		// Increment message
		++ CurrentMsg;

		// Break if there is not enough messages
		if (CurrentMsg == End)
			break;
	}

	m_pConsoleText->Text(cpBuffer);

	FW_ADELETE(cpBuffer);
}

void CDebug::WriteWindow(c_char* _kcpMessage)
{
	// Count message length
	uint uiMessageLength = strlen(_kcpMessage) + 1;

	// Copy message to new buffer
	char* cpMessage = new char[uiMessageLength];
	FW_MEMCOPY(cpMessage, _kcpMessage, sizeof(char) * uiMessageLength);

	// Append message to log
	m_MessageLog.push_back(cpMessage);

	// Update console
	m_bUpdateConsole = true;
}

void CDebug::WriteOutput(c_char* _kcpMessage)
{
#ifdef _WIN32

	wchar_t* wcpMessage = 0;
	StrUtilities::ConvertToWString(_kcpMessage, wcpMessage);

	OutputDebugString(_kcpMessage);
	OutputDebugString("\n");

	FW_ADELETE(wcpMessage);

#endif //_WIN32
}

void CDebug::WriteFile(c_char* _kcpMessage)
{
	m_rLogFile.Write(_kcpMessage, strlen(_kcpMessage));

	// Keep flushed else important messages may be lost on crash
	m_rLogFile.Flush();
}

bool CDebug::InitialiseWindowWriteText()
{
	m_pConsoleText = RENDERER.InstanceUiText();
	m_pConsoleText->Align(CUiElement::VERTICAL_ALIGN_TOP, CUiElement::HORIZONTAL_ALIGN_LEFT);
	m_pConsoleText->Padding(10, 20, 0, 0);
	m_pConsoleText->Setting(CUiElement::SETTING_WIDTH_RATIO, true);
	m_pConsoleText->Setting(CUiElement::SETTING_HEIGHT_RATIO, true);

	return (true);
}

void CDebug::CleanWindowMessages()
{
	std::vector<char*>::iterator Current = m_MessageLog.begin();
	std::vector<char*>::iterator End = m_MessageLog.end();

	while (Current != End)
	{
		// Add length of message and space for new line
		FW_ADELETE((*Current));

		++ Current;
	}

	m_MessageLog.clear();
	m_bUpdateConsole = true;
}