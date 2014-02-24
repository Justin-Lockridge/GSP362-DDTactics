#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <vector>

#define SAFE_RELEASE(x) if(x){ x->Release(); x = 0; }

class GraphicsManager2D
{
private:
	
	//Containers for textures and imageinfo
	std::vector<IDirect3DTexture9*> textures;
	std::vector<D3DXIMAGE_INFO> imageInfos;

	//cursor texture/imageinfo
	IDirect3DTexture9	*cursorTexture;
	D3DXIMAGE_INFO		cursorInfo;

	//Main Menu and button texture/imageinfo
	IDirect3DTexture9	*menuTexture, *buttonTexture;
	D3DXIMAGE_INFO		menuInfo, buttonInfo;

	//Overworld textures/imageinfo
	IDirect3DTexture9	*overworldTexture,
						*characterTexture,
						*menuBackgroundTexture,
						*menuWordsTexture,
						*townTexture,
						*bookTexture,
						*swordsTexture;
						
	D3DXIMAGE_INFO		overworldInfo,
						overworldCharInfo,
						menuBackgroundInfo,
						wordsInfo,
						townInfo,
						bookInfo,
						swordsInfo;
						

	//ADD ADDITIONAL TEXTURES/IMAGEINFO HERE
	//////////////////////////////////////////////////////////////////

	//Status Menu textures/imageinfo
	IDirect3DTexture9	*status_background_Texture;
	IDirect3DTexture9	*status_border_Texture;
	IDirect3DTexture9	*status_buttons_Texture;

	D3DXIMAGE_INFO		status_background_Info;
	D3DXIMAGE_INFO		status_border_Info;
	D3DXIMAGE_INFO		status_button_Info;

	IDirect3DTexture9	*FFT_texture;
	D3DXIMAGE_INFO		FFT_Info;

	//Shop
	IDirect3DTexture9	*FFT_shop_text;
	D3DXIMAGE_INFO		FFT_shop_Info;
	IDirect3DTexture9	*FFT_weapons_text;
	D3DXIMAGE_INFO		FFT_weapons_Info;
	IDirect3DTexture9	*FFT_helms_text;
	D3DXIMAGE_INFO		FFT_helms_Info;
	IDirect3DTexture9	*FFT_chest_text;
	D3DXIMAGE_INFO		FFT_chest_Info;
	IDirect3DTexture9	*FFT_accesory_text;
	D3DXIMAGE_INFO		FFT_accesory_Info;
	IDirect3DTexture9	*FFT_potions_text;
	D3DXIMAGE_INFO		FFT_potions_Info;
	IDirect3DTexture9	*shop_menu_text;
	D3DXIMAGE_INFO		shop_menu_Info;
	IDirect3DTexture9	*weapon_button_text;
	D3DXIMAGE_INFO		weapon_button_Info;
	IDirect3DTexture9	*potion_button_text;
	D3DXIMAGE_INFO		potion_button_Info;
	IDirect3DTexture9	*helm_button_text;
	D3DXIMAGE_INFO		helm_button_Info;
	IDirect3DTexture9	*chest_button_text;
	D3DXIMAGE_INFO		chest_button_Info;
	IDirect3DTexture9	*accesory_button_text;
	D3DXIMAGE_INFO		accesory_button_Info;

	IDirect3DTexture9	*save_text;
	D3DXIMAGE_INFO		save_info;
	
	IDirect3DTexture9	*save_buttons_text;
	D3DXIMAGE_INFO		save_button_info;

	IDirect3DTexture9	*introTexture,
						*introButtonTexture;
	D3DXIMAGE_INFO		introInfo,
						introButtonInfo;

	// battle
	IDirect3DTexture9	* bStatusWindow,
						* bWindow,
						* bMenu,
						* bPointer,
						* bActWin,
						* bMageSkills,
						* bActItemWin;

	D3DXIMAGE_INFO		bStatusWindowInfo,
						bWindowInfo,
						bMenuInfo,
						bPointerInfo,
						bActWinInfo,
						bMageSkillsInfo,
						bActItemWinInfo;
	
	
	//////////////////////////////////////////////////////////////////

	GraphicsManager2D();

	//////////////////////////////////////////////////////////////////
	//Name:			setup											//
	//Parameters:	IDirect3DDevice9* device - the 3D Device object	//
	//				LPCWSTR fileName - the file name				//
	//				IDirect3DTexture9 *texture - the texture object	//
	//					associated with the file name.				//
	//				D3DXIMAGE_INFO &info - the imageinfo object		//
	//					associated with the file name.				//
	//Return:		void											//
	//Description:  An internal helper function used to simplify	//
	//				creating a texture from a file.  The function	//
	//				will also push the textures into a vector and	//
	//				the imageinfo into a vector for ease of use.	//
	//////////////////////////////////////////////////////////////////
	void setup(IDirect3DDevice9* device, LPCWSTR fileName, IDirect3DTexture9 *texture, D3DXIMAGE_INFO &info);

public:
	~GraphicsManager2D();

	static GraphicsManager2D* instance();

	//////////////////////////////////////////////////////////////////////////
	//Name:			init													//
	//Parameters:	IDirect3DDevice9 *Device - the 3D Device Object			//
	//Return:		void													//
	//Description:	Loads textures and texture info into their own			//
	//				respective vectors.										//
	//////////////////////////////////////////////////////////////////////////
	void init(IDirect3DDevice9 *Device);

	
	//////////////////////////////////////////////////////////////////////////
	//Name:			Draw2DObject											//
	//Parameters:	D3DXVECTOR3 &scale - a vector used to scale the texture	//
	//				D3DXVECTOR3 &translate - a vector used to translate the //
	//										texture.						//
	//				D3DXVECTOR3 &rotate - a vector used to rotate the		//
	//										texture.						//
	//				RECT &r - a RECT struct used to draw a certain area of a//
	//							texture.									//
	//				ID3DXSprite *spriteObj - the D3DSprite object			//
	//				unsigned int OBJECT_VALUE - the enum value associated	//
	//							with the texture to be drawn.				//
	//				D3DCOLOR color - the color of the texture				//
	//Return:		void													//
	//Description:	Overloaded function to use RECT to draw certain areas of//
	//				a texture.												//
	//				NOTE:  Might not be needed anymore, since DrawButton	//
	//						does a better job.								//
	//				NOTE:  enum values used for OBJECT_VALUE are found in	//
	//						GameEnums.h										//
	//////////////////////////////////////////////////////////////////////////
	void Draw2DObject(D3DXVECTOR3 &scale, D3DXVECTOR3 &translate, D3DXVECTOR3 &rotate, RECT &r,
						ID3DXSprite *spriteObj, unsigned int OBJECT_VALUE,
						D3DCOLOR color);

	//////////////////////////////////////////////////////////////////////////
	//Name:			Draw2DObject											//
	//Parameters:	D3DXVECTOR3 &scale - a vector used to scale the texture	//
	//				D3DXVECTOR3 &translate - a vector used to translate the //
	//										texture.						//
	//				D3DXVECTOR3 &rotate - a vector used to rotate the		//
	//										texture.						//
	//				ID3DXSprite *spriteObj - the D3DSprite object			//
	//				unsigned int OBJECT_VALUE - the enum value associated	//
	//							with the texture to be drawn.				//
	//				D3DCOLOR - the color of the texture						//
	//Return:		void													//
	//Description:	Simple function used to draw a texture with the given	//
	//				parameters.  Enum values for OBJECT_VALUE can be found	//
	//				in GameEnums.h											//
	//////////////////////////////////////////////////////////////////////////
	void Draw2DObject(D3DXVECTOR3 &scale, D3DXVECTOR3 &translate, D3DXVECTOR3 &rotate,
						ID3DXSprite *spriteObj, unsigned int OBJECT_VALUE,
						D3DCOLOR color);

	//////////////////////////////////////////////////////////////////////////
	//Name:			DrawButton												//
	//Parameters:	D3DXVECTOR3 &scale - a vector used to scale the texture	//
	//				D3DXVECTOR3 &translate - a vector used to translate the //
	//										texture.						//
	//				D3DXVECTOR3 &rotate - a vector used to rotate the		//
	//										texture.						//
	//				RECT &r - a RECT struct used to draw a certain area of a//
	//							texture.									//
	//				ID3DXSprite *spriteObj - the D3DSprite object			//
	//				unsigned int OBJECT_VALUE - the enum value associated	//
	//							with the texture to be drawn.				//
	//				int width - the width of the button. 					//
	//				int height - the height of the button.					//
	//				D3DCOLOR color	- the color of the texture.				//
	//Return:		void													//
	//Description:	Simple function used to draw a texture with the given	//
	//				parameters.  Enum values for OBJECT_VALUE can be found	//
	//				in GameEnums.h											//
	//////////////////////////////////////////////////////////////////////////
	void DrawButton(D3DXVECTOR3 &scale, D3DXVECTOR3 &translate, D3DXVECTOR3 &rotate, RECT &r,
						ID3DXSprite *spriteObj, unsigned int OBJECT_VALUE,
						int width, int height, D3DCOLOR color);


	//////////////////////////////////////////////////////////////////////////
	//Name:			shutdown												//
	//Parameters:	none													//
	//Return:		void													//
	//Description:	clears the vector of pointers.  Releases the textures.	//
	//				Textures need to be manually released in shutdown.		//
	//////////////////////////////////////////////////////////////////////////

	void shutdown();

						
};