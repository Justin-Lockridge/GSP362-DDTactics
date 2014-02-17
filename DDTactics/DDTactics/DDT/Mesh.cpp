#include "Mesh.h"

Mesh::Mesh()
{
	mesh = 0;
	material_buffer = 0;
	effect = 0;
	mesh_material = 0;
	meshTextures = 0;
}

Mesh::~Mesh()
{}


void Mesh::loadMap(LPCWSTR filename, IDirect3DDevice9 *device, ID3DXBuffer *adjBuffer)
{
	D3DXLoadMeshFromX( filename, 
						D3DXMESH_MANAGED, 
						device, 
						&adjBuffer, 
						&material_buffer, 
						&effect,
						&numMaterials, 
						&mesh);


	//Pull material (including texture) information from loaded .x file
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)material_buffer->GetBufferPointer();

	mesh_material = new D3DMATERIAL9[numMaterials];
	meshTextures = new LPDIRECT3DTEXTURE9[numMaterials];

	for(DWORD i = 0; i <numMaterials; ++i)
	{
		//Copy the material
		mesh_material[i] = d3dxMaterials[i].MatD3D;

		//Set the ambient color (if needed) for the material (D3DX doesn't do this)
		mesh_material[i].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

		//Create the texture if it exists - it may not
		meshTextures[i] = NULL;

		if(d3dxMaterials[i].pTextureFilename)
		{
			//Need to convert the texture filenames to Unicode string
			int len = 0;
			len = (int)strlen(d3dxMaterials[i].pTextureFilename) + 1;
			wchar_t *ucString = new wchar_t[len];
			mbstowcs(ucString, d3dxMaterials[i].pTextureFilename, len);
			LPCWSTR newfilename = (LPCWSTR)ucString;

			//Load the texture now that we have a valid filename

			D3DXCreateTextureFromFile(device, newfilename, &meshTextures[i]);
		
		
		}
	
	
	}
}

void Mesh::loadCharacterMesh(LPCWSTR filename, IDirect3DDevice9 *device, ID3DXBuffer *adjBuffer)
{
	D3DXLoadMeshFromX(filename, 
						D3DXMESH_MANAGED, 
						device, 
						&adjBuffer,
						&material_buffer,
						&effect, 
						&numMaterials, 
						&mesh);

	// Pull material (including texture) information from loaded .x file
	D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)material_buffer->GetBufferPointer();

	mesh_material = new D3DMATERIAL9[numMaterials];

	for(DWORD i = 0; i < numMaterials; ++i)
	{
		//Copy the material
		mesh_material[i] = d3dxMaterials[i].MatD3D;

		//Set the ambient color (if needed) for the material (D3DX doesn't do this)
		mesh_material[i].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
				
	}
	
}


ID3DXMesh* Mesh::getMesh()
{
	return mesh;
}

DWORD Mesh::getNumMaterials()
{
	return numMaterials;
}

D3DMATERIAL9* Mesh::getMeshMaterial()
{
	return mesh_material;
}

LPDIRECT3DTEXTURE9 *Mesh::getMeshTexture()
{
	return meshTextures;
}


void Mesh::shutdown()
{

	if(effect)
	{
		effect->Release();
		effect = 0;
	}

	if(material_buffer)
	{
		material_buffer->Release();
		material_buffer = 0;
	}

	if(mesh)
	{
		mesh->Release();
		mesh = 0;
	}
}