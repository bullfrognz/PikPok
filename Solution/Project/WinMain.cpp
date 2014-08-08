//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  (c) 2013 Bryce Booth
//
//  File Name   :   WinMain.h
//  Description :   --------------------------
//
//  Author      :   Bryce Booth
//  Mail        :   brycebooth@hotmail.com
//

// Local Includes
#include "Framework/Win/WinApplication.h"
#include "Framework/Std/StdFile.h"
#include "Framework/Debug.h"
#include "Framework/Window.h"
#include "Scenes/Shapes/ShapesScene.h"

// Library Includes
#include <Windows.h>
#include <VLD/vld.h>
#include <ctime>

// Implementation

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _1pCmdLine, INT _iCmdShow)
{
	MSG tScreensMessage;
	FW_MEMZERO(&tScreensMessage, sizeof(MSG));

	// Open file for debug logging
	CStdFile DebugLogFile;
	DebugLogFile.Open("Debug Output.txt", IFile::OPEN_ATTRIBRUTE_WRITE | IFile::OPEN_ATTRIBRUTE_APPEND_ONLY);

	// Write startup time to debug log
	std::time_t i64Time = std::time(NULL);

	struct tm tTimeInfo;
	localtime_s(&tTimeInfo, &i64Time);

	char cFormatBuffer[32];
	asctime_s(cFormatBuffer, &tTimeInfo);

	// Insert padding from previous entries
	DebugLogFile.Write("\n");
	
	DebugLogFile.Write(cFormatBuffer);

	// Start application instances
	{
		CDebug Debug(DebugLogFile);
		CWinApplication Application(&Debug);

		if (!Application.Initialise("PikPok Shapes", _hInstance, 800, 700))
		{
			DEBUG_ERROR("Failed to initialise WinApplication. Application loop not started");
		}
		else
		{
			DEBUG_WRITE("WinApplication initialise successful");

			// Center window
			Application.Window().Center();

			// Load default scene
			Application.LoadScene<CShapesScene>();

			// Application loop
			while ( tScreensMessage.message	!= WM_QUIT &&
				   !Application.IsQuitting())
			{
				// Check if there are any messages
				if (PeekMessage(&tScreensMessage, NULL, 0, 0, PM_REMOVE))
				{
					// Check this is a dialog message
					if (IsDialogMessage(0, &tScreensMessage))
					{
						// Let message go to dialog window
					}
					else
					{
						TranslateMessage(&tScreensMessage); // Translates virtual-key messages into character messages
						DispatchMessage(&tScreensMessage);  // Dispatches message to window procedures
					}
				}
				else
				{
					Application.ExecuteOneFrame();
				}
			}

			DEBUG_WRITE("Application loop ended. Shutting down");
		}
	}

	// Save and close debug log file
	DebugLogFile.Close();

	return (static_cast<int>(tScreensMessage.wParam));
}