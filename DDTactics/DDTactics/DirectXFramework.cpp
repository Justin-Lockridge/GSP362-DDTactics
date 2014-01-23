#include "DirectXFramework.h"

CDirectXFramework::CDirectXFramework(void)
{
	// Init or NULL objects before use to avoid any undefined behavior
	m_bVsync		= false;
	m_pD3DObject	= 0;
	m_pD3DDevice	= 0;
	//m_currTime		= 0;
	//m_prevTime		= 0;
	m_FPS			= 0;
	//m_deltaTime		= 0;
	m_menuSelection = 0;
	m_menuState = 0;
	

	////////////////////////////////////////////////////////
	//Smyth - zero out buffer and mouseState			  //
	////////////////////////////////////////////////////////
	ZeroMemory(buffer, sizeof(buffer));
	ZeroMemory(&mouseState, sizeof(DIMOUSESTATE2));

}

CDirectXFramework::~CDirectXFramework(){
	Shutdown();
};

void CDirectXFramework::Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed)
{
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
	// Font
	//////////////////////////////////////////////////////////////////////////

	D3DXCreateFont(m_pD3DDevice, 20, 0, FW_BOLD, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_pD3DFont);

	//////////////////////////////////////////////////////////////////////////
	// Sprite Object and Textures
	//////////////////////////////////////////////////////////////////////////

	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"TitleScreen.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 0, 255), &m_backGroundInfo, 0, &m_backGround);

	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"cursor.png", 0, 0, 0, 0, 
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DCOLOR_XRGB(255, 0, 255), &m_cursorInfo, 0, &m_cursorTexture);

	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"menuButtons.png", 0, 0, 0, 0, 
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DCOLOR_XRGB(255, 0, 255), 0, 0, &m_menuTextures);



	//////////////////////////////////////////////////////////////////////////
	// DirectInput
	//////////////////////////////////////////////////////////////////////////

	DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDIObject, NULL);
	m_pDIObject->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL);
	m_pDIObject->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);

	// Set up Keyboard
	m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);

	// Set up Mouse (c_dfDIMouse2 = 8 button mouse)
	m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pDIMouse->SetDataFormat(&c_dfDIMouse2);

	
	m_cursor.x = 500;
	m_cursor.y = 300;
	m_cursor.z = 0;

	for(int i = 0; i < 255; ++i){
		m_keyPressed[i] = false;
	};

	//////////////////////////////////////////////////////////////////////////
	// FMOD
	//////////////////////////////////////////////////////////////////////////

	FMOD::System_Create(&fmodSystem);
	
	fmodSystem->init(10, FMOD_INIT_NORMAL, 0);
	
	fmodSystem->createStream("Prelude.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &m_menuMusic);
	fmodSystem->createStream("Overworld.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &m_overWorldMusic);
	//fmodSystem->createSound("Thwack.mp3", FMOD_DEFAULT, 0, &thwack);



	//////////////////////////////////////////////////////////////////////////
	// Game Variables
	//////////////////////////////////////////////////////////////////////////

	//start = (time(0));
	//m_frameTime = 0;
	//m_deltaTime = 0;
	//m_gameOver = 0;

	//m_gameState = MENU;
	InitMenu();
	overworld.init(m_pD3DDevice, fmodSystem);
	//m_gameState = OVERWORLD;

}

void CDirectXFramework::Update(float dt)
{
	fmodSystem->update();

	m_pDIKeyboard->Acquire(); 
	m_pDIKeyboard->GetDeviceState(sizeof(buffer), (LPVOID)&buffer );

	m_pDIMouse->Acquire();
	m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState );

	switch(m_gameState){
	case MENU:
		
		UpdateMenu();
		
		break;
	case OVERWORLD:
	
		overworld.update(BATTLE, m_gameState, mouseState, buffer, fmodSystem, m_musicChannel,
						m_cursor);
		
						
		break;


	default:
		DestroyWindow(m_hWnd);
	};
};

void CDirectXFramework::Render(float dt)
{
	
	/////////////////////////////////
	//Smyth - moved from Menu.cpp  //
	/////////////////////////////////
	if(!m_pD3DDevice)
		return;
	if(SUCCEEDED(m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0))){
		if(SUCCEEDED(m_pD3DDevice->BeginScene())){
			
			switch(m_gameState){
			case MENU:
				RenderMenu();

				break;
			case OVERWORLD:
				overworld.render( m_pD3DSprite, m_cursorTexture, m_cursorInfo, m_cursor, dt );
				break;

			}


			m_pD3DDevice->EndScene();
		}
		m_pD3DDevice->Present(0, 0, 0, 0);
	}

}
void CDirectXFramework::Shutdown()
{
	// Release COM objects in the opposite order they were created in
	//SAFE_RELEASE(m_title);

	overworld.shutdown();
	SAFE_RELEASE(m_backGround);
	SAFE_RELEASE(m_pD3DFont);

	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pD3DObject);

	fmodSystem->release(); //  Releases Audio
}