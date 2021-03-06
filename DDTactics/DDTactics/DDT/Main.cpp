///////////////////////////////////////////////////////////////////////////
//	Title:	Dragon's Demise Tactics
//  Authors:	Justin Lockridge
//				Cameren Sales
//				Ricky Rivera
//				Smyth Chang
//  13 January 2014
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//  KNOWN ISSUES:   
//  []
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//  Updates:
//  []  
//  []  
///////////////////////////////////////////////////////////////////////////

//#include <iostream>
//using namespace std;
#define VC_EXTRALEAN
#include <windows.h>

//#pragma comment(lib, "WINMM.LIB")


//////////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////////
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_TITLE L"Dragon's Demise Tactics"

//////////////////////////////////////////////////////////////////////////
//  INFO:  Dragon's Demise Headers
//////////////////////////////////////////////////////////////////////////
#include "DDTactics.h"

HWND				g_hWnd;			// Handle to the window
HINSTANCE			g_hInstance;	// Handle to the application instance
bool				g_bWindowed;	// Boolean for windowed or full-screen


// Entry point for the game or application.
int WINAPI wWinMain(HINSTANCE hInstance,	// Handle to the application
				   HINSTANCE hPrevInstance,	// Handle to the previous app
				   LPTSTR lpCmdLine,		// Command line string
				   int lpCmdShow);			// Show window value

LRESULT CALLBACK WndProc(HWND hWnd,			// Handle to the window
						 UINT message,		// Incoming Message
						 WPARAM wparam,		// Message Info
						 LPARAM lparam);	// Message Info

void InitWindow(void)
{
	
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	//set up the window
	wndClass.cbSize			= sizeof(WNDCLASSEX);		//size of window structure
	wndClass.lpfnWndProc	= (WNDPROC)WndProc;
	wndClass.lpszClassName  = WINDOW_TITLE;
	wndClass.hInstance		= g_hInstance;
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOWFRAME);
	//wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	RegisterClassEx(&wndClass);

	g_hWnd = CreateWindow(
					WINDOW_TITLE, WINDOW_TITLE,											//class name and title
					g_bWindowed ? WS_OVERLAPPEDWINDOW | WS_VISIBLE: (WS_POPUP | WS_VISIBLE ), //window style
					CW_USEDEFAULT, CW_USEDEFAULT,										//x and y coordinates
					SCREEN_WIDTH, SCREEN_HEIGHT,														//width and height of window
					NULL, NULL,															//parent window and menu
					g_hInstance,															//handle to application
					NULL);

	//Display the window
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	srand(time(NULL));
	g_hInstance = hInstance;	// Store application handle
	g_bWindowed = true;			// Windowed mode or full-screen

	// Init the window
	InitWindow();

	// Use this msg structure to catch window messages
	MSG msg; 
	ZeroMemory(&msg, sizeof(msg));
	// Initialize once
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	// Initialize DirectX/Game here
	//g_DX.Init(g_hWnd, g_hInstance, g_bWindowed);

	// Main Windows/Game Loop
	DWORD lastTime = timeGetTime();
	DWORD currentTime = 0;

	//  INFO:  Dragon's Demise Framework
	DDTactics g_framework;
	
	g_framework.Init(g_hWnd, g_hInstance, g_bWindowed);

	while(msg.message != WM_QUIT)
	{
		// hide cursor
		ShowCursor(false);
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		__int64 currTimeStamp = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
		float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;
		prevTimeStamp = currTimeStamp;
		
		g_framework.Update(dt);
		g_framework.Render(dt);
	}

	//Shutdown DirectX/Game here
	g_framework.Shutdown();

	// Unregister window
	UnregisterClass(WINDOW_TITLE, g_hInstance);

	// Return successful
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	// attempt to handle your messages
	switch(message)
	{
		case (WM_PAINT):
		{
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		}		
		case(WM_DESTROY):
		{
			PostQuitMessage(0); 
			break;
		}
		case(WM_KEYDOWN):
		{
			switch(wparam)
			{
				// Escape Key will close the application
				// Remove/Modify this call when creating a real game with menus, etc.
				case VK_ESCAPE:
				{
					PostQuitMessage(0);
					break;
				}
			}
		}
	}

	// pass to default handler
	return DefWindowProc(hWnd, message, wparam, lparam);
}


