#include "GraphicsManager3D.h"

GraphicsManager3D::GraphicsManager3D()
{
	m_pEffectError = 0;
	m_pEffect = 0;

	m_pD3DVertexDecl = 0;

	m_pAdjBuffer = 0;

	m_mapMesh = 0;
	m_mapMaterials = 0;
	m_mapEffects = 0;
	mapMeshMaterials = 0;
	mapMeshTextures = 0;

	m_playerMesh = 0;
	m_playerMaterials = 0;
	m_playerEffects = 0;
	playerMeshMaterials = 0;
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

	//////////////////////////////////////////////////////////////////////////
	// clueBoard.x Mesh
	//////////////////////////////////////////////////////////////////////////
	D3DXLoadMeshFromX(L"ClueBoard.x", D3DXMESH_MANAGED, device, &m_pAdjBuffer, 
		&m_mapMaterials, &m_mapEffects, &m_numMapMaterials, &m_mapMesh);

	// Pull material (including texture) information from loaded .x file
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)m_mapMaterials->GetBufferPointer();

	mapMeshMaterials	= new D3DMATERIAL9[m_numMapMaterials];
	mapMeshTextures	= new LPDIRECT3DTEXTURE9[m_numMapMaterials];

	for(DWORD i = 0; i < m_numMapMaterials; ++i)
	{
		// Copy the material
		mapMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color (if needed) for the material (D3DX does not do this)
		mapMeshMaterials[i].Ambient  = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

		//// Create the texture if it exists - it may not
		mapMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename)
		{
			// Need to convert the texture filenames to Unicode string
			int len = 0;
			len = (int)strlen(d3dxMaterials[i].pTextureFilename) + 1;
			wchar_t *ucString = new wchar_t[len];
			//mbstowcs(ucString, d3dxMaterials[i].pTextureFilename, len);
			LPCWSTR filename = (LPCWSTR)ucString;

			// Load the texture now that we have a valid filename
			D3DXCreateTextureFromFile(device, filename, &mapMeshTextures[i]);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// cluePiece.x Mesh
	//////////////////////////////////////////////////////////////////////////
	D3DXLoadMeshFromX(L"dood.x", D3DXMESH_MANAGED, device, &m_pAdjBuffer, 
		&m_playerMaterials, &m_playerEffects, &m_numPlayerMaterials, &m_playerMesh);

	// Pull material (including texture) information from loaded .x file
	d3dxMaterials = (D3DXMATERIAL*)m_playerMaterials->GetBufferPointer();

	playerMeshMaterials	= new D3DMATERIAL9[m_numPlayerMaterials];
	//boardMeshTextures	= new LPDIRECT3DTEXTURE9[m_numBoardMaterials];

	for(DWORD i = 0; i < m_numPlayerMaterials; ++i)
	{
		// Copy the material
		playerMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color (if needed) for the material (D3DX does not do this)
		playerMeshMaterials[i].Ambient  = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	}

	// Load index info, refers into index into verts array to compose triangles
	// Note: A clockwise winding order of verts will show the front face.


	D3DXVECTOR3 position = D3DXVECTOR3(-10.0,0.0,-10.0);
	for(int i = 0; i<5; i++){
		position.z+=4.0f;
		Enemies.push_back(position);
		position.x = 10.0f;
		Players.push_back(position);
		position.x = -10.0f;
	}

	m_Map.init();




}

void GraphicsManager3D::Draw3DObject(D3DXVECTOR3 &scale, D3DXVECTOR3&translate, D3DXVECTOR3 &rotate,
									 ID3DXSprite *spriteObj, unsigned int ENUM_VAL, D3DCOLOR color,
									 GraphicsManager2D *GManager)
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

		for(DWORD i = 0; i < m_numMapMaterials; i++)
		{}
	
	}
}