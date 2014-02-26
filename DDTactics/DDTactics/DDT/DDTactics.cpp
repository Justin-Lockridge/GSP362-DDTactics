#include "DDTactics.h"

DDTactics::DDTactics(void)
{
	m_bVsync = false;
	D3DObject = 0;
	D3DDevice = 0;
	m_FPS = 0;
	m_gameOver = 0;
	m_gameState = MENU;

	//////////////////////////////////
	//  INFO:  Warrior Job mod 
	job_mods temp;
	temp.mod_attack = 3;
	temp.mod_health = 3;
	temp.mod_mana	= 1;
	temp.mod_magic = 1;
	temp.mod_defense = 3;
	temp.mod_resistance = 2;
	temp.mod_speed = 2;
	temp.mod_evasion = 1;
	temp.mod_hit = 2;
	jobMods.push_back(temp);

	//////////////////////////////////
	//  INFO:  Archer Job mod 
	temp.mod_attack = 2;
	temp.mod_health = 2;
	temp.mod_mana	= 1;
	temp.mod_magic = 1;
	temp.mod_defense = 2;
	temp.mod_resistance = 2;
	temp.mod_speed = 3;
	temp.mod_evasion = 3;
	temp.mod_hit = 2;
	jobMods.push_back(temp);

	//////////////////////////////////
	//  INFO:  GreyMage Job mod 
	temp.mod_attack = 1;
	temp.mod_health = 1;
	temp.mod_mana	= 3;
	temp.mod_magic = 3;
	temp.mod_defense = 2;
	temp.mod_resistance = 2;
	temp.mod_speed = 2;
	temp.mod_evasion = 1;
	temp.mod_hit = 2;
	jobMods.push_back(temp);
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

	D3DXCreateFont(D3DDevice, 30, 0, FW_BOLD, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Ariel"), &D3DFont);

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

	battle = Battle::instance();

	status_menu = StatusMenu::instance();
	status_menu->init(player, jobMods);

	ioManager = IOManager::instance();
	ioManager->init();
	ioManager->loadSaves();

	graphics3D = GraphicsManager3D::instance();
	graphics3D->Init(D3DDevice);


	introMenu = IntroMenu::instance();
	introMenu->init();

	textManager = TextManager::instance();
	textManager->init(D3DDevice);

	town = Town::instance();
	town->init();
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
		ioManager->update(input, cursor, player, m_gameState, dt);
		//  INFO:  Quick bug fix.  Music used to continue playing when you went from Save / Load gamestate to Overworld.
		//	TODO:  Implement a better fix later.
		//  INFO:  If the player loads a game, then this function will reset all of the character stats in the player's army
		if(m_gameState != SAVE && m_gameState != LOAD){
			for(int i = 0; i < 3; i++){
				player->resetStats(i, jobMods[player->getCharacter(i)->getCurrentJob()]);
			}
		}
		if(m_gameState == OVERWORLD)
			sound->stopStream();
		break;
	case OPTIONS:
		if(input->push_button(DIK_BACKSPACE))
			m_gameState = MENU;
		break;
	case BATTLE:
		battle->Init(player);
		m_gameState = ACTUAL_BATTLE;
		break;
	case ACTUAL_BATTLE:
		sound->playStream(SONG_BATTLE);
		battle->Update(cursor,input,sound,player,m_gameState,dt,graphics3D,D3DDevice);
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
	case CREDITS:
	case QUIT:
		DestroyWindow(m_hWnd);
		break;
	case TOWN:
		town->update(cursor->cursorPos, input, sound, m_gameState, dt, player);
		break;
	case STATUS:
		status_menu->Update(cursor, input, sound, player, m_gameState, jobMods, dt);
		break;
	case INTRO:
		sound->playStream(SONG_INTRO);
		textManager->update(dt);
		introMenu->Update(cursor, input, sound, player, m_gameState, dt);
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
			if(m_gameState == ACTUAL_BATTLE)
				battle->Render3D(graphics,D3DSprite,graphics3D,dt,D3DDevice);
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
					break;
				case TOWN:
					town->render(graphics, D3DSprite, dt, player, &m_hWnd, D3DFont);
					break;
				case BATTLE:
					break;
				case ACTUAL_BATTLE:
					battle->Render2D(graphics,D3DSprite,graphics3D,dt,D3DDevice);
					break;
				case STATUS:
					status_menu->Render(graphics, D3DSprite, player, dt);
					break;
				case INTRO:
					introMenu->Render(graphics, D3DSprite, dt);
				}
				cursor->render(graphics, D3DSprite);

				D3DSprite->End();

				//  INFO:  Draws text for different game states
				switch(m_gameState)
				{
				case INTRO:
					textManager->render();
					break;
				case ACTUAL_BATTLE:
					battle->RenderText(D3DFont);
					break;
				case TOWN:
					town->renderText(D3DFont, player, &m_hWnd, dt);
					break;
				case SAVE:
				case LOAD:
					textManager->renderSavedGameText(ioManager, m_gameState);
					break;

				case STATUS:
					status_menu->drawText(D3DFont, player);
					break;
				}
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