#include "DirectXFramework.h"


void CDirectXFramework::updateShop()
{
	m_pDIMouse->Acquire();
	m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
	m_pDIKeyboard->Acquire();
	m_pDIKeyboard->GetDeviceState(sizeof(buffer), &buffer);

	buyList = false;
	sellList = false;

	if(buffer[DIK_B] & 0x80)
	{
		if(!m_keyPressed[DIK_B])
		{
			buyList = true;
			m_keyPressed[DIK_B] = true;
		}
	}
	else{if(m_keyPressed[DIK_B])
		m_keyPressed[DIK_B] = false;}

	if(buffer[DIK_S] & 0x80)
	{
		if(!m_keyPressed[DIK_S])
		{
			sellList = true;
			m_keyPressed[DIK_S] = true;
		}
	}
	else{if(m_keyPressed[DIK_S])
		m_keyPressed[DIK_S] = false;}

	if(buffer[DIK_E] & 0x80)
	{
		if(!m_keyPressed[DIK_E])
		{
			m_gameState = OVERWORLD;
			m_keyPressed[DIK_E] = true;
		}
	}
	else{if(m_keyPressed[DIK_E])
		m_keyPressed[DIK_E] = false;}

	if(buyList)
	{
		//new menu for items the shop has available

	}
	
	if(sellList)
	{
		//new menu popup for items in inventroy
	}

}

void CDirectXFramework::RenderShop()
{
	////////////////////////////////////////////////////////////
	// Basic shop Menu
	///////////////////////////////////////////////////////////
	if(SUCCEEDED(m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK)))
	{
		D3DXMATRIX shopScale, shopWorld, shopTranslation, shopRot;

		D3DXMatrixIdentity(&shopScale);
		D3DXMatrixIdentity(&shopWorld);
		D3DXMatrixIdentity(&shopTranslation);
		D3DXMatrixIdentity(&shopRot);

		D3DXMatrixScaling(&shopScale, 2.0f, 2.0f, 2.0f);
		D3DXMatrixTranslation(&shopTranslation, 150.0f, 150.0f, 0.0f);
		D3DXMatrixMultiply(&shopScale, &shopScale, &shopRot);
		D3DXMatrixMultiply(&shopWorld, &shopScale, &shopTranslation);

		m_pD3DSprite->SetTransform(&shopWorld);

		m_pD3DSprite->Draw(m_shopTexture, 0, &D3DXVECTOR3 (m_shopInfo.Width * 0.5f, m_shopInfo.Height * 0.5f, 0.0f),
			0, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_pD3DSprite->End();
	}
}