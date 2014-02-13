#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//directInput libraries
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")


#define LEFT_MOUSE_BUTTON 0
#define RIGHT_MOUSE_BUTTON 1
#define MIDDLE_MOUSE_BUTTON 2


class InputManager
{
private:
	IDirectInput8	*m_Input_Obj;
	IDirectInputDevice8 *mouse;
	IDirectInputDevice8 *keyboard;
		

	char buffer[256];
	bool key_down[256];
	DIMOUSESTATE2 mouseState;

	InputManager();

public:
	
	static InputManager*	instance();

	
	void init(HINSTANCE &hInst, HWND &hWnd);
	void update();


	//////////////////////////////////////////////////////////////////
	//Name:			push_button										//
	//Parameters:	unsigned char keycode - the keycode associated	//
	//				with the button pushed.  ex:  DIK_W				//
	//Return:		bool											//
	//Description:  Returns true if the button is pushed			//
	//				USE THIS FUNCTION FOR BUTTON PRESSES			//
	//////////////////////////////////////////////////////////////////
	bool push_button(unsigned char keycode);

	//////////////////////////////////////////////////////////////////
	//Name:			check_button_down								//
	//Parameters:	unsigned char keycode - the keycode associated	//
	//					with the button being checked				//
	//Return:		bool											//
	//Description:	Check if the button was previously pushed.		//
	//				NOTE: USE push_button IF CHECKING FOR A KEY		//
	//					  PRESS										//
	//				This function is used to limit the button being //
	//					pushed.										//
	//////////////////////////////////////////////////////////////////
	bool check_button_down(unsigned char keycode);

	//////////////////////////////////////////////////////////////////
	//Name:			set_button										//
	//Parameters:   unsigned char keycode - the keycoade associated	//
	//					with the button being checked				//
	//				bool change - a bool value						//
	//Return:		void											//
	//Description:	Used in conjunction with check_button_down to	//
	//				limit the button being pushed.					//
	//////////////////////////////////////////////////////////////////
	void set_button(unsigned char keycode, bool change);

	long get_mouseX();
	long get_mouseY();
	long get_mouseZ();

	//////////////////////////////////////////////////////////////////
	//Name:			check_mouse_button								//
	//Parameters:	int mouse_button - a number associated with		//
	//					the mouse button being pushed				//
	//Return:		bool											//
	//Description:	Check if a mouse button was pushed.				//
	//////////////////////////////////////////////////////////////////
	bool check_mouse_button(int mouse_button);

	void shutdown();

	~InputManager();
};