#include "GraphicsManager3D.h"

GraphicsManager3D::GraphicsManager3D()
{
	m_pEffectError = 0;
	m_pEffect = 0;

	m_pD3DVertexDecl = 0;

	m_pAdjBuffer = 0;


}

GraphicsManager3D::~GraphicsManager3D()
{}

GraphicsManager3D *GraphicsManager3D::instance()
{
	static GraphicsManager3D graphics3D;
	return &graphics3D;
}

void GraphicsManager3D::Init(IDirect3DDevice9* device)
{
	//////////////////////////////////////////////////////////////////////////	
	// View and Projection Matrices
	//////////////////////////////////////////////////////////////////////////
	// Initialize View Matrix
	m_eyePos	= D3DXVECTOR3(10.0f, 10.0f, -17.0f);
	m_lookAt	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_upVec		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// Easily calculate the view matrix with 3 intuitive vectors
	D3DXMatrixLookAtLH(
		&m_viewMat,	
		&m_eyePos,	
		&m_lookAt,	
		&m_upVec);

	// Apply the view matrix in the scene
	device->SetTransform(D3DTS_VIEW, &m_viewMat);

	D3DXMatrixOrthoLH(
		&m_projMat,			// Returned ProjMat
		36,	24,				// Width, Height
		1.0f,				// Near Plane
		1000.0f);	

	// Apply the projection matrix in the scene
	device->SetTransform(D3DTS_PROJECTION, &m_projMat);

	D3DVERTEXELEMENT9 decl[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};

	device->CreateVertexDeclaration(decl, &m_pD3DVertexDecl);

	//////////////////////////////////////////////////////////////////////////
	// Light
	//////////////////////////////////////////////////////////////////////////
	ZeroMemory(&m_Light, sizeof(m_Light));

	m_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light.Type = D3DLIGHT_POINT;	
	m_Light.Position = D3DXVECTOR3(0.0f, 10.0f, 0.0f); 
	m_Light.Range = 500.0f;
	m_Light.Attenuation0 = 1.0f;	
	m_Light.Attenuation1 = 0.0f;	
	m_Light.Attenuation2 = 0.0f;	

	device->SetLight(0, &m_Light);	
	device->LightEnable(0, true);

	//////////////////////////////////////////////////////////////////////////
	// Create a Font Object
	//////////////////////////////////////////////////////////////////////////
	//D3DXCreateFont(device, 40, 13, FW_BOLD, 0, false, DEFAULT_CHARSET, 
	//	OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Buxton Sketch"), &m_pD3DFont);


	//////////////////////////////////////////////////////////////////////////
	// Effect
	//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG	// If Debug flag is set in compiler, use shader debug

	m_pEffectError = 0;
	D3DXCreateEffectFromFile(device, L"Shader.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, 
		0, &m_pEffect, &m_pEffectError);


#else			// else do not use debug flag
	D3DXCreateEffectFromFile(device, L"Shader.fx", 0, 0, 0, 0, &m_pEffect,
		&m_pEffectError);

#endif

	m_pEffect->SetFloatArray("eyePos", (float*)m_eyePos, 3);
	m_pEffect->SetFloatArray("lightPos", (float*)&m_Light.Position, 3);
	m_pEffect->SetFloatArray("ambientLight", (float*)&m_Light.Ambient, 3);
	m_pEffect->SetFloatArray("specularLight", (float*)&m_Light.Specular, 3);
	m_pEffect->SetFloatArray("diffuseLight", (float*)&m_Light.Diffuse, 3);
	m_pEffect->SetFloatArray("lightAttenuation", (float*)&D3DXVECTOR3(m_Light.Attenuation0, m_Light.Attenuation1, m_Light.Attenuation2), 3);

	map.loadMap(L"ClueBoard.x", device, m_pAdjBuffer);
	maps_mesh.push_back(map);

	default_character.loadCharacterMesh(L"dood.x", device, m_pAdjBuffer);
	character_mesh.push_back(default_character);


	m_Map.init();


}

void GraphicsManager3D::DrawMap(D3DXVECTOR3 &scale, D3DXVECTOR3&translate, D3DXVECTOR3 &rotate,
								unsigned int ENUM_VAL)
{
	D3DXMATRIX rotMat, worldMat, transMat, scaleMat, WIT;

	UINT numPasses = 0;
	numPasses = 0;

	D3DXMatrixIdentity(&transMat);
	D3DXMatrixIdentity(&scaleMat);
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&worldMat);

	m_pEffect->Begin(&numPasses, 0);

	for(UINT i = 0; i < numPasses; i++)
	{
		m_pEffect->BeginPass(i);

		D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
		D3DXMatrixRotationYawPitchRoll(&rotMat, D3DXToRadian(rotate.y), D3DXToRadian(rotate.x), D3DXToRadian(rotate.z));
		D3DXMatrixTranslation(&transMat, translate.x, translate.y, translate.z);
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

		D3DXMatrixInverse(&WIT, 0, &worldMat);
		D3DXMatrixTranspose(&WIT, &WIT);
		m_pEffect->SetMatrix("worldViewProjMat", &(worldMat * m_viewMat * m_projMat));
		m_pEffect->SetMatrix("worldInverseTransposeMat", &WIT);
		m_pEffect->SetMatrix("worldMat", &worldMat);


		//D3DMATERIAL9* tempMat = maps_mesh[ENUM_VAL]
		for(DWORD i = 0; i < maps_mesh[ENUM_VAL].getNumMaterials(); i++)
		{

			// Set the material and texture for this subset
			m_pEffect->SetValue("ambientMaterial", &maps_mesh[ENUM_VAL].getMeshMaterial()->Ambient, sizeof(D3DXCOLOR));
			m_pEffect->SetValue("diffuseMaterial", &maps_mesh[ENUM_VAL].getMeshMaterial()->Diffuse, sizeof(D3DXCOLOR));
			m_pEffect->SetValue("specularMaterial", &maps_mesh[ENUM_VAL].getMeshMaterial()->Specular, sizeof(D3DXCOLOR));
			m_pEffect->SetFloat("specularPower", maps_mesh[ENUM_VAL].getMeshMaterial()->Power);
			m_pEffect->SetTexture("tex", maps_mesh[ENUM_VAL].getMeshTexture()[i]);
			m_pEffect->SetBool("isTex", true);

			m_pEffect->CommitChanges();

			maps_mesh[ENUM_VAL].getMesh()->DrawSubset(i);

		}

		m_pEffect->EndPass();

	}

	m_pEffect->End();
}

void GraphicsManager3D::DrawCharacter(D3DXVECTOR3 &scale, D3DXVECTOR3 &translate, D3DXVECTOR3 &rotate,
						unsigned int ENUM_VAL)
{
	D3DXMATRIX rotMat, worldMat, transMat, scaleMat, WIT;

	UINT numPasses = 0;
	numPasses = 0;

	D3DXMatrixIdentity(&transMat);
	D3DXMatrixIdentity(&scaleMat);
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&worldMat);

	m_pEffect->Begin(&numPasses, 0);

	for(UINT i = 0; i < numPasses; i++)
	{
		m_pEffect->BeginPass(i);

		D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
		D3DXMatrixRotationYawPitchRoll(&rotMat, D3DXToRadian(rotate.y), D3DXToRadian(rotate.x), D3DXToRadian(rotate.z));
		D3DXMatrixTranslation(&transMat, translate.x, translate.y, translate.z);
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

		D3DXMatrixInverse(&WIT, 0, &worldMat);
		D3DXMatrixTranspose(&WIT, &WIT);
		m_pEffect->SetMatrix("worldViewProjMat", &(worldMat * m_viewMat * m_projMat));
		m_pEffect->SetMatrix("worldInverseTransposeMat", &WIT);
		m_pEffect->SetMatrix("worldMat", &worldMat);


		//D3DMATERIAL9* tempMat = maps_mesh[ENUM_VAL]
		for(DWORD i = 0; i < character_mesh[ENUM_VAL].getNumMaterials(); i++)
		{

			// Set the material and texture for this subset
			m_pEffect->SetValue("ambientMaterial", &character_mesh[ENUM_VAL].getMeshMaterial()->Ambient, sizeof(D3DXCOLOR));
			m_pEffect->SetValue("diffuseMaterial", &character_mesh[ENUM_VAL].getMeshMaterial()->Diffuse, sizeof(D3DXCOLOR));
			m_pEffect->SetValue("specularMaterial", &character_mesh[ENUM_VAL].getMeshMaterial()->Specular, sizeof(D3DXCOLOR));
			m_pEffect->SetFloat("specularPower", character_mesh[ENUM_VAL].getMeshMaterial()->Power);
			//m_pEffect->SetTexture("tex", character_mesh[ENUM_VAL].getMeshTexture()[i]);
			m_pEffect->SetBool("isTex", false);

			m_pEffect->CommitChanges();

			character_mesh[ENUM_VAL].getMesh()->DrawSubset(i);

		}

		m_pEffect->EndPass();

	}

	m_pEffect->End();


}

void GraphicsManager3D::Shutdown()
{
	//Clear out vector pointers
	maps_mesh.clear();
	character_mesh.clear();
	non_character_enemies.clear();

	//shutdown Mesh objects
	map.shutdown();
	default_character.shutdown();

	//Release adjacency buffer
	if(m_pAdjBuffer)
	{
		m_pAdjBuffer->Release();
		m_pAdjBuffer = 0;
	}

	//Release verxtex decl structure
	if(m_pD3DVertexDecl)
	{
		m_pD3DVertexDecl->Release();
		m_pD3DVertexDecl = 0;
	}

	//release Effect handle
	if(m_pEffect)
	{
		m_pEffect->Release();
		m_pEffect = 0;
	}

	//release effect error buffer
	if(m_pEffectError)
	{
		m_pEffectError->Release();
		m_pEffectError = 0;
	}

}