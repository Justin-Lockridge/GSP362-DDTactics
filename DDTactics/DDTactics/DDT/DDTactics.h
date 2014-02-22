#pragma once

#include <stdio.h>
#include <time.h>
#include <vector>

#include <fstream>
#include <string>

#pragma comment(lib, "winmm.lib")

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//#include "InputManager.h"
#include "SoundManager.h"
#include "IOManager.h"
//#include "GraphicsManager2D.h"
#include "Cursor.h"
#include "Menu.h"
#include "Overworld.h"
#include "Player.h"
#include "StatusMenu.h"
#include "GraphicsManager3D.h"
#include "Intro.h"
#include "TextManager.h"
#include "Town.h"

//#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}


class DDTactics
{
	//HWND		m_hWnd;
	bool		m_bVsync;

	int			m_FPS,
				m_gameOver,
				m_gameState;


	IDirect3D9			*D3DObject;
	IDirect3DDevice9	*D3DDevice;
	D3DCAPS9			D3DCaps;

	ID3DXSprite			*D3DSprite;
	ID3DXFont			*D3DFont;

	InputManager		*input;
	SoundManager		*sound;
	GraphicsManager2D	*graphics;
	GraphicsManager3D	*graphics3D;
	Cursor				*cursor;
	Menu				*menu;
	Overworld			*overworld;
	IOManager			*ioManager;
	Town				*town;


	Player				*player;
	//add menu, overworld, battle, etc objects here

	StatusMenu			*status_menu;
	IntroMenu			*introMenu;
	TextManager			*textManager;
	std::vector<job_mods> jobMods;
public:
	HWND		m_hWnd;
	DDTactics(void);
	~DDTactics(void);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Init
	// Parameters:	HWND hWnd - Handle to the window for the application
	//				HINSTANCE hInst - Handle to the application instance
	//				bool bWindowed - Boolean to control windowed or full-screen
	// Return:		void
	// Description:	Ran once at the start.  Initialize DirectX components and 
	//				variables to control the application.  
	//////////////////////////////////////////////////////////////////////////
	
	void Init(HWND &hWnd, HINSTANCE &hInst, bool bWindowed); 

	//////////////////////////////////////////////////////////////////////////
	// Name:		Update
	// Parameters:	float elapsedTime - Time that has elapsed since the last
	//					update call.
	// Return:		void
	// Description: Runs every frame, use dt to limit functionality called to
	//				a certain amount of elapsed time that has passed.  Used 
	//				for updating variables and processing input commands prior
	//				to calling render.
	//////////////////////////////////////////////////////////////////////////
	
	void Update(float dt);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Render
	// Parameters:	float elapsedTime - Time that has elapsed since the last
	//					render call.
	// Return:		void
	// Description: Runs every frame, use dt to limit functionality called to
	//				a certain amount of elapsed time that has passed.  Render
	//				calls all draw call to render objects to the screen.
	//////////////////////////////////////////////////////////////////////////
	void Render(float dt);


	//////////////////////////////////////////////////////////////////////////
	// Name:		Shutdown
	// Parameters:	void
	// Return:		void
	// Description:	Runs once at the end of an application.  Destroy COM 
	//				objects and deallocate dynamic memory.
	//////////////////////////////////////////////////////////////////////////
	void Shutdown();
};
