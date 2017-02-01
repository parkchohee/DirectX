#include "stdafx.h"
#include "cStaticMesh.h"

cStaticMesh::cStaticMesh()
	: m_pStaticMesh(NULL)
	, m_vMtrls(0)
	, m_vTexture(0)
	, m_vMin(0, 0, 0)
	, m_vMax(0, 0, 0)
{
	D3DXMatrixIdentity(&m_pmatWorld);
}

cStaticMesh::cStaticMesh(char* szDirectory, char* szFilename)
	: m_pStaticMesh(NULL)
	, m_vMtrls(0)
	, m_vTexture(0)
	, m_vMin(0, 0, 0)
	, m_vMax(0, 0, 0)
{
	cStaticMesh* pStaticMesh = g_pStaticMeshManager->GetStaticMesh(szDirectory, szFilename);

	D3DXMatrixIdentity(&m_pmatWorld);

	m_pStaticMesh = pStaticMesh->m_pStaticMesh;
	m_vMtrls = pStaticMesh->m_vMtrls;
	m_vTexture = pStaticMesh->m_vTexture;
}

cStaticMesh::~cStaticMesh()
{
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

	// 메시에서 vertex를 가져온다. 

	//LPDIRECT3DVERTEXBUFFER9 vb = 0;
	//m_pStaticMesh->GetVertexBuffer(&vb);
	//ST_PNT_VERTEX * vertices = new ST_PNT_VERTEX[m_pStaticMesh->GetNumVertices()];
	//
	//
	//VOID* pVertices;
	//vb->Lock(0, sizeof(vertices), (void**)&pVertices, 0);
	//memcpy(vertices, pVertices, sizeof(m_pStaticMesh->GetFVF())*m_pStaticMesh->GetNumVertices());
	//{
	//	D3DXVECTOR3 vMin(0, 0, 0), vMax(0, 0, 0);

	//	D3DXComputeBoundingBox((D3DXVECTOR3*)pVertices,
	//		m_pStaticMesh->GetNumVertices(),
	//		D3DXGetFVFVertexSize(m_pStaticMesh->GetFVF()),
	//		&vMin,
	//		&vMax);

	//	D3DXVec3Minimize(&m_vMin, &m_vMin, &vMin);
	//	D3DXVec3Maximize(&m_vMax, &m_vMax, &vMax);
	//}
	//vb->Unlock();

	//SAFE_RELEASE(vb);

	// vertex중에서 max와 min을 셋팅..
	// D3DXVec3Maximize(1,2,3);	// 2 3중 큰걸 1에저장해줌
	/*for (int i = 0; i < m_pStaticMesh->GetNumVertices(); i++)
	{
		D3DXVec3Minimize(&m_vMin, &m_vMin, &vertices[i].p);
		D3DXVec3Maximize(&m_vMax, &m_vMax, &vertices[i].p);
	}*/

	int a = 0;


	return true;
}

void cStaticMesh::Render()
{
	int a = 0;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_pmatWorld);

	for (int i = 0; i < m_vMtrls.size(); i++)
	{
		g_pD3DDevice->SetMaterial(&m_vMtrls[i]);
		g_pD3DDevice->SetTexture(0, m_vTexture[i]);
		m_pStaticMesh->DrawSubset(i);
	}
}

void cStaticMesh::Destroy()
{
	for each (auto p in m_vTexture)
		SAFE_RELEASE(p);

	SAFE_RELEASE(m_pStaticMesh);
}
