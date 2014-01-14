#include "GameEngine.h"

GameEngine::GameEngine(void)
{
	// Init or NULL objects before use to avoid any undefined behavior
	m_bVsync		= false;
	m_pD3DObject	= 0;
	m_pD3DDevice	= 0;
	m_currTime		= 0;
	m_prevTime		= 0;
	m_FPS			= 0;
	m_deltaTime		= 0;
}

GameEngine::~GameEngine(){
	Shutdown();
};

void GameEngine::Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed)
{
	m_menuSelection = 0;
	m_menuState = 0;
	m_hWnd = hWnd;

	//////////////////////////////////////////////////////////////////////////
	// Direct3D Foundations - D3D Object, Present Parameters, and D3D Device
	//////////////////////////////////////////////////////////////////////////

	// Create the D3D Object
	m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	// Find the width and height of window using hWnd and GetWindowRect()
	RECT rect;
	GetWindowRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// Set D3D Device presentation parameters before creating the device
	D3DPRESENT_PARAMETERS D3Dpp;
	ZeroMemory(&D3Dpp, sizeof(D3Dpp));  // NULL the structure's memory

	D3Dpp.hDeviceWindow					= hWnd;										// Handle to the focus window
	D3Dpp.Windowed						= bWindowed;								// Windowed or Full-screen boolean
	D3Dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;								// Format of depth/stencil buffer, 24 bit depth, 8 bit stencil
	D3Dpp.EnableAutoDepthStencil		= TRUE;										// Enables Z-Buffer (Depth Buffer)
	D3Dpp.BackBufferCount				= 1;										// Change if need of > 1 is required at a later date
	D3Dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;							// Back-buffer format, 8 bits for each pixel
	D3Dpp.BackBufferHeight				= height;									// Make sure resolution is supported, use adapter modes
	D3Dpp.BackBufferWidth				= width;									// (Same as above)
	D3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;					// Discard back-buffer, must stay discard to support multi-sample
	D3Dpp.PresentationInterval			= m_bVsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE; // Present back-buffer immediately, unless V-Sync is on								
	D3Dpp.Flags							= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;		// This flag should improve performance, if not set to NULL.
	D3Dpp.FullScreen_RefreshRateInHz	= bWindowed ? 0 : D3DPRESENT_RATE_DEFAULT;	// Full-screen refresh rate, use adapter modes or default
	D3Dpp.MultiSampleQuality			= 0;										// MSAA currently off, check documentation for support.
	D3Dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;						// MSAA currently off, check documentation for support.

	// Check device capabilities
	DWORD deviceBehaviorFlags = 0;
	m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps);

	// Determine vertex processing mode
	if(m_D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		// Hardware vertex processing supported? (Video Card)
		deviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	
	}
	else
	{
		// If not, use software (CPU)
		deviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	}

	// If hardware vertex processing is on, check pure device support
	if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	
	}

	// Create the D3D Device with the present parameters and device flags above
	m_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,		// which adapter to use, set to primary
		D3DDEVTYPE_HAL,			// device type to use, set to hardware rasterization
		hWnd,					// handle to the focus window
		deviceBehaviorFlags,	// behavior flags
		&D3Dpp,					// presentation parameters
		&m_pD3DDevice);			// returned device pointer
	//////////////////////////////////////////////////////////////////////////	
	// View and Projection Matrices
	//////////////////////////////////////////////////////////////////////////
	// Initialize View Matrix
	m_eyePos	= D3DXVECTOR3(0.0f, 2.0f, -10.0f);	// Camera position
	m_lookAt	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// Pos. camera is viewing
	m_upVec		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// Rotational orientation 

	// Easily calculate the view matrix with 3 intuitive vectors
	D3DXMatrixLookAtLH(
		&m_viewMat,	// Returned viewMat
		&m_eyePos,	// Eye Position
		&m_lookAt,	// LookAt Position
		&m_upVec);	// Up Vector

	// Apply the view matrix in the scene
	//m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_viewMat);

	//// Initialize perspective projection matrix, this creates view frustum
	D3DXMatrixPerspectiveFovLH(
		&m_projMat,						// Returned ProjMat
		D3DXToRadian(65.0f),			// Field of View
		(float)width / (float)height,				// Aspect Ratio
		1.0f,							// Near Plane
		1000.0f);						// Far Plane

	// Apply the projection matrix in the scene
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_projMat);

	//////////////////////////////////////////////////////////////////////////
	// Light
	//////////////////////////////////////////////////////////////////////////
	ZeroMemory(&m_Light, sizeof(m_Light));

	// Ambient light color emitted from this light
	m_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// Diffuse light color emitted from this light
	m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// Specular light color emitted from this light
	m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// Light Type (Point) Requires: Position, Range, Attenuation
	m_Light.Type = D3DLIGHT_POINT;	// Point, alternatively D3DLIGHT_DIRECTIONAL or D3DLIGHT_SPOT
	// Light Position
	m_Light.Position = D3DXVECTOR3(0.0f, 10.0f, -10.0f); 
	// Range of Light
	m_Light.Range = 100.0f;
	// Light Attenuation
	m_Light.Attenuation0 = 0.0f;	// Constant
	m_Light.Attenuation1 = 0.05f;	// Linear
	m_Light.Attenuation2 = 0.0f;	// Quadratic

	// Set Light
	m_pD3DDevice->SetLight(0, &m_Light);	// 0 is the index for this light
	// Enable Light
	m_pD3DDevice->LightEnable(0, true);

	//////////////////////////////////////////////////////////////////////////
	// Create a Font Object
	//////////////////////////////////////////////////////////////////////////

	// Load a font for private use for this process
	//AddFontResourceEx(L"Delicious-Roman.otf", FR_PRIVATE, 0);

	// Load D3DXFont, each font style you want to support will need an ID3DXFont
	D3DXCreateFont(m_pD3DDevice, 20, 0, FW_BOLD, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_pD3DFont);

	D3DXCreateFont(m_pD3DDevice, 55, 30, FW_BOLD, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("DK Northumbria"), &m_fontTwo);

	//////////////////////////////////////////////////////////////////////////
	// Create Sprite Object and Textures
	//////////////////////////////////////////////////////////////////////////

	// Create a sprite object, note you will only need one for all 2D sprites
	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Creating Textures for Pong

	// Create a texture, each different 2D sprite to display to the screen
	// will need a new texture object.  If drawing the same sprite texture
	// multiple times, just call that sprite's Draw() with different 
	// transformation values.
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"TitleScreen.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 0, 255), &m_titleInfo, 0, &m_title);

	//D3DXCreateTextureFromFileEx(m_pD3DDevice, L"BlackBackground.png", 0, 0, 0, 0, 
	//	D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 
	//	D3DCOLOR_XRGB(255, 0, 255), &m_backGroundInfo, 0, &m_backGround);

	// Seed rand() with time
	//srand(timeGetTime());

	//////////////////////////////////////////////////////////////////////////
	// Initialize DirectInput
	//////////////////////////////////////////////////////////////////////////

	// Create the DI Object
	DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDIObject, NULL);

	// Initialize Keyboard
	int a = 0;
	m_pDIObject->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL);

	// Initialize Mouse
	m_pDIObject->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);

	// Set up Keyboard
	m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);

	// Set up Mouse (c_dfDIMouse2 = 8 button mouse)
	m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pDIMouse->SetDataFormat(&c_dfDIMouse2);

	//  Set up Audio
	FMOD::System_Create(&fmodSystem);
	fmodSystem->init(10, FMOD_INIT_NORMAL, 0);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  TODO:  Get streaming sound working
	fmodSystem->createStream("Prelude.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &menuMusic);
	fmodSystem->createStream("ZeldaOverworld.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &themeMusic);
	fmodSystem->createStream("PokemonBattle.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &combatMusic);
	//fmodSystem->createSound("Thwack.mp3", FMOD_DEFAULT, 0, &thwack);
	//fmodSystem->createSound("Whiff.mp3", FMOD_DEFAULT, 0, &whiff);

	for(int i = 0; i < 255; ++i){
		keyIsDown[i] = false;
	};
	start = (time(0));
	m_frameTime = 0;
	m_deltaTime = 0;
	m_gameOver = 0;
	cursor.x = 500;
	cursor.y = 300;
	cursor.z = 0;


	if(themeMusicChannel){
		fmodSystem->playSound( FMOD_CHANNEL_FREE, menuMusic, false, &themeMusicChannel);
		int temp = 0;
	}

	m_gameState = MENU_MAIN;
}

void GameEngine::Update(float dt)
{

	//////////////////////////////////////////////////////////////////////////
	// Get and Acquire Keyboard Input
	//////////////////////////////////////////////////////////////////////////

	// buffer - Stores our keyboard device state
	char buffer[256];
	ZeroMemory(buffer, sizeof(buffer));

	// Get the input device state
	HRESULT hr;
	hr = m_pDIKeyboard->GetDeviceState( sizeof(buffer), (LPVOID)&buffer );

	if(FAILED(hr))
	{
		hr = m_pDIKeyboard->Acquire();

		// Device has probably been lost if failed, if so keep trying to get it until it’s found.
		while( hr == DIERR_INPUTLOST)
		{
			hr = m_pDIKeyboard->Acquire();
		}

		// If we failed for some other reason
		if(FAILED(hr))
			return;

		// Read the device state again
		m_pDIKeyboard->GetDeviceState(sizeof(buffer), buffer);
	}

	//////////////////////////////////////////////////////////////////////////
	// Get and Acquire Mouse Input
	//////////////////////////////////////////////////////////////////////////
	// Stores our mouse state for an 8 button mouse.
	DIMOUSESTATE2 mouseState;
	ZeroMemory(&mouseState, sizeof(mouseState));

	// Get the input device state
	hr = m_pDIMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &mouseState );
	if(FAILED(hr))
	{
		hr = m_pDIMouse->Acquire();

		// Device has probably been lost if failed, if so keep trying to get it until it’s found.
		while( hr == DIERR_INPUTLOST)
		{
			hr = m_pDIMouse->Acquire();
		}

		// If we failed for some other reason
		if(FAILED(hr))
			return;
		// Read the device state again
		m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
	}

	//////////////////////////////////////////////////////////////////////////
	//	Keyboard Code Examples: [DIK (DirectInput Key) codes we translate]
	//	DIK_0 – DIK_9
	//	DIK_NUMPAD0 – DIK_NUMPAD9
	//	DIK_A – DIK_Z
	//	DIK_F1 – DIK_F12
	//	DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT	// Arrow Keys
	//	DIK_SPACE, DIK_TAB, DIK_CAPITAL, DIK_LCONTROL (Left Ctrl Key), 
	//  DIK_RCONTROL (Right Ctrl Key), DIK_RETURN, DIK_LMENU (Left Alt Key), 
	//  DIK_LWIN (Left Windows Key), DIK_LSHIFT (Left Shift Key), etc.
	//	Complete list under Keyboard Device in the documentation.
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	Mouse variables:  [MouseState2 structure supports 8 button mice]
	//	lX				-	X-axis mouse relative coordinates
	//	lY				-	Y-axis mouse relative coordinates
	//	lZ				-	Mouse wheel relative coordinates
	//	rgbButtons[8]	-	Array of 8 mouse buttons
	//
	//	Usually mouse hardware maps the button layout in a standard way for 
	//	the first 4 buttons, after that it depends on the mouse hardware layout
	//	rgbButtons[0]	-	Left Mouse Button
	//	rgbButtons[1]	-	Right Mouse Button
	//	rgbButtons[2]	-	Middle Mouse Button (click scroll wheel)
	//	rgbButtons[3]	-	Side Mouse Button 1
	//	rgbButtons[4]	-	Side Mouse Button 2
	//	rgbButtons[5]	-	Side Mouse Button 3
	//	rgbButtons[6]	-	Side Mouse Button 4
	//	rgbButtons[7]	-	Side Mouse Button 5
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	Act on Input
	//////////////////////////////////////////////////////////////////////////

	calcDeltaTime();
	fmodSystem->update();

	switch(m_gameState){
	case MENU_MAIN:
	case MENU_LOAD:
	case MENU_OPTIONS:
	case MENU_CREDITS:
		menuControls();
		break;
	};
};

void GameEngine::Render()
{
	// If the device was not created successfully, return
	if(!m_pD3DDevice)
		return;

	//////////////////////////////////////////////////////////////////////////
	// All draw calls between swap chain's functions, and pre-render and post- 
	// render functions (Clear and Present, BeginScene and EndScene)
	//////////////////////////////////////////////////////////////////////////

	// Clear the back buffer, call BeginScene()
	if(SUCCEEDED(m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DXCOLOR(0.0f, 0.4f, 0.8f, 1.0f), 1.0f, 0)))
	{
		if(SUCCEEDED(m_pD3DDevice->BeginScene()))
		{
			// Call Sprite's Begin to start rendering 2D sprite objects
			if(SUCCEEDED(m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK)))
			{
				if( m_gameOver == 0){
					//INFO:  Draw Graphics
					switch(m_gameState){
						///////////////////////////////////////////
						//  INFO:  If the game is in a menu state,
						//			run drawMenuGraphics().  This function
						//			has a switch that draws the appropriate
						//			graphics for each submenu
					case MENU_MAIN:
					case MENU_LOAD:
					case MENU_OPTIONS:
					case MENU_CREDITS:
						drawMenuGraphics();
						break;
					}
					m_pD3DSprite->End();
					//INFO:  Draw Text
					switch(m_gameState){
						///////////////////////////////////////////
						//  INFO:  If the game is in a menu state,
						//			run drawMenuText().  This function
						//			has a switch that draws the appropriate
						//			text for each submenu
					case MENU_MAIN:
					case MENU_LOAD:
					case MENU_OPTIONS:
					case MENU_CREDITS:
						drawMenuText();
						break;
					};
					m_pD3DDevice->EndScene();
				}
				
				m_pD3DDevice->Present(0, 0, 0, 0);
			}
		}
		// Calculate Frames per Second
		m_currTime = (float)timeGetTime();
		static int fpsCounter = 0;
		if(m_currTime - m_prevTime >= 1000.0f)
		{
			m_prevTime = m_currTime;
			m_FPS = fpsCounter;
			fpsCounter = 0;		
		}
		else
		{
			++fpsCounter;
		}

	}

}

void GameEngine::Shutdown()
{
	// Release COM objects in the opposite order they were created in
	SAFE_RELEASE(m_title);
	//SAFE_RELEASE(m_backGround);
	SAFE_RELEASE(m_pD3DFont);
	SAFE_RELEASE(m_fontTwo);
	RemoveFontResourceEx(L"charge.ttf", FR_PRIVATE, 0);

	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pD3DObject);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Releases Audio
	fmodSystem->release();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  INFO:  Grapic drawing functions
void GameEngine::drawMenuGraphics(){
	D3DXMATRIX transMat, rotMat, scaleMat, worldMat;
	D3DXMatrixIdentity(&transMat);
	D3DXMatrixIdentity(&scaleMat);
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&worldMat);

	switch(m_gameState){
	case MENU_MAIN:
	D3DXMatrixScaling(&scaleMat, 0.8f, 0.3f, 0.0f);				// Scaling
	D3DXMatrixTranslation(&transMat, 370, 170, 0.0f);			// Translation
	D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);			// Multiply scale and rotation, store in scale
	D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);		// Multiply scale and translation, store in world

	// Set Transform
	m_pD3DSprite->SetTransform(&worldMat);
	m_pD3DSprite->Draw(m_title, 0, &D3DXVECTOR3(m_titleInfo.Width * 0.5f, m_titleInfo.Height * 0.5f, 0.0f),
		0, D3DCOLOR_ARGB(255, 255, 255, 255));
	break;
	case MENU_LOAD:
		break;
	case MENU_OPTIONS:
		break;
	case MENU_CREDITS:
		break;
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  INFO:  Text drawing functions
void GameEngine::drawMenuText(){
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	rect.right = rect.right - rect.left;
	rect.bottom = rect.bottom - rect.top;
	rect.top = 80;			
	rect.left = 60;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  INFO:  Draws player UI in each player's color
	switch(m_gameState){
		wchar_t buffer[512];
	case MENU_MAIN:
		swprintf_s(buffer, 512, L"Dragon's Demise Tactics\n\n      1:  New Game\n      2:  Load Game\n      3:  Options\n      4:  Credits\n      5:  Quit");
		m_fontTwo->DrawText(0, buffer, -1, &rect, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 125, 0));
		break;
	case MENU_LOAD:
		rect.top = 10;
		rect.left = 10;
		swprintf_s(buffer, 512, L"TODO:  Add load game functionality\n\nPress Enter to return to Main Menu");
		m_fontTwo->DrawText(0, buffer, -1, &rect, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 125, 0));
		break;
	case MENU_OPTIONS:
		swprintf_s(buffer, 512, L"TODO:  Add Game Options\n\nPress Enter to return to Main Menu");
		m_fontTwo->DrawText(0, buffer, -1, &rect, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 125, 0));
		break;
	case MENU_CREDITS:
		swprintf_s(buffer, 512, L"TODO:  Add Credits\n\nPress Enter to return to Main Menu");
		m_fontTwo->DrawText(0, buffer, -1, &rect, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 125, 0));
		break;

	};
};

void GameEngine::drawDeltaTime(){
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	rect.right = rect.right - rect.left;
	rect.bottom = rect.bottom - rect.top;
	rect.left = 0;
	rect.top = 0;
	// Draw FPS
	wchar_t buffer[64];
	swprintf_s(buffer, 64, L"Time: %d", (int)m_deltaTime);
	//if(m_FPS > 1)
	m_pD3DFont->DrawText(0, buffer, -1, &rect, DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  INFO:  Game Controls functions
void GameEngine::menuControls(){
	// buffer - Stores our keyboard device state
	char buffer[256];
	ZeroMemory(buffer, sizeof(buffer));
	// Get the input device state
	HRESULT hr;
	hr = m_pDIKeyboard->GetDeviceState( sizeof(buffer), (LPVOID)&buffer );

	if(FAILED(hr))
	{
		hr = m_pDIKeyboard->Acquire();

		// Device has probably been lost if failed, if so keep trying to get it until it’s found.
		while( hr == DIERR_INPUTLOST)
		{
			hr = m_pDIKeyboard->Acquire();
		}

		// If we failed for some other reason
		if(FAILED(hr))
			return;

		// Read the device state again
		m_pDIKeyboard->GetDeviceState(sizeof(buffer), buffer);
	}

	//////////////////////////////////////////////////////////////////////////
	// Get and Acquire Mouse Input
	//////////////////////////////////////////////////////////////////////////
	// Stores our mouse state for an 8 button mouse.
	DIMOUSESTATE2 mouseState;
	ZeroMemory(&mouseState, sizeof(mouseState));

	// Get the input device state
	hr = m_pDIMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &mouseState );
	if(FAILED(hr))
	{
		hr = m_pDIMouse->Acquire();

		// Device has probably been lost if failed, if so keep trying to get it until it’s found.
		while( hr == DIERR_INPUTLOST)
		{
			hr = m_pDIMouse->Acquire();
		}

		// If we failed for some other reason
		if(FAILED(hr))
			return;
		// Read the device state again
		m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  INFO:  Allows the user to navigate menus using number keys and "Enter".  I reload the font here to change the size, so each submenu can fit
	//			all of the text within the screen.  There may be a better way to resize the font, but I do not know how to implement it at this time.
	if(m_gameState == MENU_MAIN){
		if(buffer[DIK_1] & 0x80){
			if(!keyIsDown[DIK_1]){
				keyIsDown[DIK_1] = true;
				m_gameState = LEVEL_ONE;
				themeMusicChannel->stop();
				fmodSystem->playSound( FMOD_CHANNEL_FREE, themeMusic, false, &themeMusicChannel);
			}

		}
		else
			keyIsDown[DIK_1] = false;

		if(buffer[DIK_2] & 0x80){
			if(!keyIsDown[DIK_2]){
				keyIsDown[DIK_2] = true;
				D3DXCreateFont(m_pD3DDevice, 35, 15, FW_BOLD, 0, false, DEFAULT_CHARSET, 
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("DK Northumbria"), &m_fontTwo);
				m_gameState = MENU_LOAD;
			}
		}
		else
			keyIsDown[DIK_2] = false;

		if(buffer[DIK_3] & 0x80){
			if(!keyIsDown[DIK_3]){
				keyIsDown[DIK_3] = true;
				D3DXCreateFont(m_pD3DDevice, 35, 15, FW_BOLD, 0, false, DEFAULT_CHARSET, 
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("DK Northumbria"), &m_fontTwo);
				m_gameState = MENU_OPTIONS;
			}

		}
		else
			keyIsDown[DIK_3] = false;

		if(buffer[DIK_4] & 0x80){
			if(!keyIsDown[DIK_4]){
				keyIsDown[DIK_4] = true;
				D3DXCreateFont(m_pD3DDevice, 35, 15, FW_BOLD, 0, false, DEFAULT_CHARSET, 
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("DK Northumbria"), &m_fontTwo);
				m_gameState = MENU_CREDITS;
			}

		}
		else
			keyIsDown[DIK_4] = false;

		if(buffer[DIK_5] & 0x80){
			if(!keyIsDown[DIK_5]){
				keyIsDown[DIK_5] = true;
				Shutdown();
				exit(0);
			}

		}
		else
			keyIsDown[DIK_5] = false;
	}
	else
		if(buffer[DIK_RETURN] & 0x80){
			if(!keyIsDown[DIK_RETURN]){
				keyIsDown[DIK_RETURN] = true;
				D3DXCreateFont(m_pD3DDevice, 55, 30, FW_BOLD, 0, false, DEFAULT_CHARSET, 
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("DK Northumbria"), &m_fontTwo);
				m_gameState = MENU_MAIN;
			}

		}
		else
			keyIsDown[DIK_RETURN] = false;
};












void GameEngine::calcDeltaTime(){
	m_deltaTime = difftime(time(0), start);
};


