#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <vector>

#define DEBUGGING

#define COM_RELEASE(x) if(x){x->Release(); x = 0;}

#include "CGraph.h"
#include "GraphicsManager2D.h"

class GraphicsManager3D
{
private:
	GraphicsManager3D();

	//////////////////////////////////////////////////////////////////////////
	// View and Projection Matrices
	//////////////////////////////////////////////////////////////////////////
	// Set these 3 component to initialize the view matrix (camera)
	D3DXVECTOR3 m_eyePos;
	D3DXVECTOR3 m_lookAt;
	D3DXVECTOR3 m_upVec;

	// View matrix
	D3DXMATRIX m_viewMat;

	// Projection matrix
	D3DXMATRIX m_projMat;

	//////////////////////////////////////////////////////////////////////////
	// Lighting, Material, Textures
	//////////////////////////////////////////////////////////////////////////
	D3DLIGHT9			m_Light;

	//////////////////////////////////////////////////////////////////////////
	// Effects
	//////////////////////////////////////////////////////////////////////////
	ID3DXBuffer*		m_pEffectError;
	ID3DXEffect*		m_pEffect;
	D3DXHANDLE			m_hTech;

	//////////////////////////////////////////////////////////////////////////
	// D3DXMesh - Used for D3DXCreate functions and .x files
	//////////////////////////////////////////////////////////////////////////
	IDirect3DVertexDeclaration9*	m_pD3DVertexDecl;

	// Adjacency Buffer
	ID3DXBuffer*		m_pAdjBuffer;	
	
	// .x File Meshes

	// Map Mesh
	ID3DXMesh*			m_mapMesh;
	ID3DXBuffer*		m_mapMaterials;
	ID3DXBuffer*		m_mapEffects;
	DWORD				m_numMapMaterials;
	D3DMATERIAL9*		mapMeshMaterials;	
	LPDIRECT3DTEXTURE9*	mapMeshTextures;	

	// Player Mesh
	ID3DXMesh*			m_playerMesh;
	ID3DXBuffer*		m_playerMaterials;
	ID3DXBuffer*		m_playerEffects;
	DWORD				m_numPlayerMaterials;
	D3DMATERIAL9*		playerMeshMaterials;	

	int m_frames,
		m_gameState,
		m_controling;
	float m_lastTime;
	std::vector<D3DXVECTOR3> Players;
	std::vector<D3DXVECTOR3> Enemies;

	CGraph m_Map;
public:
	~GraphicsManager3D();
	static GraphicsManager3D *instance();

	void Init(IDirect3DDevice9* device);

	void Draw3DObject(D3DXVECTOR3 &scale, D3DXVECTOR3 &translate, D3DXVECTOR3 &rotate,
						ID3DXSprite *spriteObj, unsigned int ENUM_VAL, D3DCOLOR color,
						GraphicsManager2D *GManager);

	void Shutdown();


};