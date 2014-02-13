#include "InputManager.h"

InputManager::InputManager()
{
	m_Input_Obj = 0;
	mouse = 0;
	keyboard = 0;
	ZeroMemory( buffer, sizeof(buffer));
	ZeroMemory( key_down, sizeof(key_down));
	ZeroMemory(&mouseState, sizeof( DIMOUSESTATE2 ));
}

InputManager* InputManager::instance()
{
	static InputManager input;
	return &input;
}

void InputManager::init(HINSTANCE &hInst, HWND &hWnd)
{


	DirectInput8Create( hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_Input_Obj,
		NULL);

	m_Input_Obj->CreateDevice( GUID_SysKeyboard,
		&keyboard,
		NULL);

	m_Input_Obj->CreateDevice( GUID_SysMouse,
		&mouse,
		NULL);

	keyboard->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	mouse->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE );
	keyboard->SetDataFormat(&c_dfDIKeyboard);
	mouse->SetDataFormat(&c_dfDIMouse2);

	
}

void InputManager::update()
{
	HRESULT hr;

	hr = mouse->Acquire();

	//check for failure to acquire mouse
	if( FAILED(hr) )
	{
		ZeroMemory(&mouseState, sizeof( DIMOUSESTATE2 ) );  //if mouse fails, clear the memory out
		hr = mouse->Acquire();								//and re-acquire
	}else													//else get the state of the mouse
		hr = mouse->GetDeviceState( sizeof( DIMOUSESTATE2 ), &mouseState); 
	
	hr = keyboard->Acquire();

	if( FAILED(hr) )							//if keyboard fails to acquire
	{
		ZeroMemory(buffer, sizeof(buffer) );	//clear keyboard buffer memory
		hr = keyboard->Acquire();				//and re-acquire
	}else										//else get the state of the keyboard
		hr = keyboard->GetDeviceState( sizeof(buffer),  (LPVOID)&buffer ); 

}


bool InputManager::push_button(unsigned char keycode)
{
	
	return ( buffer[keycode] & 0x80 )  != 0;

}

bool InputManager::check_button_down(unsigned char keycode)
{
	return key_down[keycode];
}

void InputManager::set_button(unsigned char keycode, bool change)
{
	key_down[keycode] = change;
}

long InputManager::get_mouseX()		
{
	/***return change in X */
	return mouseState.lX;  
}

long InputManager::get_mouseY()
{
	/**return change in Y */
	return mouseState.lY;
}

long InputManager::get_mouseZ()
{
	return mouseState.lZ;
}


bool InputManager::check_mouse_button(int mouse_button)
{
	return (mouseState.rgbButtons[mouse_button] & 0x80) != 0;

}



void InputManager::shutdown()
{
	mouse->Unacquire();
	if(mouse)
	{
		mouse->Release();
		mouse = 0;
	}
	
	
	keyboard->Unacquire();
	if(keyboard)
	{
		keyboard->Release();
		keyboard = 0;
	}
	if(m_Input_Obj != 0)
	{
		m_Input_Obj->Release();
		m_Input_Obj = 0;
	}
}

InputManager::~InputManager()
{

}