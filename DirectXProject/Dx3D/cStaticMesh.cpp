#include "stdafx.h"
#include "cStaticMesh.h"

cStaticMesh::cStaticMesh()
	: m_pStaticMesh(NULL)
	, m_vMtrls(0)
	, m_vTexture(0)
{
	D3DXMatrixIdentity(&m_pmatWorld);
}

cStaticMesh::~cStaticMesh()
{
	for each (auto p in m_vTexture)
		SAFE_RELEASE(p);

	SAFE_RELEASE(m_pStaticMesh);
}

bool cStaticMesh::Setup(char* szDirectory, char* szFilename)
{
	HRESULT hr = 0;

	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls = 0;

	std::string sFullPath(szDirectory);
	sFullPath += std::string(szFilename);

	hr = D3DXLoadMeshFromX(
		sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&m_pStaticMesh);

	if (FAILED(hr))
	{
		::MessageBox(0, "D3DLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}

	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			m_vMtrls.push_back(mtrls[i].MatD3D);

			if (mtrls[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;

				std::string sTexturePath(szDirectory);
				sTexturePath += std::string(mtrls[i].pTextureFilename);

				D3DXCreateTextureFromFile(g_pD3DDevice,
					sTexturePath.c_str(),
					&tex);

				m_vTexture.push_back(tex);
			}
			else
			{
				m_vTexture.push_back(0);
			}
		}
	}

	return true;
}

void cStaticMesh::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_pmatWorld);

	for (int i = 0; i < m_vMtrls.size(); i++)
	{
		g_pD3DDevice->SetMaterial(&m_vMtrls[i]);
		g_pD3DDevice->SetTexture(0, m_vTexture[i]);
		m_pStaticMesh->DrawSubset(i);
	}
}
