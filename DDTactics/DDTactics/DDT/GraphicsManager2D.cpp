#include "GraphicsManager2D.h"

GraphicsManager2D::GraphicsManager2D()
{
	textures.clear();
	imageInfos.clear();
	cursorTexture = 0;
	menuTexture = 0;
	buttonTexture = 0;
}

GraphicsManager2D::~GraphicsManager2D()
{}

GraphicsManager2D* GraphicsManager2D::instance()
{
	static GraphicsManager2D Gmanager;
	return &Gmanager;
}

void GraphicsManager2D::init(IDirect3DDevice9 *Device)
{
	
	setup(Device, L"TitleScreen.png", menuTexture, menuInfo);
	setup(Device, L"menuButtons.png", buttonTexture, buttonInfo);
	setup(Device, L"cursor.png", cursorTexture, cursorInfo);

	setup(Device, L"TOmap.jpg", overworldTexture, overworldInfo);
	setup(Device, L"DK_Cecil.png", characterTexture, overworldCharInfo);
	setup(Device, L"O_Menu_background.png", menuBackgroundTexture, menuBackgroundInfo);
	setup(Device, L"O_Menu_words.png", menuWordsTexture, menuBackgroundInfo);
	setup(Device, L"Town.png", townTexture, townInfo);
	setup(Device, L"book.png", bookTexture, bookInfo);
	setup(Device, L"swords.png", swordsTexture, swordsInfo);
	
	


	
	/*
		CALL setup function here with the parameters of the texture
	*/
	/////////////////////////////////////////////////////////////////////////////
	setup(Device, L"Status_background.png", status_background_Texture, status_background_Info);
	setup(Device, L"status_border.png", status_border_Texture, status_border_Info);
	setup(Device, L"Status_buttons.png", status_buttons_Texture, status_button_Info);

	setup(Device, L"FFTBattle.jpg", FFT_texture, FFT_Info);
	setup(Device, L"Shop.jpg", FFT_shop_text, FFT_shop_Info);
	setup(Device, L"BlackBackground.png", save_text, save_info);
	setup(Device, L"SaveMenuText.png", save_buttons_text, save_button_info);

	setup(Device, L"CastleBaron.png", introTexture, introInfo);
	setup(Device, L"IntroButton.png", introButtonTexture, introButtonInfo);

	//////////////////////////////////////////////////////////////////////////////

}

void GraphicsManager2D::setup(IDirect3DDevice9 *device, LPCWSTR fileName, IDirect3DTexture9 *texture, D3DXIMAGE_INFO &info)
{
	D3DXCreateTextureFromFileEx(device, fileName, 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DCOLOR_XRGB(255,0,255), &info, 0, &texture);

	textures.push_back(texture);
	imageInfos.push_back(info);
}

void GraphicsManager2D::Draw2DObject(D3DXVECTOR3 &scale, D3DXVECTOR3 &translate, D3DXVECTOR3 &rotate, RECT &r,
									 ID3DXSprite *spriteObj, unsigned int OBJECT_VALUE,
									 D3DCOLOR color)
{
	D3DXMATRIX	transMat, scaleMat, rotMat, worldMat;

	D3DXMatrixIdentity(&transMat);
	D3DXMatrixIdentity(&scaleMat);
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&worldMat);

	D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&transMat, translate.x, translate.y, translate.z);
	D3DXMatrixRotationYawPitchRoll(&rotMat, D3DXToRadian(rotate.y), D3DXToRadian(rotate.x), D3DXToRadian(rotate.z));
	D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
	D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

	spriteObj->SetTransform(&worldMat);
	spriteObj->Draw(textures[OBJECT_VALUE], &r, &D3DXVECTOR3(imageInfos[OBJECT_VALUE].Width *0.5f, imageInfos[OBJECT_VALUE].Height *0.5f, 0.0f),
		0, color);


}

void GraphicsManager2D::Draw2DObject(D3DXVECTOR3 &scale, D3DXVECTOR3 &translate, D3DXVECTOR3 &rotate, 
									 ID3DXSprite *spriteObj, unsigned int OBJECT_VALUE,
									 D3DCOLOR color)
{
	D3DXMATRIX	transMat, scaleMat, rotMat, worldMat;

	D3DXMatrixIdentity(&transMat);
	D3DXMatrixIdentity(&scaleMat);
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&worldMat);

	D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&transMat, translate.x, translate.y, translate.z);
	D3DXMatrixRotationYawPitchRoll(&rotMat, D3DXToRadian(rotate.y), D3DXToRadian(rotate.x), D3DXToRadian(rotate.z));
	D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
	D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

	spriteObj->SetTransform(&worldMat);
	spriteObj->Draw(textures[OBJECT_VALUE], 0, &D3DXVECTOR3(imageInfos[OBJECT_VALUE].Width *0.5f, imageInfos[OBJECT_VALUE].Height *0.5f, 0.0f),
		0, color);


}


void GraphicsManager2D::DrawButton(D3DXVECTOR3 &scale, D3DXVECTOR3 &translate, D3DXVECTOR3 &rotate, RECT &r,
						ID3DXSprite *spriteObj, unsigned int OBJECT_VALUE,
						int width, int height, D3DCOLOR color)
{
	
	D3DXMATRIX	transMat, scaleMat, rotMat, worldMat;

	D3DXMatrixIdentity(&transMat);
	D3DXMatrixIdentity(&scaleMat);
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&worldMat);

	D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&transMat, translate.x, translate.y, translate.z);
	D3DXMatrixRotationYawPitchRoll(&rotMat, D3DXToRadian(rotate.y), D3DXToRadian(rotate.x), D3DXToRadian(rotate.z));
	D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
	D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

	spriteObj->SetTransform(&worldMat);
	spriteObj->Draw(textures[OBJECT_VALUE], &r, &D3DXVECTOR3(width *0.5f, height *0.5f, 0.0f),
		0, color);
}

void GraphicsManager2D::shutdown()
{
	// clear/destroy the pointers in the vector
	textures.clear();
	
	//RELEASE Textures from memory
	SAFE_RELEASE(save_text);
	SAFE_RELEASE(save_buttons_text);
	SAFE_RELEASE(FFT_shop_text);
	SAFE_RELEASE(FFT_texture);
	SAFE_RELEASE(status_buttons_Texture);
	SAFE_RELEASE(status_border_Texture);
	SAFE_RELEASE(status_background_Texture);
	SAFE_RELEASE(introTexture);
	SAFE_RELEASE(introButtonTexture);

	SAFE_RELEASE(swordsTexture);
	SAFE_RELEASE(bookTexture);
	SAFE_RELEASE(townTexture);
	SAFE_RELEASE(menuWordsTexture);
	SAFE_RELEASE(menuBackgroundTexture);
	SAFE_RELEASE(characterTexture);
	SAFE_RELEASE(overworldTexture);

	SAFE_RELEASE(cursorTexture);
	SAFE_RELEASE(buttonTexture);
	SAFE_RELEASE(menuTexture);
	
}