#pragma once
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "winmm.lib")

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

class Mesh
{
private:

	ID3DXMesh*			mesh;
	ID3DXBuffer*		material_buffer;
	ID3DXBuffer*		effect;
	DWORD				numMaterials;
	D3DMATERIAL9*		mesh_material;
	LPDIRECT3DTEXTURE9 *meshTextures;
	

public:

	Mesh();
	~Mesh();
	
	void loadMap(LPCWSTR filename, IDirect3DDevice9 *device, ID3DXBuffer *adjBuffer);

	void loadCharacterMesh(LPCWSTR filename, IDirect3DDevice9* device, ID3DXBuffer *adjBuffer);


	ID3DXMesh* getMesh();
	DWORD getNumMaterials();
	D3DMATERIAL9* getMeshMaterial();
	LPDIRECT3DTEXTURE9 *getMeshTexture();

	void shutdown();




};