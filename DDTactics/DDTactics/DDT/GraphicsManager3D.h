#pragma once

#include "Mesh.h"
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
	
	Mesh	map;
	Mesh	default_character;

	std::vector<Mesh> maps_mesh;
	std::vector<Mesh> character_mesh;
	std::vector<Mesh> non_character_enemies;


	int m_frames,
		m_gameState,
		m_controling;
	float m_lastTime;

	CGraph m_Map;
public:
	~GraphicsManager3D();
	static GraphicsManager3D *instance();

	void Init(IDirect3DDevice9* device);

	void DrawMap(D3DXVECTOR3 &scale, D3DXVECTOR3 &translate, D3DXVECTOR3 &rotate,
						unsigned int ENUM_VAL
						);

	void DrawCharacter(D3DXVECTOR3 &scale, D3DXVECTOR3 &translate, D3DXVECTOR3 &rotate,
						unsigned int ENUM_VAL
						);

	void Shutdown();


};