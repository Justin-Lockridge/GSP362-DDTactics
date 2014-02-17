#include "DDTactics.h"

DDTactics::DDTactics(void)
{
	m_bVsync = false;
	D3DObject = 0;
	D3DDevice = 0;
	m_FPS = 0;
	m_gameOver = 0;
	m_gameState = MENU;
}

DDTactics::~DDTactics(void)
{}

void DDTactics::Init(HWND &hWnd, HINSTANCE &hInst, bool bWindowed)
{
	m_hWnd = hWnd;

	//Create the D3D Object
	D3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	//Find the width and height of window using hWnd and GetWindowRect()
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
	D3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &D3DCaps);

	// Determine vertex processing mode
	if(D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
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
	if(D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	
	}

	// Create the D3D Device with the present parameters and device flags above
	D3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,		// which adapter to use, set to primary
		D3DDEVTYPE_HAL,			// device type to use, set to hardware rasterization
		hWnd,					// handle to the focus window
		deviceBehaviorFlags,	// behavior flags
		&D3Dpp,					// presentation parameters
		&D3DDevice);			// returned device pointer

		
	input = InputManager::instance();
	input->init(hInst, m_hWnd);

	sound = SoundManager::instance();
	sound->init();

	graphics = GraphicsManager2D::instance();
	graphics->init(D3DDevice);

	D3DXCreateSprite(D3DDevice, &D3DSprite);


	cursor = Cursor::instance();
	cursor->init();

	menu = Menu::instance();
	menu->Init();

	player = Player::instance();

	overworld = Overworld::instance();
	overworld->init(player);

	

	status_menu = StatusMenu::instance();
	status_menu->init();

	ioManager = IOManager::instance();
	ioManager->init();

	graphics3D = GraphicsManager3D::instance();
	graphics3D->Init(D3DDevice);
	
}

void DDTactics::Update(float dt)
{
	input->update();
	sound->update();
	cursor->update(input->get_mouseX(), input->get_mouseY());



	switch(m_gameState)
	{
	case MENU:
		menu->Update(cursor, sound, input, m_gameState, dt);
		
		break;
	case LOAD: case SAVE:
		ioManager->update(input, cursor, m_gameState, dt);
		break;
	case OPTIONS:
		if(input->push_button(DIK_BACKSPACE))
			m_gameState = MENU;
		break;
	case BATTLE:
		sound->playStream(SONG_BATTLE);
		if(input->push_button(DIK_BACKSPACE))
		{
			sound->stopStream();
			m_gameState = OVERWORLD;
		}
		break;
	case MAP:
		break;
	case TAVERN:
				
		if(input->push_button(DIK_BACKSPACE))
		{
			m_gameState = OVERWORLD;
			sound->stopStream();
		}
		break;
	case OVERWORLD:
		overworld->update(cursor->cursorPos, input, sound, player, m_gameState, dt);
		break;
	case QUIT:
		DestroyWindow(m_hWnd);
		break;
	case TOWN:
		sound->playStream(SONG_TOWN);
		m_gameState = TAVERN;
		break;
	case STATUS:
		status_menu->Update(cursor, input, sound, player, m_gameState, dt);
		break;
	
	}
}


void DDTactics::Render(float dt)
{
	if(!D3DDevice)
		return;
	if(SUCCEEDED(D3DDevice->Clear(0,0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0)))
	{
		if(SUCCEEDED(D3DDevice->BeginScene()))
		{
			if(SUCCEEDED(D3DSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK)))
			{


				switch(m_gameState)
				{
					
				case MENU:
					menu->Render(graphics, D3DSprite, dt);
					break;
				case OVERWORLD:
					overworld->render(graphics, D3DSprite, player->overworld_pos, dt);
					break;
				case LOAD: case SAVE:
					ioManager->render(graphics, D3DSprite, dt);
					break;
				case OPTIONS:
					break;

				case TAVERN:
					graphics->Draw2DObject(D3DXVECTOR3(2.0f, 2.0f, 2.0f),
						D3DXVECTOR3(150.0f, 150.0f, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DSprite, GRAPHICS_SHOP_KEEP, 
						D3DCOLOR_ARGB(255,255,255,255));
					
					break;
				case BATTLE:

					graphics3D->DrawMap(D3DXVECTOR3(5,5,5),
										D3DXVECTOR3(0,0,0),
										D3DXVECTOR3(0,0,0),
										MAP_DEFAULT);
					graphics3D->DrawCharacter(D3DXVECTOR3(.7,.7,.7),
											  D3DXVECTOR3(0,0,0),
											  D3DXVECTOR3(0,0,0),
											  MODEL_DEFAULT);

										

					
					/*graphics->Draw2DObject(D3DXVECTOR3(1.0f, 1.0f, 1.0f),
											D3DXVECTOR3(400.0f, 200.0f, 0.0f),
											D3DXVECTOR3(0.0f, 0.0f, 0.0f),
											D3DSprite,
											GRAPHICS_BATTLE_SPLASH,
											D3DCOLOR_ARGB(255,255,255,255)
											);*/

				
					break;

				case STATUS:
					status_menu->Render(graphics, D3DSprite, dt);
					break;

				}
				cursor->render(graphics, D3DSprite);
				D3DSprite->End();
			}
		}
		D3DDevice->EndScene();
	}
	D3DDevice->Present(0,0,0,0);
}

void DDTactics::Shutdown()
{
	graphics3D->Shutdown();
	input->shutdown();
	sound->shutdown();
	graphics->shutdown();

	SAFE_RELEASE(D3DSprite);
	SAFE_RELEASE(D3DDevice);
	SAFE_RELEASE(D3DObject);
}